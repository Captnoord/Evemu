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
#include "PyClass.h"

/************************************************************************/
/* PyClass                                                              */
/************************************************************************/
PyClass::PyClass() : mType(PyTypeClass), mRefcnt(1), mDict(NULL), mName(NULL), mBases(NULL), mDirivedCallList(NULL), mDirivedCallDict(NULL)
{
    mHash = &PyClass::_hash;
}

PyClass::~PyClass()
{
    if (mName)
    {
        mName->DecRef();
        mName = NULL;
    }
    if (mBases)
    {
        mBases->DecRef();
        mBases = NULL;
    }

    if (mDict)
    {
        mDict->DecRef();
        mDict = NULL;
    }

    if (mDirivedCallList)
    {
        mDirivedCallList->DecRef();
        mDirivedCallList = NULL;
    }

    if (mDirivedCallDict)
    {
        mDirivedCallDict->DecRef();
        mDirivedCallDict = NULL;
    }

    mType = PyTypeDeleted;
}

uint8 PyClass::gettype()
{
    return mType;
}

bool PyClass::setname( PyString* name )
{
    // for now we don't do class name object swapping and stuff
    ASCENT_ASSERT(mName == NULL);

    // object must exist
    if (name == NULL)
        return false;

    // name must be a string object
    if (name->gettype() != PyTypeString)
        return false;

    // must not contain null bytes
    if (name->length() == 0)
        return false;

    mName = name;
    return true;
}

bool PyClass::setbases( PyTuple* tuple )
{
    ASCENT_ASSERT(mBases == NULL);

    // object should exist
    if (tuple == NULL)
        return false;

    mBases = tuple;
    return true;
}

bool PyClass::setdict( PyDict* dict )
{
    ASCENT_ASSERT(mDict == NULL);

    // object should exist
    if (dict == NULL)
        return false;

    if (dict->gettype() != PyTypeDict)
        return false;

    mDict = dict;
    return true;
}

bool PyClass::setDirList( PyList * list )
{
    ASCENT_ASSERT(mDirivedCallList == NULL);
    //assert(list->gettype() == PyTypeList);
    if (list == NULL)
        return false;
    mDirivedCallList = list;
    return true;
}

bool PyClass::setDirDict( PyDict * dict )
{
    ASCENT_ASSERT(mDirivedCallDict == NULL);
    if (dict == NULL)
        return false;
    mDirivedCallDict = dict;
    return true;
}

void PyClass::IncRef()
{
    mRefcnt++;
}

void PyClass::DecRef()
{
    mRefcnt--;
    if (mRefcnt <= 0)
        PyDelete(this);
}

PyString* PyClass::getname()
{
    return mName;
}

PyTuple* PyClass::getbases()
{
    return mBases;
}

PyDict* PyClass::getdict()
{
    return mDict;
}

PyList * PyClass::getDirList()
{
    return mDirivedCallList;
}

PyDict * PyClass::getDirDict()
{
    return mDirivedCallDict;
}

uint32 PyClass::hash()
{
    return (this->*mHash)();
}

/**
 * @note this hash is fucked up.
 */
uint32 PyClass::_hash()
{
    uint32 hsh = 0;
    if (mName)
    {
        hsh = mName->hash();
        hsh = hsh << 3;
    }

    if (mBases)
    {
        hsh|= mBases->hash();
        hsh = hsh << (mBases->size() / 3);
    }

    if (mDict)
    {
        hsh|= mDict->hash();
        hsh = hsh << (mDict->size() / 3);
    }

    if (mDirivedCallList)
    {
        hsh|= mDirivedCallList->hash();
        hsh = hsh << (mDirivedCallList->size() / 10);
    }

    if (mDirivedCallDict)
    {
        hsh|= mDirivedCallDict->hash();
        hsh = hsh << (mDirivedCallDict->size() / 10);
    }

    return hsh;
}
