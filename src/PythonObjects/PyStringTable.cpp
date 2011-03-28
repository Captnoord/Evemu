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

#include "PyStringTable.h"

//createFileSingleton(PyMarshalStringTable);
initialiseSingleton(PyMarshalStringTable);

PyMarshalStringTable::PyMarshalStringTable()
{
	for (size_t i = 0; i < StringTableSize; i++)
	{
		uint32 hashValue = hash(StringTableData[i]);
		mStringTable[hashValue] = static_cast<uint8>(i);
		mPyStringTable[i].set(StringTableData[i], strlen(StringTableData[i]));
	}
}

PyMarshalStringTable::~PyMarshalStringTable()
{
	/*for (size_t i = 0; i < StringTableSize; i++)
	{
		//mPyStringTable[i].set(StringTableData[i], strlen(StringTableData[i]));
		//if (mPyStringTable[i].)

		//uint8* tst = (uint8*)&mPyStringTable[i];
		//size_t * refcount = (size_t*)(tst+1);
		//printf("str[%u] %u\n", i, *refcount);
	}
	printf("string table delete \n");*/
}

/* lookup a index using a string */
size_t PyMarshalStringTable::LookupIndex(std::string &str)
{
	mLock.Acquire();
	uint32 hashValue = hash(str);
	StringTableMapConstItr Itr = mStringTable.find(hashValue);
	if (Itr != mStringTable.end())
	{
		mLock.Release();
		return Itr->second + 1;
	}
	mLock.Release();
	return -1;
}

/* lookup a index using a string */
size_t PyMarshalStringTable::LookupIndex(const char* str)
{
	// I am lazy... so I do it this way
	std::string _str(str);
	return LookupIndex(_str);
}

bool PyMarshalStringTable::LookupString(uint8 index, std::string &str)
{
	mLock.Acquire();
	if (index > StringTableSize)
	{
		str = "";
		mLock.Release();
		return false;
	}

	str = StringTableData[index-1];
	mLock.Release();
	return true;
}

bool PyMarshalStringTable::LookupPyString( uint8 index, PyString *&str )
{
	mLock.Acquire();
	if (index > StringTableSize)
	{
		ASCENT_HARDWARE_BREAKPOINT;
		*str = ""; //this is a crasher...
		mLock.Release();
		return false;
	}

	assert(index > 0);
	str = &mPyStringTable[index-1];
	str->IncRef();
	mLock.Release();
	return true;
}
