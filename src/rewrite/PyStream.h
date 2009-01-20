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

#ifndef PY_STREAM_H
#define PY_STREAM_H

//#include "EvemuPCH.h"
//#include "EVEMarshalOpcodes.h"

/* how the client marshals objects */
/*
void Marshal(const char* format, ...)
{
}
*/

// some simple constants for seeking trough a stream.

/**
 * @brief streamSeek represents a buffer seek system similar to the FILE stuff.
 */
enum streamSeek
{
	STREAM_SEEK_SET = 0,
	STREAM_SEEK_CUR = 1,
	STREAM_SEEK_END = 2,
};

/**
 * \class PyStream
 *
 * @brief PyStream is the base class for every simple packet stream.
 *
 * The aim of this class is to create a stream class for simple packet, which
 * allows the developer to add known C/C++ types to a stream. The class then
 * trytranslates those types into the correct
 *
 * @author Captnoord.
 * @date December 2008
 */
class PyStream
{
public:
	PyStream();
	PyStream(size_t size);

	virtual ~PyStream();

	PyStream(const PyStream& stream);
	/**
	 * @brief operator overload for booleans.
	 *
	 * 
	 *
	 * @param[in] value is the boolean that is added to the stream.
	 * @return we return the reference to the stream.
	 */
	PyStream &operator<<(bool value);
	
	/**
	 * @brief operator overload for doubles.
	 *
	 * 
	 *
	 * @param[in] value the double that needs to be added to the stream.
	 * @return we return the reference to the stream.
	 */
	PyStream &operator<<(const double& value);
	
	/**
	 * @brief operator overload for uint8's.
	 *
	 * 
	 *
	 * @param[in]
	 * @return we return the reference to the stream.
	 */
	PyStream &operator<<(const uint8& value);

	/**
	 * @brief operator overload for uint32's.
	 *
	 * 
	 *
	 * @param[in]
	 * @return we return the reference to the stream.
	 */
	PyStream &operator<<(const uint32& value);

	/**
	 * @brief operator overload for int's.
	 *
	 * 
	 *
	 * @param[in]
	 * @return we return the reference to the stream.
	 */
	PyStream &operator<<(const int value);
	
	/**
	 * @brief operator overload for a const char* string's.
	 *
	 * 
	 *
	 * @param[in] str is the string that needs to be added to the stream.
	 * @return we return the reference to the stream.
	 */
	PyStream &operator<<(const char* const str);
	
	/**
	 * @brief operator overload for std::string objects.
	 *
	 * 
	 *
	 * @param[in] str is the string that needs to be added to the stream.
	 * @return we return the reference to the stream.
	 */
	PyStream &operator<<(std::string& str);

	/**
	 * @brief appends a existing stream to our stream.
	 *
	 * 
	 *
	 * @param[in] stream the stream object that needs to be appended to the present stream.
	 */
	void _pushStream(PyStream & stream);
	
	/**
	 * @brief appends a existing stream to our stream with a certain length.
	 *
	 * 
	 *
	 * @param[in] stream the stream chunk that needs to be appended to the present stream.
	 * @param[in] len the length of the stream chunk that gets added.
	 * @param[in] offset the offset of the stream chunk that gets added.
	 */
	void _pushStream(PyStream & stream, size_t len, size_t offset = -1);

protected:

	/* typedef to make the code a bit more readable */
	typedef const unsigned char const_uc;

