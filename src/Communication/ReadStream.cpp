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

#include "Common.h"
#include "DebugTools.h"
#include "Log.h"
#include "NGLog.h"
#include "ReadStream.h"

#include "FastQueue.h"
#include "BufferPool.h"

ReadStream::ReadStream() : mBuffer(NULL), mSize(0), mReadIndex(0),  mAllocatedMem(0), mPayloadSize(0) {}

ReadStream::ReadStream( size_t size ) : mBuffer(NULL), mSize(size), mReadIndex(0), mAllocatedMem(0), mPayloadSize(size)
{
    resize(size);
}

ReadStream::ReadStream( const char* buff, size_t size ) : mBuffer(NULL), mSize(size), mReadIndex(0), mAllocatedMem(0), mPayloadSize(size)
{
    resize(size);

    ASCENT_MEMCPY(mBuffer, buff, size);
}

ReadStream::~ReadStream()
{
    SafeFree(mBuffer);
}

size_t ReadStream::size()
{
    return mPayloadSize;
}

bool ReadStream::resize( size_t newsize )
{
    // check if we have enough memory allocated.
    if( mAllocatedMem > newsize && mBuffer != NULL )
    {
        mSize = newsize;
        mPayloadSize = newsize;
        return true;
    }

    // we seem to need to allocate or reallocate memory...
    if (mBuffer != NULL)
        mBuffer = (uint8*)ASCENT_REALLOC(mBuffer, newsize);
    else
        mBuffer = (uint8*)ASCENT_MALLOC(newsize);

    // check if the memory is allocated
    if (mBuffer == NULL)
    {
        mAllocatedMem = 0;
        return false;
    }

    mSize = newsize;
    mPayloadSize = newsize;
    mAllocatedMem = newsize;
    return true;
}

bool ReadStream::readBuffer( uint8 ** buffer, size_t len )
{
    if (mSize < mReadIndex + len)
        return false;

    *buffer = &mBuffer[mReadIndex];
    mReadIndex+=len;
    return true;
}

bool ReadStream::readString( char ** buffer, size_t len )
{
    if (mSize < mReadIndex + len)
        return false;

    *buffer = (char*)&mBuffer[mReadIndex];
    mReadIndex+=len;
    return true;
}

bool ReadStream::readWstring( wchar_t ** buffer, size_t len )
{
    if (mSize < mReadIndex + len*2)
        return false;

    *buffer = (wchar_t*)&mBuffer[mReadIndex];
    mReadIndex+=len*2;
    return true;
}



void ReadStream::reset()
{
    mReadIndex = 0;
}

bool ReadStream::set( uint8* buff, size_t size )
{
    if (buff == NULL || size <= 0)
        return false;

    resize(size);
    ASCENT_MEMCPY(mBuffer, buff, size);
    reset();
    return true;
}

bool ReadStream::seek( int32 offset, int origin )
{
    switch (origin)
    {
    case SEEK_SET:
        {
            // (signed) maybe break the stuff...
            if (offset <= (signed)mSize)
                mReadIndex = offset;
            else
                return false;
        } return true;
    case SEEK_CUR:
        {
            if (mReadIndex + offset <= mSize)
                mReadIndex+=offset;
            else
                return false;
        } return true;
    case SEEK_END:
        {
            if (mSize - offset >= 0)
                mReadIndex = mSize - offset;
            else
                return false;
        } return true;
    default:
        {
            assert(false);
        } return false;
    }
    return false;
}

bool ReadStream::readSizeEx( size_t & size )
{
    size_t _size = 0;
    if (!read1(_size))
        return false;

    if (_size == 0xFF)
    {
        if(!read4(_size))
            return false;
    }
    size = _size;
    return true;
}

size_t ReadStream::tell()
{
    return mReadIndex;
}

uint8* ReadStream::content()
{
    return mBuffer;
}

size_t ReadStream::buffersize()
{
    return mSize;
}

bool ReadStream::setpayloadsize( size_t size )
{
    if (size > mSize)
        return false;

    mPayloadSize = size;
    return true;
}
