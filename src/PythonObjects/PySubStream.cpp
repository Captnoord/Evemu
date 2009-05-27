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
#include "PySubStream.h"

/************************************************************************/
/* PySubStream                                                          */
/************************************************************************/
PySubStream::PySubStream() : mType(PyTypeSubStream), mRefcnt(1), mHash(&PySubStream::_hash),
                             mLen(0), mData(NULL)
{
}

PySubStream::PySubStream( uint8* data, size_t len ) : mType(PyTypeSubStream), mRefcnt(1), mHash(&PySubStream::_hash),
                                                      mLen(len), mData(NULL)
{
    if (data == NULL || mLen == 0)
        return;

    mData = ASCENT_MALLOC(mLen+1);
    ASCENT_MEMCPY(mData, data, mLen);
}

PySubStream::~PySubStream()
{
    SafeFree(mData);
}

uint8 PySubStream::gettype()
{
    return mType;
}

uint8* PySubStream::content()
{
    return (uint8*)mData;
}

bool PySubStream::set( uint8 * data, size_t len )
{
    if (mData != NULL)
        mData = ASCENT_REALLOC(mData, len+1);
    else
        mData = ASCENT_MALLOC(len+1);

    if (mData == NULL)
    {
        mLen = 0;
        return false;
    }

    mLen = len;

    ASCENT_MEMCPY(mData, data, mLen);
    return true;
}

void PySubStream::IncRef()
{
    mRefcnt++;
}

void PySubStream::DecRef()
{
    mRefcnt--;
    if (mRefcnt <= 0)
        PyDelete(this);
}

size_t PySubStream::size()
{
    return mLen;
}

uint32 PySubStream::hash()
{
    return (this->*mHash)();
}

uint32 PySubStream::_hash()
{
    return Utils::Hash::sdbm_hash((char*)mData, (int)mLen);
}