	/**
	 * @brief _push8 is a internal function that takes care of adding eight byte variables to the stream.
	 *
	 * uint64, int64, double, time_t
	 *
	 * @param[in] value the variable that needs to be added.
	 */
	template <typename T>
	void _push8(T & value) {
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
	
	/**
	 * @brief _push4 is a internal function that takes care of adding four byte variables to the stream.
	 *
	 * size_t, int, uint32, int32, float
	 *
	 * @param[in] value the variable that needs to be added.
	 */
	template <typename T>
	void _push4(T value) {
		_validateBufferSize(4);
		mData[mWriteIndex++] = ((const_uc*)&value)[0];
		mData[mWriteIndex++] = ((const_uc*)&value)[1];
		mData[mWriteIndex++] = ((const_uc*)&value)[2];
		mData[mWriteIndex++] = ((const_uc*)&value)[3];
	}
	
	/**
	 * @brief _push2 is a internal function that takes care of adding double byte variables to the stream.
	 *
	 * lame azz template as we only know int16 as 2 byte chunk
	 *
	 * @param[in] value the variable that needs to be added.
	 */
	template <typename T>
	void _push2(T value) {
		_validateBufferSize(2);
		mData[mWriteIndex++] = ((const_uc*)&value)[0];
		mData[mWriteIndex++] = ((const_uc*)&value)[1];
	}
	
	/**
	 * @brief _push1 is a internal function that takes care of adding single byte's to the stream.
	 *
	 * crappy'r function as it only would add int8/uint8/char/unsigned char's
	 *
	 * @param[in] value the variable that needs to be added.
	 */
	template <typename T>
	void _push1(T value) {
		_validateBufferSize(1);
		mData[mWriteIndex++] = ((const_uc*)&value)[0];
	}

	/**
	 * @brief adds a string to the stream.
	 *
	 * 
	 *
	 * @param[in] string is the std::string that contains the string data.
	 */
	void _pushString(const std::string & string);
	
	/**
	 * @brief adds a opcode byte to the stream
	 *
	 * void/spam/hell
	 *
	 * @param[in] opcode the opcode of the object that is going in the stream
	 */
	void _pushOpcode(PyRepOpcodes opcode);

	/**
	 * @brief templated version of the integer marshal function.
	 *
	 * 
	 *
	 * @param[in] value the number that needs marshaling.
	 * @note even as it seems logical, don't use this function for non integers.
	 * @note doesn't support LongLong's if you need it this isn't the best function for that.
	 */
	template <typename T>
	void _writeInteger(T & value) {
		if (value == 0)
		{
			_pushOpcode(Op_PyZeroInteger);
		}
		else if(value == 1)
		{
			_pushOpcode(Op_PyOneInteger);
		}
		else if (value + 0x80u > 0xFF)
		{
			if (value + 0x8000u > 0xFFFF)
			{
				if (value + 0x800000u > 0xFFFFFFFF)
				{
					/* don't support very large integers, for various reasons */
					assert(false);
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
	
	/**
	 * @brief WriteString does the marshal part for the string
	 *
	 * 
	 *
	 * @param[in] str the string that needs to be streamed to the data stream.
	 * @param[in] len is the length of the string that is marshaled.
	 */
	void _writeString(const char* str, size_t len);
		
	/**
	 * @brief check if the stream buffer has enough free memory.
	 *
	 * void/spam/hell
	 *
	 * @param[in] size the size of the chunk that is pending to be written.
	 */
	void _validateBufferSize(size_t size);
	
	/**
	 * @brief pure virtual function that acts like a event trigger when a object is added to the stream
	 *
	 * OnObjectWrite is a pure virtual function that acts like a event trigger when a object is added to the stream.
	 * This allows the stream derived classes to do something with it. The Tuple class uses it to increase the object count,
	 * Which in fact all classes derived from this one should.
	 *
	 */
	//virtual void OnObjectWrite() = 0;
	virtual void OnObjectWrite() {}; // I still think this should be a pure virtual function.

	/**
	 * @brief pure virtual function that acts like a event trigger when the length of the buffer has changed.
	 *
	 * @todo check if this is useful for PyStream or something.
	 *
	 */
	//virtual void OnBufferResize() = 0;

public:

	/**
	 * @brief returns the size of the data.
	 *
	 * void/spam/hell
	 *
	 * @return the size of the content buffer
	 */
	size_t size() {return mWriteIndex;}

	/**
	 * @brief returns the pointer to the content
	 *
	 * void/spam/hell
	 *
	 * @return the pointer to data.
	 */
	uint8* content() {return &mData[0];}

protected:
	std::vector<uint8> mData;
	size_t mWriteIndex;
};

/**
 * \class PyTupleStream
 *
 * @brief a class for a single/simple tuple communication packet.
 *
 * 
 *
 * @author Captnoord
 * @date December 2008
 */
class PyTupleStream : public PyStream
{
public:
	//friend PyStream;

	/**
	 * @brief Tuple constructor.
	 *
	 * nothing special about this constructor.
	 *
	 * @param[in] size is the expected packet size of the packet.
	 */
	PyTupleStream(size_t size);

private:
	/**
	 * @brief the OnObjectWrite event
	 *
	 * 
	 *
	 */
	void OnObjectWrite();
};

/**
 * \class PyIntStream
 *
 * @brief a class for a single/simple integer communication packet.
 *
 * 
 *
 * @author Captnoord
 * @date January 2009
 */
class PyIntStream : public PyStream
{
	//friend PyStream;
public:

	/**
	 * @brief Marshal stream Int packet.
	 *
	 * 
	 *
	 * @param[in] T is the int object that needs to be send.
	 * @note IMPORTANT DON'T MOVE THIS CODE TO THE cpp FILE.
	 */
	template<typename T>
	PyIntStream(T value) : PyStream(14)
	{
		/* write marshal header + marshal int */
		_push1('~');
		_push4(0);
		_writeInteger(value);
	}
};

/**
 * \class PyStringStream
 *
 * @brief a class for a single/simple string communication packet.
 *
 * 
 *
 * @author Captnoord
 * @date January 2009
 */
class PyStringStream : public PyStream
{
	//friend PyStream;
public:

	/**
	 * @brief PyStringStream constructor that takes a const char* array.
	 *
	 * 
	 *
	 * @param[in] str string data that gets converted into a string packet.
	 */
	PyStringStream(const char* str);

	/**
	 * @brief PyStringStream constructor that takes a const char* array and the length of the string.
	 *
	 * 
	 *
	 * @param[in] str string data that gets converted into a string packet.
	 * @param[in] len the length of the string data.
	 */
	PyStringStream(const char* str, size_t len);

	/**
	* @brief PyStringStream constructor that takes a std::string.
	*
	* 
	*
	* @param[in] str string data that gets converted into a string packet.
	*/
	PyStringStream(std::string & str);
};

/**
 * \class PyDictStream
 *
 * @brief This class is intended as a python dict write class.
 *
 * 
 *
 * @author Captnoord
 * @date January 2009
 * @note WARNING NOT FINISHED DOESN'T WORK AS INTENDED.
 * @todo rewrite, finish, test, move code to cpp file.
 */
class PyDictStream : public PyStream
{
public:
	PyDictStream() : PyStream(6)
	{
		assert(false);
		/*  */
		_push1('~');
		_push4(0);
		_pushOpcode(Op_PyDict);
	}

	PyStream &operator[](const char* str)
	{
		assert(false);
		std::string keyName(str);
		DictMapItr Itr = m_Dict.find(keyName);
		if (Itr != m_Dict.end())
		{
			return *((PyStream*)&Itr->second);

		}
		else
		{
			// slow..... VERY SLOW...
			PyStream entry(0);
			m_Dict[keyName] = entry;
			return m_Dict[keyName];
		}
	}

	// we can increase object count with this...
	void OnObjectWrite()
	{
		assert(false);
	}

	void finish()
	{
		assert(false);
		uint32 count = (uint32)m_Dict.size();
		if (count < 0xFF)
		{
			_push1(count);
		}
		else
		{
			_push1(0xFF);
			_push4(count);
		}

		DictMapItr Itr = m_Dict.begin();
		for (; Itr != m_Dict.end(); Itr++)
		{
			_pushStream(Itr->second);
			_pushString(Itr->first);
		}
	}

private:
	typedef std::tr1::unordered_map<std::string, PyStream>	DictMap;
	typedef DictMap::iterator								DictMapItr;
	typedef DictMap::const_iterator							DictMapConstItr;
	DictMap m_Dict;
};

/**
 * \class PyNetworkStream
 *
 * @brief first try to make a generic network stream class for reading.
 *
 * 
 *
 * @author Captnoord
 * @date January 2009
 * @todo change the class its name into something like PyReadStream.
 * @todo merge this class with the PyStream class which in his turn should be called PyWriteStream.
 */
class PyNetworkStream : public PyStream
{
public:

	/**
	 * @brief 
	 *
	 * 
	 *
	 * @param[in]
	 * @param[out]
	 * @return
	 */
	PyNetworkStream();

	/**
	 * @brief 
	 *
	 * 
	 *
	 * @param[in]
	 * @param[out]
	 * @return
	 */
	PyNetworkStream(const char * data, size_t size);

	/**
	 * @brief 
	 *
	 * 
	 *
	 * @param[in]
	 * @param[out]
	 * @return
	 */
	PyNetworkStream(size_t size);

	/**
	 * @brief 
	 *
	 * 
	 *
	 * @param[in]
	 * @param[out]
	 * @return
	 */
	PyStream &operator>>(bool &value);

	/**
	 * @brief 
	 *
	 * 
	 *
	 * @param[in]
	 * @param[out]
	 * @return
	 */
	PyStream &operator>>(uint8 &value);
	PyStream &operator>>(uint16 &value);
	PyStream &operator>>(int &value);
	PyStream &operator>>(uint32 &value);
	PyStream &operator>>(uint64 &value);
	PyStream &operator>>(double &value);	
	PyStream &operator>>(std::wstring &value);
	PyStream &operator>>(std::string &value);

	template <typename T>
	void ZeroVar(T & var)
	{
		memset(&var, 0, sizeof(T));
	}

	template <typename T>
	T _pop8()
	{
		T value;
		ZeroVar(value);

		// todo add size check.
		((uint8*)&value)[0] = mData[mReadIndex++];
		((uint8*)&value)[1] = mData[mReadIndex++];
		((uint8*)&value)[2] = mData[mReadIndex++];
		((uint8*)&value)[3] = mData[mReadIndex++];
		((uint8*)&value)[4] = mData[mReadIndex++];
		((uint8*)&value)[5] = mData[mReadIndex++];
		((uint8*)&value)[6] = mData[mReadIndex++];
		((uint8*)&value)[7] = mData[mReadIndex++];
		return value;
	}

	template <typename T>
	T _pop4()
	{
		T value;
		ZeroVar(value);

		// todo add size check.
		((uint8*)&value)[0] = mData[mReadIndex++];
		((uint8*)&value)[1] = mData[mReadIndex++];
		((uint8*)&value)[2] = mData[mReadIndex++];
		((uint8*)&value)[3] = mData[mReadIndex++];
		return value;
	}

	template <typename T>
	T _pop2()
	{
		T value;
		ZeroVar(value);

		// todo add size check.
		((uint8*)&value)[0] = mData[mReadIndex++];
		((uint8*)&value)[1] = mData[mReadIndex++];
		return value;
	}

	template <typename T>
	T _pop1()
	{
		T value;
		ZeroVar(value);

		// todo add size check.
		((uint8*)&value)[0] = mData[mReadIndex++];
		return value;
	}

	PyRepOpcodes _popOpcode();

	// a method to seek tough a stream in c style
	void _seek(size_t offset, streamSeek rule);

	// get the current offset
	size_t _tell();	
	void _readString(std::string & str, size_t len);
	void _readString(std::wstring & str, size_t len);

	template<typename T>
	T _readInteger()
	{
		T value;
		size_t varSize = sizeof(T);// hack?

		// make sure we don't read more than we actually can
		if (mReadIndex + 1 + varSize > size())
			return 0;

		PyRepOpcodes opcode = _popOpcode();
		switch(opcode)
		{
			case Op_PyLongLong:
			{
				assert(varSize == 8);
				value = _pop8<T>();
			} break;

			case Op_PyLong:
			{
				assert(varSize >= 4);
				value = _pop4<T>();
			} break;

			case Op_PySignedShort:
			{
				assert(varSize >= 2);
				value = _pop2<T>();
			} break;

			case Op_PyByte:
			{
				assert(varSize >= 1);
				value =_pop1<T>();
			} break;

			case Op_PyInfinite:
			{
				value = (unsigned)(T)-1;
			} break;

			case Op_PyZeroInteger:
			{
				value  = 0;
			} break;

			case Op_PyOneInteger:
			{
				value = 1;
			} break;

			default:
			{
				assert(false);
			}
		}
		return value;
	}

private:
	size_t mReadIndex;
};

enum ItrRules
{
	ITR_RULE_NONE		= 0,
	ITR_RULE_SIMPLE		= -1,
	ITR_RULE_DYNAMIC	= -2,
	ITR_RULE_COMPLEX	= -3,
};
static uint32 PyStreamItrLengthRules[] = {
	ITR_RULE_NONE, 
	ITR_RULE_NONE,		//Op_PyNone				= 0x01,
	ITR_RULE_SIMPLE,	//Op_PyByteString		= 0x02,
	8,					//Op_PyLongLong			= 0x03,
	4,					//Op_PyLong				= 0x04,
	2,					//Op_PySignedShort		= 0x05,
	1,					//Op_PyByte				= 0x06,
	ITR_RULE_NONE, 		//Op_PyInfinite			= 0x07,
	ITR_RULE_NONE, 		//Op_PyZeroInteger		= 0x08,
	ITR_RULE_NONE, 		//Op_PyOneInteger		= 0x09,
	8,					//Op_PyReal				= 0x0A,
	ITR_RULE_NONE, 		//Op_PyZeroReal			= 0x0B,
	ITR_RULE_NONE, 		//Op_PyUnused1			= 0x0C,
	ITR_RULE_DYNAMIC,	//Op_PyBuffer			= 0x0D,
	ITR_RULE_NONE, 		//Op_PyEmptyString		= 0x0E,
	1,					//Op_PyCharString		= 0x0F,
	ITR_RULE_DYNAMIC,	//Op_PyShortString		= 0x10, 
	1,					//Op_PyStringTableItem	= 0x11,
	ITR_RULE_DYNAMIC,	//Op_PyUnicodeByteString= 0x12,
	ITR_RULE_NONE, 		//Op_Pyunk1				= 0x13,
	ITR_RULE_COMPLEX,	//Op_PyTuple			= 0x14,
	ITR_RULE_COMPLEX,	//Op_PyList				= 0x15,
	ITR_RULE_COMPLEX,	//Op_PyDict				= 0x16,
	ITR_RULE_COMPLEX,	//Op_PyObject			= 0x17,
	ITR_RULE_NONE, 		//Op_Pyunk2				= 0x18,
	ITR_RULE_COMPLEX,	//Op_PySubStruct		= 0x19,
	ITR_RULE_NONE, 		//Op_Pyunk3				= 0x1A,
	ITR_RULE_DYNAMIC,	//Op_PySavedStreamElement = 0x1B, /*not sure about this one*/
	ITR_RULE_COMPLEX,	//Op_PyChecksumedStream	= 0x1C,
	ITR_RULE_NONE, 		//Op_PyUnused2			= 0x1D,
	ITR_RULE_NONE, 		//Op_PyUnused3			= 0x1E,
	ITR_RULE_NONE, 		//Op_PyTrue				= 0x1F,
	ITR_RULE_NONE, 		//Op_PyFalse			= 0x20,
	ITR_RULE_NONE, 		//Op_Pyunk4				= 0x21,
	ITR_RULE_COMPLEX,	//Op_NewObject1			= 0x22,
	ITR_RULE_COMPLEX,	//Op_NewObject2			= 0x23,
	ITR_RULE_NONE, 		//Op_PyEmptyTuple		= 0x24,
	ITR_RULE_COMPLEX,	//Op_PyOneTuple			= 0x25,
	ITR_RULE_NONE, 		//Op_PyEmptyList		= 0x26,
	ITR_RULE_COMPLEX,	//Op_PyOneList			= 0x27,
	ITR_RULE_NONE, 		//Op_PyEmptyUnicodeString = 0x28,
	2,					//Op_PyUnicodeCharString= 0x29,
	ITR_RULE_COMPLEX,	//Op_PyPackedRow		= 0x2A,
	ITR_RULE_COMPLEX,	//Op_PySubStream		= 0x2B,
	ITR_RULE_COMPLEX,	//Op_PyTwoTuple			= 0x2C,
	ITR_RULE_NONE, 		//Op_PackedTerminator	= 0x2D,
	ITR_RULE_DYNAMIC,	//Op_PyUnicodeString	= 0x2E,
	ITR_RULE_COMPLEX,	//Op_PyVarInteger		= 0x2F,
};

/* this class represents a generic chunk of a stream, its mainly used for lists and dicts */
class PyStreamItr
{
public:
	PyStreamItr() : chunkSize(0){}

	// the purpose of the constructor is to grasp a "item" from the stream and store it for later use.
	PyStreamItr(PyNetworkStream & stream) : chunkSize(0)
	{
		ExtractItr(stream, chunk);
	}

	// extract a itr item by the marshal rules
	void ExtractItr(PyNetworkStream& in, PyNetworkStream & out)
	{
		chunkSize = 1;
		size_t curOffset = in._tell();
		PyRepOpcodes opcode = in._pop1<PyRepOpcodes>();

		// check if we aren't fucked up
		if (opcode == 0 || opcode > 0x2F)
			return;

		uint32 itrRule = PyStreamItrLengthRules[opcode];
		
		if (itrRule == ITR_RULE_SIMPLE)
		{
			size_t size = (size_t)in._pop1<uint32>();
			chunkSize += size;
		}
		else if (itrRule == ITR_RULE_DYNAMIC)
		{
			size_t size = (size_t)in._pop1<uint32>();
			if (size == 0xFF)
				size = (size_t)in._pop4<uint32>() + 4;
			chunkSize += size + 1;
		}
		else if (itrRule == ITR_RULE_COMPLEX)
		{
			// we don't handle these yet
			assert(false);
		}
		else
		{
			chunkSize += itrRule;
		}

		// the best generic code I have ever seen.
		in._seek(curOffset, STREAM_SEEK_SET);
		out._pushStream(in, chunkSize, in._tell());
		in._seek(chunkSize, STREAM_SEEK_CUR);
	}

	// fake function crap thingy
	template<typename T>
	PyStreamItr &operator>>(T &value) {
		chunk >> value;
		return *this;
	}

	// returns the iterator stream size.
	size_t size() {return chunkSize;}

private:
	PyNetworkStream chunk;
	size_t chunkSize;
};

static PyStreamItr ErrorItr;

//universal container for n number of 
class PyDictNetStream
{
public:
	// nothing special yet
	PyDictNetStream()
	{
	}
	PyDictNetStream(PyNetworkStream & packet)
	{
		insertStream(packet);
	}
	~PyDictNetStream()
	{
		DictMapItr Itr = mDict.begin();
		for (; Itr != mDict.end(); Itr++)
		{
			delete Itr->second;
		}
	}


	void insertStream(PyNetworkStream & packet)
	{
		PyRepOpcodes opcode = packet._pop1<PyRepOpcodes>();
		ASCENT_ASSERT(opcode == Op_PyDict);

		uint32 dictObjectCount = packet._pop1<uint32>();
		if (dictObjectCount == 0xFF)
		{
			dictObjectCount = packet._pop4<uint32>();
		}

		// O fucking hell read and parse:S
		for (uint32 i = 0; i < dictObjectCount; i++)
		{
			std::string keyName;
			PyStreamItr * inItr = new PyStreamItr(packet);	// extract the dict entry iterator.
			packet >> keyName;								// extract the dict entry key name.
			mDict[keyName] = inItr;
		}
	}

	PyStreamItr &operator[](const char* str) {
		std::string keyName(str);
		DictMapConstItr Itr = mDict.find(keyName);
		if (Itr != mDict.end())
			return *Itr->second;

		// small time hack to fix the crap thingys
		return ErrorItr;
	}



protected:
	typedef std::tr1::unordered_map<std::string, PyStreamItr*>		DictMap;
	typedef DictMap::iterator										DictMapItr;
	typedef DictMap::const_iterator									DictMapConstItr;
	DictMap mDict;
};

class PyTupleNetStream
{
public:
	PyTupleNetStream(PyNetworkStream & packet) : tupleObjectCount(0), stream(packet)
	{
		PyRepOpcodes opcode = packet._pop1<PyRepOpcodes>();
		if (opcode == Op_PyTuple)
			tupleObjectCount = packet._pop1<uint8>();
		else if (opcode == Op_PyTwoTuple)
			tupleObjectCount = 2;
		else if (opcode == Op_PyOneTuple)
			tupleObjectCount = 1;
		else if (opcode == Op_PyEmptyTuple)
			tupleObjectCount = 0;
		else
			assert(false);

	}

	// fake function crap thingy
	template<typename T>
	PyTupleNetStream &operator>>(T &value) {
		stream >> value;
		return *this;
	}

	//class PyDictNetStream;
	PyTupleNetStream &operator>>(PyDictNetStream &value)
	{
		value.insertStream(stream);
		return *this;
	}

	size_t size() {return (size_t)tupleObjectCount;}

protected:
	
	PyNetworkStream & stream;
	uint8 tupleObjectCount;
	uint8 tupleReadIndex;
};

#endif//PY_STREAM_H
