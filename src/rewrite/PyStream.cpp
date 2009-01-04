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
/* Begin PyStream implementation                                        */
/************************************************************************/
PyStream::PyStream() : mWriteIndex(0) {}
PyStream::PyStream(size_t size) : mWriteIndex(0) { mData.resize(size); }
PyStream::~PyStream() {}

// please don't use this often..
PyStream::PyStream(const PyStream& data)
{
#pragma message(__FILE__""__FUNCTION__": slow copy constructor used, use this one with care young padawa!!\n")

	this->mWriteIndex = data.mWriteIndex;

	mData.resize(data.mData.size());
	memcpy(&this->mData[0], &data.mData[0], data.mData.size());
}

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
	_writeInteger(value);
	OnObjectWrite();
	return *this;
}

PyStream &PyStream::operator<<(const uint32& value)
{
	_writeInteger(value);
	OnObjectWrite();
	return *this;
}

PyStream &PyStream::operator<<(const int value)
{
	_writeInteger(value);
	OnObjectWrite();
	return *this;
}

PyStream &PyStream::operator<<(const char* const str)
{
	size_t len = strlen(str);
	_writeString(str, len);
	OnObjectWrite();
	return *this;
}

PyStream &PyStream::operator<<(std::string& str)
{
	_writeString(str.c_str(), str.size());
	OnObjectWrite();
	return *this;
}

void PyStream::_pushStream(PyStream & stream)
{
	size_t len = stream.size();
	_validateBufferSize(len);

	memcpy(&mData[mWriteIndex], stream.content(), len);
	mWriteIndex+=len;
}

void PyStream::_pushStream(PyStream & stream, size_t len, size_t offset)
{
	assert(stream.size() >= len); // make sure we can read the damn thing
	if (offset != -1)
		assert(stream.size() > offset);

	_validateBufferSize(len);

	if (offset != -1)
		memcpy(&mData[mWriteIndex], &stream.content()[offset], len);
	else
		memcpy(&mData[mWriteIndex], &stream.content()[6], len);

	mWriteIndex+=len;
}

