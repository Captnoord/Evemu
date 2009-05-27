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
#include "PyList.h"

/************************************************************************/
/* PyList                                                               */
/************************************************************************/
PyList::PyList() : mType(PyTypeList), mRefcnt(1)
{
    mHash = &PyList::_hash;
}

PyList::PyList( int elementCount ) : mType(PyTypeList), mRefcnt(1)
{
    mHash = &PyList::_hash;
    mList.resize(elementCount);
}

PyList::~PyList()
{
    iterator Itr = mList.begin();
    for (; Itr != mList.end(); Itr++)
    {
        delete *Itr;
    }

    mList.clear();
    mType = PyTypeDeleted;
}

PyChameleon & PyList::operator[]( const int index )
{
    if (index < 0)
        return PyErrorIterator;

    if (index > (int)mList.size())
        mList.resize(index);

    if (mList[index] == NULL)
        mList[index] = new PyChameleon();


    return *mList[index];
}

uint8 PyList::gettype()
{
    return mType;
}

void PyList::IncRef()
{
    mRefcnt++;
}

void PyList::DecRef()
{
    mRefcnt--;
    if (mRefcnt <= 0)
        PyDelete(this);
}

size_t PyList::size()
{
    return mList.size();
}

bool PyList::add( PyObject* obj )
{
    if (obj == NULL || obj->gettype() == PyTypeDeleted)
        return false;

    PyChameleon * itr = new PyChameleon();
    itr->setPyObject(obj);
    mList.push_back(itr);

    return true;
}

uint32 PyList::hash()
{
    return (this->*mHash)();
}

uint32 PyList::_hash()
{
    uint32 hsh = 0;
    PyList::iterator itr = mList.begin();
    for (; itr != mList.end(); itr++)
    {
        PyObject* obj = (*itr)->getPyObject();
        assert(obj);
        hsh|= PyObject_Hash(obj);
        hsh = hsh << 3;
    }
    return hsh;
}
