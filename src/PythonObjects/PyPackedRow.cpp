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
#include "PyPackedRow.h"

/************************************************************************/
/* PyPackedRow                                                          */
/************************************************************************/
PyPackedRow::PyPackedRow() : mType(PyTypePackedRow), mHeader(NULL), mRawFieldData(NULL), mRawFieldDataLen(0), mRefcnt(1)
{
    mHash = &PyPackedRow::_hash;
}

PyPackedRow::~PyPackedRow()
{
    iterator Itr = flowers.begin();
    for (; Itr != flowers.end(); Itr++)
    {
        delete Itr->second;
    }

    flowers.clear();

    mHeader->DecRef();

    if (mRawFieldData)
        SafeFree(mRawFieldData);
}

uint8 PyPackedRow::gettype()
{
    return mType;
}

void PyPackedRow::IncRef()
{
    mRefcnt++;
}

void PyPackedRow::DecRef()
{
    mRefcnt--;
    if (mRefcnt <= 0)
        PyDelete(this);
}

bool PyPackedRow::addleaf( PyObject* leaf )
{
    assert(leaf != NULL);

    // fucked up because of some c++ fuckup...
    uint32 hsh = PyObject_Hash(leaf);
    PyChameleon * itr = new PyChameleon();
    itr->setPyObject(leaf);

    flowers.insert(std::make_pair(hsh, itr));

    return true;
}

uint32 PyPackedRow::hash()
{
    return (this->*mHash)();
}

uint32 PyPackedRow::_hash()
{
    uint32 hsh = 0;

    if (mHeader)
        hsh |= PyObject_Hash((PyObject *)mHeader);

    if (rawPayLoad)
    {
        hsh = hsh << 3;
        hsh |= PyObject_Hash((PyObject *)mHeader);
    }

    iterator itr = flowers.begin();
    for (; itr != flowers.end(); itr++)
    {
        hsh = hsh << 3;
        if(itr->second->getPyObject())
            hsh |= PyObject_Hash(itr->second->getPyObject());
    }
    return hsh;
}

bool PyPackedRow::setheader( PyClass * obj )
{
    if (obj->gettype() != PyTypeClass)
        return false;

    mHeader = obj;
    return true;
}

PyPackedRow::iterator PyPackedRow::begin()
{
    return flowers.begin();
}

PyPackedRow::iterator PyPackedRow::end()
{
    return flowers.end();
}

PyClass* PyPackedRow::getheader()
{
    return mHeader;
}

size_t PyPackedRow::size()
{
    return flowers.size();
}

bool PyPackedRow::setRawPayLoad( PyObject* tuple )
{
    rawPayLoad = (PyTuple*)tuple;
    return true;
}

PyObject* PyPackedRow::getRawPayLoad()
{
    return (PyObject*)rawPayLoad;
}

bool PyPackedRow::init( PyObject* header )
{
    size_t argumentCount = 0;
    if (header->gettype() == PyTypeTuple)
    {
        PyTuple& tulple = *(PyTuple*)header;
        argumentCount = tulple.size();
        if (argumentCount == 0)
        {
            Log.Error("PyPackedRow", "init: expected at least one argument");
            return false;
        }
    }
    else
    {
        argumentCount = 1; // I have no clue
    }

    //mHeader = header;

    return true;
}
