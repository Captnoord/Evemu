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

#ifndef _PYDICT_H
#define _PYDICT_H

#pragma pack(push,1)

class PyDict
{
    uint8 mType;
    size_t mRefcnt;
    uint32 (PyDict::*mHash)();
    /** mMappingMode makes it possible to simply store the objects
     */
    bool    mMappingMode;
    uint32  mMappingIndex;
public:
    uint8 gettype();
    void IncRef();
    void DecRef();
    uint32 hash();
public:
    PyDict();
    ~PyDict();
    PyChameleon operator[](const char* keyName);

    /**
     * \brief set_item adds or sets a database entry.
     *
     * PyDict::set_item handles the adding and setting of object in
     * mapped and non mapped dictionary's.
     *
     * @param[in]
     * @param[out]
     * @return
     */
    template<typename T>
    bool set_item(const char* key_name, T* obj)
    {
        if (key_name == NULL || obj == NULL)
        {
            /* something broke so debug it */
            ASCENT_HARDWARE_BREAKPOINT;
            return false;
        }

        if (mMappingMode == true)
        {
            /* create a new dictionary entry */
            PyDictEntry * entry = new PyDictEntry;
            entry->key = (PyObject*)new PyString(key_name);
            entry->obj = (PyObject*)obj;
            mDict.insert(std::make_pair(mMappingIndex++, entry));
            obj->IncRef();
        }
        else
        {
            /* IMPLEMENT THIS ONE..... */
            ASCENT_HARDWARE_BREAKPOINT;
            /* do the same as the set_item function that takes a PyObject as a key, but do some tricks with it */

            size_t str_len = strlen(key_name);

            /* test this and check these */
            // NOTE: not used.
            //uint32 hsh = Utils::Hash::sdbm_hash(key_name, (int)str_len);
            //uint32 hsh1 = Utils::Hash::sdbm_hash(key_name);
        }

        return true;
    }

    bool set_item(PyObject* key, PyObject* obj);
    PyObject* get_item(PyObject* key);
    PyObject* get_item(const char* key_name);

    bool scanf(const char * keyName, const char * format, ...);

    bool get_buffer(const char * keyName, char* dst, size_t dst_len);
    bool set_int(const char * keyName, int number);
    bool set_double(const char * keyName, double number);
    bool set_str(const char * keyName, const char* str);
    bool set_bool(const char * keyName, bool check);

    size_t size();

    /* atm only changing static mapped dict to hashed dict */
    void update();

private:
    typedef std::map<uint32, PyDictEntry*>  DictMap;
    typedef DictMap::iterator               DictMapItr;
public:
    typedef DictMap::iterator               iterator;
    iterator begin();
    iterator end();
private:
    DictMap mDict;
    uint32 _hash();
};

#pragma pack(pop)

#endif // _PYDICT_H
