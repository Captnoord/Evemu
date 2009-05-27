/*
    ------------------------------------------------------------------------------------
    LICENSE:
    ------------------------------------------------------------------------------------
    This file is part of EVEmu: EVE Online Server Emulator
    Copyright 2006 - 2009 The EVEmu Team
    For the latest information visit http://evemu.mmoforge.org
    ------------------------------------------------------------------------------------
    This program is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by the Free Software
    Foundation; either version 2 of the License, or (at your option) any later
    version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License along with
    this program; if not, write to the Free Software Foundation, Inc., 59 Temple
    Place - Suite 330, Boston, MA 02111-1307, USA, or go to
    http://www.gnu.org/copyleft/lesser.txt.
    ------------------------------------------------------------------------------------
    Author:     Captnoord
*/

#include "EvemuPCH.h"

#include "Common.h"
#include "NGLog.h"
#include "Log.h"
#include "string_map.h"

#include "PyObjects.h"
#include "PyChameleon.h"
#include "PyDict.h"

/************************************************************************/
/* PyDict                                                               */
/************************************************************************/
PyDict::PyDict() : mType(PyTypeDict), mRefcnt(1), mMappingMode(true), mMappingIndex(0)
{
    mHash = &PyDict::_hash;
}
PyDict::~PyDict()
{
    int i = 0;
    iterator itr = mDict.begin();
    for (; itr != mDict.end(); itr++)
    {
        PyDictEntry * entry = itr->second;
        assert(entry);
        assert(entry->key);
        assert(entry->obj);

        entry->key->DecRef();
        entry->obj->DecRef();
        SafeDelete(entry);
        i++;
    }

    mDict.clear();
    mType = PyTypeDeleted;
}

uint8 PyDict::gettype()
{
    return mType;
}

PyChameleon PyDict::operator[]( const char* keyName )
{
    ASCENT_HARDWARE_BREAKPOINT;
    if (keyName == NULL || *keyName == '\0')
        return PyErrorIterator;

    uint32 hsh = Utils::Hash::sdbm_hash(keyName);

    PyDictEntry * entry = mDict[hsh];
    if (entry == NULL)
    {
        entry = new PyDictEntry;
        entry->key = (PyObject*)new PyString(keyName);
        entry->obj = NULL;
        mDict[hsh] = entry;
    }

    return entry;
}

void PyDict::IncRef()
{
    mRefcnt++;
}

void PyDict::DecRef()
{
    mRefcnt--;
    if (mRefcnt <= 0)
        PyDelete(this);
}

size_t PyDict::size()
{
    return mDict.size();
}

bool PyDict::set_item( PyObject* key, PyObject* obj )
{
    if (key == NULL || obj == NULL)
    {
        ASCENT_HARDWARE_BREAKPOINT;
        return false;
    }

    if (mMappingMode == true)
    {
        /* create a new dictionary entry */
        PyDictEntry * entry = new PyDictEntry;
        entry->key = key;
        entry->obj = obj;
        mDict.insert(std::make_pair(mMappingIndex++, entry));
        key->IncRef();  // we seem to reuse a object that is already in the system so increase its mojo
        obj->IncRef();
    }
    else
    {
        // hit debug as I want to make sure its working correctly.
        ASCENT_HARDWARE_BREAKPOINT;

        uint32 hsh = PyObject_Hash(key);

        iterator itr = mDict.find(hsh);

        /* check if we are updating a dictionary entry or creating a new one */
        if (itr == mDict.end())
        {
            /* create a new dictionary entry */
            PyDictEntry * entry = new PyDictEntry;
            entry->key = key;
            entry->obj = obj;
            mDict.insert(std::make_pair(hsh, entry));
            key->IncRef();  // we seem to reuse a object that is already in the system so increase its mojo
            obj->IncRef();
        }
        else
        {
            /* update/replace a already existing entry ( bit tricky ) */
            PyDictEntry * entry = itr->second;
            entry->obj->DecRef();
            entry->obj = obj;
            obj->IncRef();
        }
    }

    return true;
}

