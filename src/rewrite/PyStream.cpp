/*
	------------------------------------------------------------------------------------
	LICENSE:
	------------------------------------------------------------------------------------
	This file is part of EVEmu: EVE Online Server Emulator
	Copyright 2006 - 2008 The EVEmu Team
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

#include "EvemuPCH.h"

/************************************************************************/
/* PyStream class                                                       */
/************************************************************************/
PyStream::PyStream(size_t size) : mWriteIndex(0) { mData.resize(size); }
PyStream::~PyStream() {}

PyStream &PyStream::operator<<(bool value)
{
	if (value == true)
		_pushOpcode(Op_PyTrue);
	else
		_pushOpcode(Op_PyFalse);
	OnObjectWrite();
	return *this;
}

PyStream &PyStream::operator<<(const double& value)
{
	if (value == 0.0)
	{
		_pushOpcode(Op_PyZeroReal);
	}
	else
	{
		_pushOpcode(Op_PyReal);
		_push8(value);
	}
	OnObjectWrite();
	return *this;
}

PyStream &PyStream::operator<<(const uint8& value)
{
	WriteInteger(value);
	OnObjectWrite();
	return *this;
}

PyStream &PyStream::operator<<(const uint32& value)
{
	WriteInteger(value);
	OnObjectWrite();
	return *this;
}

/*PyStream &PyStream::operator<<(uint64& value)
{
	WriteInteger(value);
	OnObjectWrite();
	return *this;
}*/	

PyStream &PyStream::operator<<(const char* const str)
{
	size_t len = strlen(str);
	WriteString(str, len);
	OnObjectWrite();
	return *this;
}

PyStream &PyStream::operator<<(std::string& str)
{
	WriteString(str.c_str(), str.size());
	OnObjectWrite();
	return *this;
}

template <typename T>
void PyStream::_push8(T & value)
{
	_validateBufferSize(8);
	mData[mWriteIndex++] = ((const_uc*)&value)[0];
	mData[mWriteIndex++] = ((const_uc*)&value)[1];
	mData[mWriteIndex++] = ((const_uc*)&value)[2];
	mData[mWriteIndex++] = ((const_uc*)&value)[3];
	mData[mWriteIndex++] = ((const_uc*)&value)[4];
	mData[mWriteIndex++] = ((const_uc*)&value)[5];
	mData[mWriteIndex++] = ((const_uc*)&value)[6];
	mData[mWriteIndex++] = ((const_uc*)&value)[7];
}


template <typename T>
void PyStream::_push4(T value)
{
	_validateBufferSize(4);
	mData[mWriteIndex++] = ((const_uc*)&value)[0];
	mData[mWriteIndex++] = ((const_uc*)&value)[1];
	mData[mWriteIndex++] = ((const_uc*)&value)[2];
	mData[mWriteIndex++] = ((const_uc*)&value)[3];
}

template <typename T>
void PyStream::_push2(T value)
{
	_validateBufferSize(2);
	mData[mWriteIndex++] = ((const_uc*)&value)[0];
	mData[mWriteIndex++] = ((const_uc*)&value)[1];
}

template <typename T>
void PyStream::_push1(T value)
{
	_validateBufferSize(1);
	mData[mWriteIndex++] = ((const_uc*)&value)[0];
}

template <typename T>
void PyStream::WriteInteger(T & value)
{
	if (value == 0)
	{
		_pushOpcode(Op_PyZeroInteger);
	}
	else if(value == 1)
	{
		_pushOpcode(Op_PyOneInteger);
	}
	else if ( value + 0x80u > 0xFF )
	{
		if ( value + 0x8000u > 0xFFFF )
		{
			if (value + 0x800000u > 0xFFFFFFFF)
			{
				assert(false); // don't support very large integers
				//_PyRepInteger_AsByteArray(rep);
			}
			else
			{
				_pushOpcode(Op_PyLong);
				_push4(value);
			}				
		}
		else
		{
			_pushOpcode(Op_PySignedShort);
			_push2(value);
		}
	}
	else
	{
		_pushOpcode(Op_PyByte);
		_push1(value);
	}
}

void PyStream::WriteString(const char* str, size_t len)
{
	if (len == 0)
	{
		_pushOpcode(Op_PyEmptyString);
		return;
	}
	else if (len == 1)
	{
		_pushOpcode(Op_PyCharString);
		_push1(str[0]);
		return;
	}
	else
	{
		size_t stringID = sPyStringTable.LookupIndex(str);
		if (stringID != STRING_TABLE_ERROR)
		{
			_pushOpcode(Op_PyStringTableItem);
			_push1(stringID);
			return;
		}
		else
		{
			if (len > 0xFF)
			{
				_pushOpcode(Op_PyBuffer);
				_push4(len);
				_pushString(str);
				return;
			}
			else
			{
				_pushOpcode(Op_PyShortString);
				_push1(len);
				_pushString(str);
				return;
			}
		}
	}
}

void PyStream::_pushString(const std::string & string)
{
	_validateBufferSize(string.size());
	// check size before this one
	memcpy(&mData[mWriteIndex], string.c_str(), string.size());
	mWriteIndex+=string.size();
}

void PyStream::_pushOpcode(PyRepOpcodes opcode)
{
	_validateBufferSize(1);
	mData[mWriteIndex++] = opcode;
}

void PyStream::_validateBufferSize(size_t size)
{
	if (mWriteIndex + size > mData.size())
	{
		//OnBufferResize();
		mData.resize(mWriteIndex + size);
	}
}


/************************************************************************/
/* PyTupleStream class                                                  */
/************************************************************************/
PyTupleStream::PyTupleStream(size_t size) : PyStream(size + 6)
{
	/*  */
	_push1('~');
	_push4(0);
	_pushOpcode(Op_PyTuple);	// python object opcode
	_push1(0);					// count placeholder.
}

void PyTupleStream::OnObjectWrite()
{
	(content()[6])++;
}
