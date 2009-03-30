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

#ifndef _PYCHAMELEON_H
#define _PYCHAMELEON_H

class PyInt;
class PyDict;
class PyList;
class PyTuple;
class PyPackedRow;
class PyLong;
class PyFloat;
class PyBool;
class PyClass;
class PyObject;
class PyBaseNone;
struct PyDictEntry;

/**
 * \class PyChameleon
 *
 * @brief the mythical lizard that can shift color to blend to his surrounding.
 *
 * tries to be a universal objects for python types.
 *
 * @note this object evolved into a generic iterator object.
 * @author Captnoord
 * @date January 2009
  */
class PyChameleon
{
public:
	uint8 gettype();
	PyChameleon();
	~PyChameleon();
	PyChameleon(PyDictEntry * entry);

	bool isempty();

	void setAtributeObj(PyObject* attrib);
	PyObject* getAtributeObj();

	PyObject * getPyObject();
	PyObject & getPyObjRef();
	bool setPyObject(PyObject* obj);

	/**
	 * @brief 
	 *
	 * 
	 *
	 * @param[in]
	 * @param[out]
	 * @return
	 */
	PyChameleon &operator=(const char* const str);
	PyChameleon &operator=(const std::string &str);

	PyChameleon &operator=(const wchar_t* const str);
	PyChameleon &operator=(const std::wstring &str);

	PyChameleon &operator=(const uint32 number);
	PyChameleon &operator=(const int number);
	PyChameleon &operator=(const float number);
	PyChameleon &operator=(const PyDict* pointer);
	PyChameleon &operator=(const PyList* pointer);
	PyChameleon &operator=(const PyTuple* pointer);
	PyChameleon &operator=(const PyBaseNone* pointer);

	PyChameleon &operator=(PyObject* pointer);
	PyChameleon &operator=(PyObject& pointer);

protected:
	void _settype(uint8 type);

private:
	uint8 mType;
	PyObject* mPayload;
	bool mIsEmpty;			// indicates a non active PyChameleon
	PyObject* mKeyObj; // optional
};

/**
 * \class PyErrorChameleon
 *
 * @brief the PyErrorChameleon class is a VERY important class related to error detection and checking.
 *
 * 
 *
 * @author Captnoord.
 * @date January 2009
 * @note should this be a singleton???
 */
class PyErrorChameleon : public PyChameleon
{
public:
	PyErrorChameleon() : PyChameleon() {}
	
	template<typename T>
	PyErrorChameleon &operator=(T& str)
	{
		Log.Error("PyErrorChameleon", "Error we are used... thats not good... hmm.. there must have been something wrong...");
		ASCENT_HARDWARE_BREAKPOINT;
		return *this;
	}
};

static PyErrorChameleon PyErrorIterator;
#endif //_PYCHAMELEON_H