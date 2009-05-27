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
#include "PySubStruct.h"

/************************************************************************/
/* substruct                                                            */
/************************************************************************/
PySubStruct::PySubStruct() : mType(PyTypeSubStruct), payload(NULL), mRefcnt(1)
{
    mHash = &PySubStruct::_hash;
}

PySubStruct::~PySubStruct()
{
    if (payload)
        payload->DecRef();
}

uint8 PySubStruct::gettype()
{
    return mType;
}

void PySubStruct::IncRef()
{
    mRefcnt++;
}

void PySubStruct::DecRef()
{
    mRefcnt--;
    if (mRefcnt <= 0)
        PyDelete(this);
}

PyObject * PySubStruct::getPyObject()
{
    return payload;
}

bool PySubStruct::setPyObject( PyObject* obj )
{
    if (obj == NULL)
        return false;

    payload = obj;
    obj->IncRef();

    return true;
}

// generic caller
uint32 PySubStruct::hash()
{
    return (this->*mHash)();
}

// class related hash function
uint32 PySubStruct::_hash()
{
    ASCENT_HARDWARE_BREAKPOINT;
}
