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

#include "Common.h"
#include "EVEMarshalOpcodes.h"

/* how the client marshals objects */
/*
void Marshal(const char* format, ...)
{
}
*/

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
	PyStream(size_t size);
	virtual ~PyStream();

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
	 * @brief operator overload for uint64's.
	 *
	 * don't support very large integers for the moment
	 *
	 * @param[in] value is the variable that needs to be added to the stream.
	 * @return we return the reference to the stream.
	 */
	/*PyStream &operator<<(uint64& value);*/	

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
	void _push8(T & value);
	
	/**
	 * @brief _push4 is a internal function that takes care of adding four byte variables to the stream.
	 *
	 * size_t, int, uint32, int32, float
	 *
	 * @param[in] value the variable that needs to be added.
	 */
	template <typename T>
	void _push4(T value);
	
	/**
	 * @brief _push2 is a internal function that takes care of adding double byte variables to the stream.
	 *
	 * lame azz template as we only know int16 as 2 byte chunk
	 *
	 * @param[in] value the variable that needs to be added.
	 */
	template <typename T>
	void _push2(T value);
	
	/**
	 * @brief _push1 is a internal function that takes care of adding single byte's to the stream.
	 *
	 * crappy'r function as it only would add int8/uint8/char/unsigned char's
	 *
	 * @param[in] value the variable that needs to be added.
	 */
	template <typename T>
	void _push1(T value);
	
	/**
	 * @brief templated version of the integer marshal function.
	 *
	 * 
	 *
	 * @param[in] value the number that needs marshaling.
	 */
	template <typename T>
	void WriteInteger(T & value);
	
	/**
	 * @brief WriteString does the marshal part for the string
	 *
	 * 
	 *
	 * @param[in] str the string that needs to be streamed to the data stream.
	 * @param[in] len is the length of the string that is marshaled.
	 */
	void WriteString(const char* str, size_t len);
	
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
	virtual void OnObjectWrite() = 0;

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
 * @brief a class for a single/simple tuple communication packet
 *
 * 
 *
 * @author Captnoord
 * @date December 2008
 */
class PyTupleStream : public PyStream
{
public:
	friend PyStream;

	/**
	 * @brief Tuple constructor
	 *
	 * nothing special about this constructor
	 *
	 * @param[in] size is the expected packet size of the packet
	 */
	PyTupleStream(size_t size);
	
	/**
	 * @brief the OnObjectWrite event
	 *
	 * 
	 *
	 */
	virtual void OnObjectWrite();
};

#endif//PY_STREAM_H
