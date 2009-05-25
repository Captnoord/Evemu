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

#include "ascent.h"
#include "Common.h"
#include "NGLog.h"
#include "Log.h"
#include "string_map.h"
#include "PyChameleon.h"
#include "PyObjects.h"

/************************************************************************/
/* PyChameleon code                                                     */
/************************************************************************/
PyChameleon::PyChameleon() : mPayload(NULL), mKeyObj(NULL)
{
	mIsEmpty = true;
	mType = PyTypeNone;
}

PyChameleon::PyChameleon( PyDictEntry * entry )
{
	mKeyObj = entry->key;
	mPayload = entry->obj;
	if (mPayload != NULL)
	{
		mType = mPayload->gettype();
	}
	else
	{
		mType = PyTypeNone;
	}
	
	mIsEmpty = false;
}

PyChameleon::~PyChameleon()
{
	if (mKeyObj != NULL)
		mKeyObj->DecRef();

	if (mPayload != NULL && mIsEmpty == true)
	{
		ASCENT_HARDWARE_BREAKPOINT;
	}

	if (mPayload)
		((PyObject*)mPayload)->DecRef();

	//if (mIsEmpty == false && mPayload != NULL)
	//	((PyObject*)mPayload)->DecRef();
	//else
	//	ASCENT_HARDWARE_BREAKPOINT;

	mType = PyTypeDeleted;
	mPayload = NULL;
}

PyChameleon &PyChameleon::operator=(const char* const str)
{
	assert(mIsEmpty == true);
	_settype(PyTypeString);
	assert(mPayload == NULL);
	mPayload = (PyObject*)new PyString(str);
	mIsEmpty = false;
	return *this;
}

PyChameleon &PyChameleon::operator=(const std::string &str)
{
	assert(mIsEmpty == true);
	_settype(PyTypeString);
	assert(mPayload == NULL);
	mPayload = (PyObject*)new PyString(str.c_str(), str.size());
	mIsEmpty = false;
	return *this;
}

PyChameleon &PyChameleon::operator=(const uint32 number)
{
	assert(mIsEmpty == true);
	_settype(PyTypeInt);
	assert(mPayload == NULL);
	mPayload = (PyObject*)new PyInt(number);
	mIsEmpty = false;
	return *this;
}

PyChameleon &PyChameleon::operator=(const int number)
{
	assert(mIsEmpty == true);
	_settype(PyTypeInt);
	assert(mPayload == NULL);
	mPayload = (PyObject*)new PyInt(number);
	mIsEmpty = false;
	return *this;
}

PyChameleon &PyChameleon::operator=(const float number)
{
	assert(mIsEmpty == true);
	_settype(PyTypeReal);
	assert(mPayload == NULL);
	mPayload = (PyObject*)new PyFloat(number);
	mIsEmpty = false;
	return *this;
}

PyChameleon &PyChameleon::operator=(const PyDict* pointer)
{
	assert(pointer != NULL);
	assert(mIsEmpty == true);
	_settype(PyTypeDict);
	assert(mPayload == NULL);
	mPayload = (PyObject*)pointer;
	mIsEmpty = false;
	return *this;
}

PyChameleon &PyChameleon::operator=(const PyList* pointer)
{
	assert(pointer != NULL);
	assert(mIsEmpty == true);
	_settype(PyTypeDict);
	assert(mPayload == NULL);
	mPayload = (PyObject*)pointer;
	mIsEmpty = false;
	return *this;
}

PyChameleon &PyChameleon::operator=(const PyTuple* pointer)
{
	assert(pointer != NULL);
	//if (pointer == NULL)
	//	return *this;

	assert(mIsEmpty == true);
	_settype(PyTypeTuple);
	assert(mPayload == NULL);
	mPayload = (PyObject*)pointer;
	mIsEmpty = false;
	return *this;
}

PyChameleon & PyChameleon::operator=(const PyBaseNone* pointer)
{
	assert(mIsEmpty == true);
	this->_settype(PyTypeNone);
	assert(mPayload == NULL);
	mPayload = (PyObject*)pointer;
	mIsEmpty = false;
	return *this;
}

PyChameleon &PyChameleon::operator=(PyObject* pointer)
{
	assert(pointer != NULL);

	//if (pointer == NULL)
	//	return *this;

	assert(mIsEmpty == true);
	this->_settype(pointer->gettype());
	assert(mPayload == NULL);
	mPayload = (PyObject*)pointer;
	mIsEmpty = false;
	return *this;
}

PyChameleon &PyChameleon::operator=(PyObject& pointer)
{
	assert(mIsEmpty == true);
	ASCENT_HARDWARE_BREAKPOINT;
	mIsEmpty = false;
	return *this;
}

PyChameleon & PyChameleon::operator=( const double number )
{
	assert(mIsEmpty == true);
	_settype(PyTypeReal);
	assert(mPayload == NULL);
	mPayload = (PyObject*)new PyFloat(number);
	mIsEmpty = false;
	return *this;
}

PyChameleon & PyChameleon::operator=( PyString* str )
{
	assert(str != NULL);

	assert(mIsEmpty == true);
	this->_settype(str->gettype());
	assert(mPayload == NULL);
	mPayload = (PyObject*)str;
	mIsEmpty = false;
	return *this;
}

PyChameleon & PyChameleon::operator=( const PyBool* pointer )
{
	assert(pointer != NULL);
	assert(mIsEmpty == true);
	_settype(PyTypeBool);
	assert(mPayload == NULL);
	mPayload = (PyObject*)pointer;
	mIsEmpty = false;
	return *this;
}

PyChameleon & PyChameleon::operator=( const PySubStream* pointer )
{
	assert(pointer != NULL);
	//assert(mIsEmpty == true);
	_settype(PyTypeSubStream);
	//assert(mPayload == NULL);
	
	/* remove the reference if we are replacing a entry */
	if(mPayload != NULL)
		mPayload->DecRef();

	mPayload = (PyObject*)pointer;
	mIsEmpty = false;
	return *this;
}

PyChameleon & PyChameleon::operator=( const PyInt* pyInt )
{
	assert(pyInt != NULL);
	assert(mIsEmpty == true);
	assert(mPayload == NULL);

	_settype(PyTypeInt);
	mPayload = (PyObject*)pyInt;
	mIsEmpty = false;
	return *this;
}

uint8 PyChameleon::gettype()
{
	return mType;
}

bool PyChameleon::isempty()
{
	return mIsEmpty;
}

void PyChameleon::_settype( uint8 type )
{
	assert(mType == PyTypeNone); // O we don't want to reuse the object so only 1 thingy...
	mType = type;
}

bool PyChameleon::setPyObject( PyObject* obj )
{
	assert(obj != NULL);
	if (obj == NULL || obj->gettype() == PyTypeDeleted)
		return false;

	assert(mPayload == NULL);
	if( mPayload != NULL)
		mPayload->DecRef();

	mPayload = obj;
	mIsEmpty = false;
	return true;
}

PyObject * PyChameleon::getPyObject()
{
	return mPayload;
}

PyObject & PyChameleon::getPyObjRef()
{
	return *mPayload;
}

void PyChameleon::setAtributeObj( PyObject* attrib )
{
	assert(mKeyObj == NULL);
	assert(attrib);
	mKeyObj = attrib;
	attrib->IncRef();
}

PyObject* PyChameleon::getAtributeObj()
{
	return mKeyObj;
}