PyObject* PyDict::get_item( PyObject* key )
{
    ASCENT_HARDWARE_BREAKPOINT;
    if (key == NULL)
        return NULL;
    return NULL;
}

uint32 PyDict::hash()
{
    return (this->*mHash)();
}

uint32 PyDict::_hash()
{
    uint32 hsh = 0;
    iterator itr = mDict.begin();
    for (; itr != mDict.end(); itr++)
    {
        PyDictEntry * entry = itr->second;
        hsh|= PyObject_Hash(entry->key);
        hsh = hsh << 3;
        hsh|= PyObject_Hash(entry->obj);
    }

    return hsh;
}

PyDict::iterator PyDict::begin()
{
    return mDict.begin();
}

PyDict::iterator PyDict::end()
{
    return mDict.end();
}

bool PyDict::scanf( const char * keyName, const char* format, ... )
{
    /* useless call.. because the dict need a lookup callback for this.... lol I actualy don't care.*/
    update();

    uint32 hsh = Utils::Hash::sdbm_hash(keyName);

    PyDict::iterator fItr = mDict.find(hsh);
    if (fItr == mDict.end())
        return false;

    PyObject * foundObject = ((PyDictEntry *)fItr->second)->obj;

    va_list ap;
    va_start(ap, format);

    //sum += i;
    //count++;
    //i = va_arg( ap, (void*));

    void* pVar = NULL;
    int formatIndex = 0;
    //while (pVar != -1)
    //{
        pVar = va_arg( ap, void*);

        char tag = format[formatIndex];
        switch(tag)
        {
            /*unicode string*/
        case 'u':
            std::wstring * str = (std::wstring *)pVar;
            str->clear();

            size_t len = ((PyUnicodeUCS2*)foundObject)->length();
            wchar_t * buff = ((PyUnicodeUCS2*)foundObject)->content();
            str->append(buff, len);
            break;
        }

        //formatIndex++;
    //}

    va_end(ap);
    return true;
}

void PyDict::update()
{
    if(mMappingMode == true)
    {
        //ASCENT_HARDWARE_BREAKPOINT;
        PyDictEntry * entry = NULL;
        iterator itr = mDict.begin();
        iterator dItr;

        size_t dictSize = mDict.size();
        for (size_t i = 0; i < dictSize; ++i)
        {
            entry = itr->second;

            uint32 hsh = PyObject_Hash(entry->key);
            dItr = itr++;

            mDict.erase(dItr);
            mDict.insert(mDict.end(), std::make_pair(hsh, entry));
        }
        mMappingMode = false;
    }
}

bool PyDict::get_buffer( const char * keyName, char* dst, size_t dst_len )
{
    if (keyName == NULL || dst == NULL || dst_len == 0)
        return false;

    update();

    uint32 hsh = Utils::Hash::sdbm_hash(keyName);

    PyDict::iterator fItr = mDict.find(hsh);
    if (fItr == mDict.end())
        return false;

    PyObject * foundObject = ((PyDictEntry *)fItr->second)->obj;

    if (foundObject == NULL)
        return false;

    if (foundObject->gettype() != PyTypeString)
        return false;

    PyString * str = (PyString *)foundObject;

    /* check if we have enough room to read the string / buffer */
    if (str->length() > dst_len)
        return false;

    ASCENT_MEMCPY(dst, str->content(), str->length());
    return true;
}

bool PyDict::set_int( const char * keyName, int number )
{
    return set_item(keyName, (PyObject *)new PyInt(number));
}

bool PyDict::set_double( const char * keyName, double number )
{
    return set_item(keyName, (PyObject *)new PyFloat(number));
}

bool PyDict::set_str( const char * keyName, const char* str )
{
    return set_item(keyName, (PyObject *)new PyString(str));
}

bool PyDict::set_bool( const char * keyName, bool check )
{
    return set_item(keyName, (PyObject *)new PyBool(check));
}
