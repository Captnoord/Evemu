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
#pragma message(__FILE__"("__STR1__(__LINE__)")"__FUNCTION__" : slow copy constructor used, use this one with care young padawa!!\n")

	this->mWriteIndex = data.mWriteIndex;

	if (data.mWriteIndex == 0)
		return;

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
		_pushOpcode(Op_PyZeroFloat);
	}
	else
	{
		_pushOpcode(Op_PyFloat);
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

PyStream &PyStream::operator<<(std::wstring& str)
{
	_writeWString(str.c_str(), str.size());
	OnObjectWrite();
	return *this;
}

PyStream &PyStream::operator<<(PyNoneBase obj)
{
	_pushOpcode(Op_PyNone);
	OnObjectWrite();
	return *this;
}

PyStream &PyStream::operator<<(PyBufferStream& obj)
{
	//_pushOpcode(Op_PyBuffer);
	_pushStream(obj);
	OnObjectWrite();
	return *this;
}

PyStream &PyStream::operator<<(PyTupleStream& obj)
{
	//_pushOpcode(Op_PyBuffer);
	_pushStream(obj);
	OnObjectWrite();
	return *this;
}

PyStream &PyStream::operator<<(PyDictStream& obj)
{
	_pushStream(obj);
	OnObjectWrite();
	return *this;
}

PyStream &PyStream::operator<<(PyListStream& obj)
{
	_pushStream(obj);
	OnObjectWrite();
	return *this;
}

PyStream &PyStream::operator<<(PyStringStream& obj)
{
	_pushStream(obj);
	OnObjectWrite();
	return *this;
}

void PyStream::_pushStream(PyStream & stream)
{
	size_t len = stream.size();
	if (len == 0)
		return;

	_validateBufferSize(len);

	memcpy(&mData[mWriteIndex], stream.content(), len);
	mWriteIndex+=len;
}

void PyStream::_pushStream(PyStream & stream, size_t len, size_t offset)
{
	if (len == 0)
		return;

	assert(stream.size() >= len); // make sure we can read the damn thing
	
	// offset -1 is the default
	if (offset != -1)
		assert(stream.size() > offset);

	_validateBufferSize(len);

	if (offset != -1)
		memcpy(&mData[mWriteIndex], &stream.content()[offset], len);
	else
		memcpy(&mData[mWriteIndex], &stream.content()[6], len);

	mWriteIndex+=len;
}

void PyStream::resize(size_t len)
{
	// don't resize to stupid sizes.
	if (len == -1)
		return;

	mData.resize(len);
}

void PyStream::_writeString(const char* str, size_t len, bool noLookup)
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
		size_t stringID = -1;
		if (noLookup == false)
			stringID = sPyStringTable.LookupIndex(str);

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

void PyStream::_writeWString(const wchar_t* str, size_t len, bool noLookup)
{
	if (str == NULL)
	{
		// add empty string or PyNone?
		_pushOpcode(Op_PyEmptyUnicodeString);
		return;
	}
	else if (len == 0)
	{
		_pushOpcode(Op_PyEmptyUnicodeString);
		return;
	}
	else if (len == 1)
	{
		_pushOpcode(Op_PyUnicodeCharString);
		_push2(str[0]);
		return;
	}
	else
	{
		if (len > 0xFF)
		{
			_pushOpcode(Op_PyUnicodeString);
			_push4(len);
			_pushWString(str);
			return;
		}
		else
		{
			_pushOpcode(Op_PyUnicodeByteString);
			_push1(len);
			_pushWString(str);
			return;
		}
	}
}

void PyStream::_writeString(const std::string & str, bool noLookup)
{
	_writeString(str.c_str(), str.size(), noLookup);
}

void PyStream::_writeWString(const std::wstring & str, bool noLookup)
{
	_writeWString(str.c_str(), str.size(), noLookup);
}

void PyStream::_pushString(const std::string & string)
{
	if (string.size() == 0)
		return;

	_validateBufferSize(string.size());
	// check size before this one
	memcpy(&mData[mWriteIndex], string.c_str(), string.size());
	mWriteIndex+=string.size();
}

void PyStream::_pushWString(const std::wstring & string)
{
	if (string.size() == 0)
		return;

	_validateBufferSize(string.size()*2);
	// check size before this one
	memcpy(&mData[mWriteIndex], string.c_str(), string.size()*2);
	mWriteIndex+=string.size()*2;
}

void PyStream::_pushOpcode(PyRepOpcodes opcode)
{
	_validateBufferSize(1);
	mData[mWriteIndex++] = opcode;
}

void PyStream::_pushExSize(uint32 size)
{
	if (size < 0xFF)
	{
		_validateBufferSize(1);
		mData[mWriteIndex++] = (uint8)size;
	}
	else
	{
		_validateBufferSize(5);
		mData[mWriteIndex++] = 0xFF;
		memcpy(&mData[mWriteIndex], (void*)&size, 4);
		mWriteIndex+=4;
	}
}

// don't check len here because I want to be able to send empty buffers.
void PyStream::_pushBuffer(const uint8 * buffer, size_t len)
{
	_validateBufferSize(len);
	// check size before this one
	if (buffer != NULL)
		memcpy(&mData[mWriteIndex], buffer, len);

	mWriteIndex+=len;
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
PyStringStream::PyStringStream(const char* str) : PyStream(strlen(str))
{
	_writeString(str, strlen(str));
}

PyStringStream::PyStringStream(const char* str, size_t len) : PyStream(len)
{
	_writeString(str, len);
}

PyStringStream::PyStringStream(std::string & str) : PyStream(str.size())
{
	_writeString(str.c_str(), str.size());
}
/************************************************************************/
/* End of PyStringStream implementation                                 */
/************************************************************************/

/************************************************************************/
/* Begin PyDictStream implementation                                    */
/************************************************************************/
PyDictStream::PyDictStream() : PyStream(2)
{
	_pushOpcode(Op_PyDict);
	_push1(0);
}

PyStream &PyDictStream::operator[](const char* str)
{
	// this piece of code kinda assumes that they key name can already be present.
	// that in fact is almost never... mainly here for security reasons.
	std::string keyName(str);
	DictMapItr Itr = m_Dict.find(keyName);
	if (Itr != m_Dict.end())
	{
		return Itr->second;
	}
	else
	{
		m_Dict.insert(std::make_pair(keyName, PyStream()));
		return m_Dict.find(keyName)->second;
	}
}

void PyDictStream::finish()
{
	uint32 count = (uint32)m_Dict.size();

	// small hack
	mWriteIndex--;

	if (count < 0xFF)
	{
		_push1(count);
	}
	else
	{
		_push1(0xFF);
		_push4(count);
	}

	if (count > 0)
	{
		/* in order to make this function a bit faster as its slow as hell
		we get the length of all the elements and key names and validate the buffer.
		this should make us increase the buffer size only 1x.
		*/
		size_t xsize = 0;

		DictMapItr Itr = m_Dict.begin();
		for (; Itr != m_Dict.end(); Itr++)
		{
			xsize += Itr->first.size();
			xsize += Itr->second.size();
		}
		_validateBufferSize(xsize);

		Itr = m_Dict.begin();
		for (; Itr != m_Dict.end(); Itr++)
		{
			_pushStream(Itr->second);
			_writeString(Itr->first, true);
		}
	}
}

/************************************************************************/
/* End PyDictStream implementation                                      */
/************************************************************************/

/************************************************************************/
/* Begin PyTupleStream implementation                                   */
/************************************************************************/
PyTupleStream::PyTupleStream() : PyStream(),mElementCount(0), mSizeInjected(false)
{
	_pushOpcode(Op_PyEmptyTuple);	// python object opcode
}

PyTupleStream::PyTupleStream(size_t size) : PyStream(size), mElementCount(0), mSizeInjected(false)
{
	_pushOpcode(Op_PyEmptyTuple);	// python object opcode
}

/* even that this isn't as hacky as the previouse implementation there is a lot to optimise. */
void PyTupleStream::OnObjectWrite(size_t /*size*/)
{
	mElementCount++;
	if (mElementCount == 1)
	{
		mData[0] = Op_PyOneTuple;
	}
	else if (mElementCount == 2)
	{
		mData[0] = Op_PyTwoTuple;
	}
	else if (mSizeInjected == false)
	{
		mData[0] = Op_PyTuple;
		DataItr Itr = mData.begin();
		Itr++;
		mData.insert(Itr, 1, 0); // would be slow when a lot of data is in the stream....
		mWriteIndex++;

		mData[1] = mElementCount;

		mSizeInjected = true;
	}
	else
	{
		mData[1] = mElementCount;
	}
}
/************************************************************************/
/* End PyTupleStream implementation                                     */
/************************************************************************/

/************************************************************************/
/* Begin PyReadStream implementation								    */
/************************************************************************/
PyReadStream::PyReadStream() : PyStream(0), mReadIndex(0) {}
PyReadStream::PyReadStream(const char * data, size_t size) : PyStream(size), mReadIndex(0)
{
	if (size == 0)
		return;

	if (data == NULL)
		return;

	memcpy(&mData[0], data, size);
	mWriteIndex = size; // hack to set size of the buffer.
}

PyReadStream::PyReadStream(size_t size) : PyStream(size), mReadIndex(5)
{
	// this is a hack...
	mWriteIndex = size;
}

PyStream &PyReadStream::operator>>(bool &value) {
	PyRepOpcodes opcode = _popOpcode();
	assert(opcode == Op_PyTrue || opcode == Op_PyFalse); // hehe...
	if (opcode == Op_PyTrue)
		value = true;
	else
		value = false;
	return *this;
}

PyStream &PyReadStream::operator>>(uint8 &value) {
	value = _readInteger<uint8>();
	return *this;
}

PyStream &PyReadStream::operator>>(uint16 &value) {
	value = _readInteger<uint16>();
	return *this;
}

PyStream &PyReadStream::operator>>(int &value) {
	value = _readInteger<int>();
	return *this;
}

PyStream &PyReadStream::operator>>(uint32 &value) {
	value = _readInteger<uint32>();
	return *this;
}

PyStream &PyReadStream::operator>>(uint64 &value) {
	value = _readInteger<uint64>();
	return *this;
}

PyStream &PyReadStream::operator>>(double &value) {
	PyRepOpcodes opcode = _popOpcode();
	assert(opcode == Op_PyFloat || opcode == Op_PyZeroFloat);
	value = _pop8<double>();
	return *this;
}

PyStream &PyReadStream::operator>>(std::wstring &value) {
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

PyStream &PyReadStream::operator>>(std::string &value) {
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

PyRepOpcodes PyReadStream::_popOpcode() {
	if (_validateReadSize(1) == false)
	{
		// signal error system that we where unable to read the opcode
		return PyOpcodeError;
	}

	uint8 opcode = mData[mReadIndex++];
	return static_cast<PyRepOpcodes>(opcode);
}

// a method to seek tough a stream in c style
void PyReadStream::seek(size_t offset, streamSeek rule)
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
size_t PyReadStream::tell()
{
	return mReadIndex;
}

bool PyReadStream::_validateReadSize(size_t size)
{
	if (mReadIndex + size > PyStream::size())
		return false;

	return true;
}

void PyReadStream::_readString(std::string & str, size_t len)
{
	str.clear();
	if (_validateReadSize(len) == false)
		return;

	str.clear();
	str.resize(len);
	memcpy(&str[0], &mData[mReadIndex], len);
	mReadIndex+=len;
	str[len] = '\0';
}

void PyReadStream::_readString(std::wstring & str, size_t len)
{
	str.clear();

	if (_validateReadSize(len) == false)
		return;

	str.resize(len);

	size_t parsedLen = mbstowcs(&str[0], (const char*)&mData[mReadIndex], len);
	assert(parsedLen == len);

	mReadIndex+=len;
}

PyListStream::PyListStream() : PyStream(), mEntryCount(0), mPreDefinedEntryCount(0), mSetExtended(false)
{
	_pushOpcode(Op_PyEmptyList);
}

PyListStream::PyListStream(size_t size) : PyStream(), mEntryCount(0), mPreDefinedEntryCount((uint32)size), mSetExtended(false)
{
	assert(false);
	/*_pushOpcode(Op_PyList);
	if (size > 0xFE)
	{
		_push1(0xFF);
		_push4(size);
	}
	else
	{
		_push1(size);
	}*/
}

void PyListStream::OnObjectWrite(size_t /*size*/)
{
	mEntryCount++;

	// this means the opcode is probably going to change...
	if (mEntryCount <= 2)
	{
		if (mEntryCount == 1)
		{
			mData[0] = Op_PyOneList;
		}
		else if (mEntryCount == 2)
		{
			mData[0] = Op_PyList;
			_push1(0); // count placeholder.
		}		
	}

	if (mSetExtended == true)
	{
		//mData[1] = 0xFF; // should already be set
		uint32* listSize = (uint32*)&mData[2];
		(*listSize) = mEntryCount;
	}
	else
	{
		// check if its extended
		if (mEntryCount > 0xFE)
		{
			__asm {int 3} // hardware breakpoint because I want to know when this happens even when we are running normal shit
			// we change to extended size.
			mSetExtended = true;
			DataItr Itr = mData.begin();
			Itr++; // the 2e iterator is the size one...
			/*Itr = */mData.insert(Itr, 4, 0); // we add 4 bytes ... this is kinda slow.. if we already have a lot of data in the stream.
			
			mData[1] = 0xFF;
			uint32* listSize = (uint32*)&mData[2];
			(*listSize) = mEntryCount;
		}
		else
		{
			// its not extended so... just change the list size
			mData[1] = mEntryCount;
		}
	}
}

/************************************************************************/
/* End PyReadStream implementation                                      */
/************************************************************************/