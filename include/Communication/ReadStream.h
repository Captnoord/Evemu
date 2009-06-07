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

#ifndef _MARSHALREADSTREAM_H
#define _MARSHALREADSTREAM_H

/**
 * \class ReadStream
 *
 * @brief utility for read data from a buffer.
 *
 * @author Captnoord
 * @date February 2009
 */
class ReadStream
{
public:
	/**
	 * Constructor.
	 */
	ReadStream();

	/**
	* Constructor.
	*/
	ReadStream(size_t size);

	/** 
	 * Constructor.
	 */
	ReadStream(const char* buff, size_t size);

	/** 
	 * destructor.
	 */
	~ReadStream();

	/**
	 * 
	 */
	template<typename T>
	bool peek1(T & dst)
	{
		if (mSize < mReadIndex + 1)
			return false;
		((uint8*)&dst)[0] = mBuffer[mReadIndex];
		return true;
	}

	/**
	 * 
	 */
	template<typename T>
	bool peek2(T & dst)
	{
		if (mSize < mReadIndex + 2)
			return false;
		*((uint16*)&dst) = *((uint16*)&mBuffer[mReadIndex]);
		return true;
	}

	/**
	 * 
	 */
	template<typename T>
	bool peek4(T & dst)
	{
		if (mSize < mReadIndex + 4)
			return false;
		*((uint32*)&dst) = *((uint32*)&mBuffer[mReadIndex]);
		return true;
	}

	/**
	 * template function to read 8 bits chunks from the stream.
	 */
	template<typename T>
	bool read1(T & dst)
	{
		if (mSize < mReadIndex + 1)
			return false;
		((uint8*)&dst)[0] = mBuffer[mReadIndex++];
		return true;
	}

	/**
	 * template function to read 16 bits chunks from the stream.
	 * @note possible heap corruption when doing read2(char)
	 */
	template<typename T>
	bool read2(T & dst)
	{
		if (mSize < mReadIndex + 2)
			return false;

		*((uint16*)&dst) = *((uint16*)&mBuffer[mReadIndex]);
		mReadIndex+=2;

		return true;
	}

	/**
	 * template function to read 32 bits chunks from the stream.
	 * @note possible heap corruption when doing read4(char)
	 */
	template<typename T>
	bool read4(T & dst)
	{
		if (mSize < mReadIndex + 4)
			return false;
		*((uint32*)&dst) = *((uint32*)&mBuffer[mReadIndex]);
		mReadIndex+=4;
		return true;
	}

	/**
	 * template function to read 64 bits chunks from the stream.
	 * @note possible heap corruption when doing read8(char)
	 */
	template<typename T>
	bool read8(T & dst)
	{
		if (mSize < mReadIndex + 8)
			return false;

		*((uint64*)&dst) = *((uint64*)&mBuffer[mReadIndex]);
		mReadIndex+=8;

		return true;
	}

	/**
	 * reads a buffer from the stream.
	 */
	bool readBuffer(uint8 ** buffer, size_t len);

	/**
	 * reads a string from the stream.
	 */
	bool readString(char ** buffer, size_t len);

	/**
	 * reads a wide string from the stream.
	 */
	bool readWstring(wchar_t ** buffer, size_t len);

	/**
	 * reads a SizeEx from the stream.
	 */
	bool readSizeEx(size_t & size);

	/**
	 * @brief resizes the buffer
	 *
	 * The idea is to be able to resize the containing buffer
	 * so we can reuse the object if we want.
	 *
	 * @param[in] newsize the number of bytes needed to be allocated.
	 * @return true is a success and false if not.
	 */
	bool resize(size_t newsize);

	/**
	 * @brief reset the read index.
	 *
	 * resets the buffer read index to 0;
	 *
	 */
	void reset();

	/**
	 * sets the payload of this stream.
	 */
	bool set( uint8* buff, size_t size );

	/**
	 * returns the size of the stream.
	 */
	size_t size();

	/**
	 * returns the current read index.
	 */
	size_t tell();

	/**
	 * sets the current read index in a similar way as fseek works.
	 */
	bool seek(int32 offset, int origin);

	/**
	 * returns the data pointer.
	 */
	uint8* content();

	/**
	 * returns the buffer size of this stream.
	 */
	size_t buffersize();

	/**
	 * returns the payload size of the marshal stream.
	 * @note this is kinda a hack, we need a proper solution for it.
	 */
	bool setpayloadsize(size_t size);

private:
	/**
	 * the data buffer.
	 */
	uint8* mBuffer;

	/**
	 * the size of the data buffer.
	 */
	size_t mSize;

	/**
	 * the read index of the stream.
	 */
	size_t mReadIndex;

	/**
	 * the amount of allocated memory of this stream.
	 */
	size_t mAllocatedMem;

	/**
	 * the size of the marshal payload.
	 */
	size_t mPayloadSize;
};

#endif //_MARSHALREADSTREAM_H

