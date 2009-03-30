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

#ifndef _MARSHALWRITESTREAM_H
#define _MARSHALWRITESTREAM_H

#define WS_DEFAULT_BUFF_SIZE 0x100

/**
 * \class WriteStream
 *
 * @brief utility for writing data to a buffer.
 *
 * No long description.
 *
 * @author Captnoord
 * @date February 2009
 */
class WriteStream
{
public:
	/** 
	 * Constructor.
	 */
	WriteStream();

	/** 
	 * Constructor.
	 */
	WriteStream(size_t len);

	/** 
	 * Destructor.
	 */
	~WriteStream();

	bool write(const char* buffer, const size_t len);

	/**
	 * function to write a opcode to the stream.
	 * @note this function is kinda obsolete but I like it.
	 */
	bool writeOpcode(uint8 opcode);

	/**
	 * template function to write a 8 bits chunk to the stream.
	 */
	template<typename T>
	bool write1(T value)
	{
		if (reserve(1) == false) return false;

		uint8* raw = (uint8*)&value;
		mBuffer[mWriteIndex++] = *raw;
		return true;
	}

	/**
	 * template function to write a 16 bits chunk to the stream.
	 * @note we have to be carefully because write2(char) is possible.
	 */
	template<typename T>
	bool write2(T value)
	{
		if (reserve(2) == false) return false;

		uint16* raw = (uint16*)&value;
		*((uint16*)(&mBuffer[mWriteIndex])) = *raw;
		mWriteIndex+=2;
		return true;
	}

	/**
	 * template function to write a 32 bits chunk to the stream.
	 * @note we have to be carefully because write4(char) is possible.
	 */
	template<typename T>
	bool write4(T value)
	{
		if (reserve(4) == false) return false;

		uint32* raw = (uint32*)&value;
		*((uint32*)(&mBuffer[mWriteIndex])) = *raw;
		mWriteIndex+=4;
		return true;
	}

	/**
	 * function to write a size_t to the stream, its a bit hacky to solve a 
	 * compiler warning this way. But because the size of size_t depends on
	 * the platform ( x86 / x64 ) I am forced to keep this function here.
	 */
	bool write4(size_t value)
	{
		if (reserve(4) == false) return false;

		uint32* raw = (uint32*)&value;
		*((uint32*)(&mBuffer[mWriteIndex])) = *raw;
		mWriteIndex+=4;
		return true;
	}

	/**
	 * template function to write a 64 bits chunk to the stream.
	 * @note we have to be carefully because write8(char) is possible.
	 */
	template<typename T>
	bool write8(T & value)
	{
		if (reserve(8) == false) return false;

		uint64* raw = (uint64*)&value;
		*((uint64*)(&mBuffer[mWriteIndex])) = *raw;
		mWriteIndex+=8;
		return true;
	}

	/**
	 * writes a SizeEx to the buffer.
	 */
	bool writeSizeEx(size_t size);

	/**
	 * returns the write index.
	 */
	size_t size();

	/**
	 * returns the amount of allocated memory.
	 */
	size_t allocatedsize();

	/**
	 * returns the current write index.
	 */
	size_t tell();

	/**
	 * allows to change the write index in a similar way as fseek.
	 */
	bool seek(int32 offset, int origin);

	/**
	 * returns the buffer pointer.
	 */
	uint8* content();

	/**
	 * inserts data into a already existing data stream.
	 * @todo optimize this function, the hack implementation uses a extra temp buffer which can be removed.
	 */
	bool insert(uint8* buff, size_t buff_len, size_t index);

	/**
	 * reserve a amount of mem on top of the current buffer size.
	 */
	bool reserve(size_t size);
private:
	/** write index */
	size_t mWriteIndex;

	/** allocated buffer size */
	size_t mSize;

	/** buffer pointer */
	uint8* mBuffer;
};

#endif //_MARSHALWRITESTREAM_H