void PyStream::_writeString(const char* str, size_t len)
{
	if (str == NULL)
	{
		// add empty string or PyNone?
		_pushOpcode(Op_PyEmptyString);
		return;
	}
	else if (len == 0)
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
/* End of PyStream implementation                                       */
/************************************************************************/

/************************************************************************/
/* Begin of PyStringStream implementation                               */
/************************************************************************/
PyStringStream::PyStringStream(const char* str) : PyStream(6 + strlen(str))
{
	/*  */
	_push1('~');
	_push4(0);
	_writeString(str, strlen(str));
}

PyStringStream::PyStringStream(std::string & str) : PyStream(6 + str.size())
{
	/*  */
	_push1('~');
	_push4(0);
	_writeString(str.c_str(), str.size());
}
/************************************************************************/
/* End of PyStringStream implementation                                 */
/************************************************************************/

/************************************************************************/
/* Begin PyTupleStream implementation                                   */
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
/************************************************************************/
/* End PyTupleStream implementation                                     */
/************************************************************************/

/************************************************************************/
/* Begin PyNetworkStream implementation                                 */
/************************************************************************/
PyNetworkStream::PyNetworkStream(): mReadIndex(0) {}
PyNetworkStream::PyNetworkStream(const char * data, size_t size) : PyStream(size), mReadIndex(0)
{
	memcpy(&mData[0], data, size);
}

PyNetworkStream::PyNetworkStream(size_t size) : PyStream(size), mReadIndex(5)
{
	// this is a hack...
	mWriteIndex = size;
}

PyStream &PyNetworkStream::operator>>(bool &value) {
	PyRepOpcodes opcode = _popOpcode();
	assert(opcode == Op_PyTrue || opcode == Op_PyFalse); // hehe...
	if (opcode == Op_PyTrue)
		value = true;
	else
		value = false;
	return *this;
}

PyStream &PyNetworkStream::operator>>(uint8 &value) {
	value = _readInteger<uint8>();
	return *this;
}

PyStream &PyNetworkStream::operator>>(uint16 &value) {
	value = _readInteger<uint16>();
	return *this;
}

PyStream &PyNetworkStream::operator>>(int &value) {
	value = _readInteger<int>();
	return *this;
}

PyStream &PyNetworkStream::operator>>(uint32 &value) {
	value = _readInteger<uint32>();
	return *this;
}

PyStream &PyNetworkStream::operator>>(uint64 &value) {
	value = _readInteger<uint64>();
	return *this;
}

PyStream &PyNetworkStream::operator>>(double &value) {
	PyRepOpcodes opcode = _popOpcode();
	assert(opcode == Op_PyReal || opcode == Op_PyZeroReal);
	value = _pop8<double>();
	return *this;
}

PyStream &PyNetworkStream::operator>>(std::wstring &value) {
	PyRepOpcodes opcode = _popOpcode();
	switch (opcode)
	{
		// mainly here for compatibility reasons.
	case Op_PyNone:
		{
			value.clear();
		} break;

	case Op_PyUnicodeString:
		{
			size_t size = (size_t)_pop1<uint32>();
			if (size == 0xFF)
				size = (size_t)_pop4<uint32>();
			_readString(value, size);
		} break;

	case Op_PyEmptyUnicodeString:
		{
			value.clear();
		} break;

	case Op_PyUnicodeCharString:
		{
			value  = _pop2<wchar_t>();
		} break;

	case Op_PyUnicodeByteString:
		{
			size_t size = (size_t)_pop1<uint32>();
			_readString(value, size);
		} break;

		// todo implement this one...
		/*case Op_PyStringTableItem:
		{
		uint8 stringIndex = _pop1<uint8>();

		sPyStringTable.LookupString(stringIndex, value);
		assert(value != "");
		} break;*/
	}

	return *this;
}

PyStream &PyNetworkStream::operator>>(std::string &value) {
	PyRepOpcodes opcode = _popOpcode();

	switch (opcode)
	{
		// mainly here for compatibility reasons.
	case Op_PyNone:
		{
			value.clear();
		} break;

	case Op_PyBuffer:
		{
			size_t size = (size_t)_pop1<uint32>();
			if (size == 0xFF)
				size = (size_t)_pop4<uint32>();
			_readString(value, size);
		} break;

	case Op_PyEmptyString:
		{
			value.clear();
		} break;

	case Op_PyCharString:
		{
			value = _pop1<char>();
		} break;

	case Op_PyShortString:
		{
			size_t size = (size_t)_pop1<uint32>();
			_readString(value, size);
		} break;

	case Op_PyStringTableItem:
		{
			uint8 stringIndex = _pop1<uint8>();

			sPyStringTable.LookupString(stringIndex, value);
			assert(value != "");
		} break;
	}
	return *this;
}

PyRepOpcodes PyNetworkStream::_popOpcode() {
	uint8 opcode = mData[mReadIndex++];
	return static_cast<PyRepOpcodes>(opcode);
}

// a method to seek tough a stream in c style
void PyNetworkStream::_seek(size_t offset, streamSeek rule)
{
	switch (rule)
	{
		case STREAM_SEEK_SET:
		{
			if (offset < size())
				mReadIndex = offset;
			else
				mReadIndex = -1;
		} break;
		
		case STREAM_SEEK_CUR:
		{
			if (mReadIndex + offset < size())
				mReadIndex+=offset;
			else
				mReadIndex = -1;
		} break;
		
		case STREAM_SEEK_END:
		{
			if (size() - offset >= 0)
				mReadIndex = size() - offset;
			else
				mReadIndex = -1;
		} break;
		
		default:
		{
			assert(false);
		} break;
	}
}

// get the current offset
size_t PyNetworkStream::_tell()
{
	return mReadIndex;
}

void PyNetworkStream::_readString(std::string & str, size_t len)
{
	str.clear();
	str.resize(len);
	memcpy(&str[0], &mData[mReadIndex], len);
	mReadIndex+=len;
	str[len] = '\0';
}

void PyNetworkStream::_readString(std::wstring & str, size_t len)
{
	str.clear();
	str.resize(len);

	size_t parsedLen = mbstowcs(&str[0], (const char*)&mData[mReadIndex], len);
	assert(parsedLen == len);

	mReadIndex+=len;
}
/************************************************************************/
/* End PyNetworkStream implementation                                   */
/************************************************************************/