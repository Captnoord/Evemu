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
 * \class MarshalReadStream
 *
 * @brief internal class for reading action in MarshalStream.
 *
 * 
 *
 * @author Captnoord
 * @date February 2009
 * @todo add compiletime size checks (if possible) for the templates..... so that you can't read a uint64 into a uint8
 */
class MarshalReadStream
{
public:
	MarshalReadStream();
	MarshalReadStream(const char* buff, size_t size);
	~MarshalReadStream();

	template<typename T>
	bool peekInt1(T & dst)
	{
		if (mSize < mReadIndex + 1)
			return false;
		((uint8*)&dst)[0] = mBuffer[mReadIndex];
		return true;
	}

	template<typename T>
	bool readInt1(T & dst)
	{
		if (mSize < mReadIndex + 1)
			return false;
		((uint8*)&dst)[0] = mBuffer[mReadIndex++];
		return true;
	}

	template<typename T>
	bool readInt2(T & dst)
	{
		if (mSize < mReadIndex + 2)
			return false;
		((uint8*)&dst)[0] = mBuffer[mReadIndex++];
		((uint8*)&dst)[1] = mBuffer[mReadIndex++];
		return true;
	}

	template<typename T>
	bool readInt4(T & dst)
	{
		if (mSize < mReadIndex + 4)
			return false;
		((uint8*)&dst)[0] = mBuffer[mReadIndex++];
		((uint8*)&dst)[1] = mBuffer[mReadIndex++];
		((uint8*)&dst)[2] = mBuffer[mReadIndex++];
		((uint8*)&dst)[3] = mBuffer[mReadIndex++];
		return true;
	}

	template<typename T>
	bool readInt8(T & dst)
	{
		if (mSize < mReadIndex + 8)
			return false;
		((uint8*)&dst)[0] = mBuffer[mReadIndex++];
		((uint8*)&dst)[1] = mBuffer[mReadIndex++];
		((uint8*)&dst)[2] = mBuffer[mReadIndex++];
		((uint8*)&dst)[3] = mBuffer[mReadIndex++];
		((uint8*)&dst)[4] = mBuffer[mReadIndex++];
		((uint8*)&dst)[5] = mBuffer[mReadIndex++];
		((uint8*)&dst)[6] = mBuffer[mReadIndex++];
		((uint8*)&dst)[7] = mBuffer[mReadIndex++];
		return true;
	}

	bool readBuffer(uint8 ** buffer, size_t len);
	bool readString(char ** buffer, size_t len);
	bool readWstring(wchar_t ** buffer, size_t len);
	bool readExSize(size_t & size);

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
	 * @brief reset the readindex.
	 *
	 * 
	 *
	 */
	void reset();
	bool set( uint8* buff, size_t size );

	size_t size();
	size_t tell() {return mReadIndex;}
	bool seek(int32 offset, int origin);
	uint8* content() {return mBuffer;}

private:
	uint8* mBuffer;
	size_t mSize;
	size_t mReadIndex;
	size_t mAllocatedMem;
};

#endif //_MARSHALREADSTREAM_H