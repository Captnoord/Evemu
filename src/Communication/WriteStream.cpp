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
	Author:		Captnoord
*/

#include "Common.h"
#include "WriteStream.h"

WriteStream::WriteStream() : mWriteIndex(0), mSize(WS_DEFAULT_BUFF_SIZE)
{
	mBuffer = (uint8*)ASCENT_MALLOC(mSize);
	assert(mBuffer);
}

WriteStream::WriteStream( size_t len ) : mWriteIndex(0), mSize(len)
{
	mBuffer = (uint8*)ASCENT_MALLOC(mSize);
	assert(mBuffer);
}

WriteStream::~WriteStream()
{
	SafeFree(mBuffer);
}

bool WriteStream::writeOpcode( uint8 opcode )
{
	reserve(1);
	mBuffer[mWriteIndex++] = opcode;
	return true;
}

bool WriteStream::writeSizeEx( size_t size )
{
	bool resizeBuffSuccess = reserve(1);
	if (size > 0xFE)
	{
		mBuffer[mWriteIndex++] = -1;
		if ((resizeBuffSuccess == true) && (reserve(4) == true))
		{
			*((uint32*)(&mBuffer[mWriteIndex])) = (uint32)size;
			mWriteIndex+=4;
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (resizeBuffSuccess == true)
		{
			mBuffer[mWriteIndex++] = (uint8)size;
			return true;
		}
		else
		{
			return false;
		}
	}
}

size_t WriteStream::tell()
{
	return mWriteIndex;
}

bool WriteStream::seek( int32 offset, int origin )
{
	switch (origin)
	{
	case SEEK_SET:
		{
			// (signed) maybe break the stuff...
			if (offset <= (signed)mSize)
				mWriteIndex = offset;
			else
				return false;
		} return true;
	case SEEK_CUR:
		{
			if (mWriteIndex + offset <= mSize)
				mWriteIndex+=offset;
			else
				return false;
		} return true;
	case SEEK_END:
		{
			if (mSize - offset >= 0)
				mWriteIndex = mSize - offset;
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

bool WriteStream::write( const char* buffer, const size_t len )
{
	if (buffer == NULL || len <= 0)
		return false;

	if (reserve(len) == false)
		return false;
	
	uint8* dest = &mBuffer[mWriteIndex];
	ASCENT_MEMCPY(dest, buffer, len);
	mWriteIndex+=len;
	return true;
}

bool WriteStream::reserve( size_t size )
{
	/* if the current allocated amount of memory is enough */
	if (mSize > mWriteIndex + size)
		return true;

	/* @todo plan nice memory tricks here */
	mSize = mWriteIndex + size;
	mBuffer = (uint8*)ASCENT_REALLOC(mBuffer, mSize);
	if (mBuffer == NULL)
		return false;
	return true;
}

bool WriteStream::insert( uint8* buff, size_t buff_len, size_t index )
{
	/* I am reserveing tired of this shit, so lets do it the slow way */

	size_t tempBuffSize = size() - index;
	uint8* tempBuff = (uint8*)ASCENT_MALLOC(tempBuffSize+1);
	mBuffer = (uint8*)ASCENT_REALLOC(mBuffer, size() + buff_len);
	ASCENT_MEMCPY(tempBuff, &mBuffer[index], tempBuffSize);					// copy the shit into a temp buffer
	ASCENT_MEMCPY(&mBuffer[index + buff_len], tempBuff, tempBuffSize);		// copy the buffer to the new location
	ASCENT_MEMCPY(&mBuffer[index], buff, buff_len);							// copy the new data in place.

	SafeFree(tempBuff);
	
	// save the size
	/*size_t data_size = size();

	// the amount of data that needs to be moved
	size_t delta_size = data_size - index;*/

	/* tricky shit now... damn */
	//reserve(allocatedsize() + buff_len); // make sure we have enough room

	/*uint8* begin = &mBuffer[index];
	uint8* delta = &mBuffer[index + buff_len];
	uint8* end = &mBuffer[data_size + buff_len];

	for (size_t i = 0; i < delta_size; i++)
	{
		*end = *delta;
		end--;
		delta--;
	}*/


	/*uint8* startpoint = &content()[index];

	uint8* endpoint = &content()[index + buff_len];
	uint8* workpoint = &content()[size() + buff_len];

	for (size_t i = 0; i < delta_size; i++)
	{
		*workpoint = *endpoint;
		workpoint--;
		endpoint--;
	}*/

	/*while (endpoint != startpoint)
	{
		*workpoint = *endpoint;
		workpoint--;
		endpoint--;
	}*/

	/* evil shit */
	mWriteIndex+=buff_len;

	return true;
}

uint8* WriteStream::content()
{
	return mBuffer;
}

size_t WriteStream::size()
{
	return mWriteIndex;
}

size_t WriteStream::allocatedsize()
{
	return mSize;
}

