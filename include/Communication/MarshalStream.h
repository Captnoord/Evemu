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
#ifndef _MARSHALSTREAM_H
#define _MARSHALSTREAM_H

class PyObject;
class PyStream;
class MarshalReadStream;
class UnmarshalReferenceMap;

#include "MarshalReferenceMap.h"

/* although I don't like it we need a marshal stream class.. */
class MarshalStream
{
public:
	MarshalStream();
	~MarshalStream();
	PyObject* load(MarshalReadStream & stream);
	void save(PyObject * object);

protected:

	PyInt PyIntZero;
	PyInt PyIntOne;
	PyInt PyIntMinOne;
	PyInt PyIntInfinite;

	PyFloat PyFloatZero;
	PyBool	Py_TrueStruct;
	PyBool	Py_ZeroStruct;

	PyString PyStringEmpty;

	UnmarshalReferenceMap sharedObjectsMapInternal;
#define sharedObjectsMap (sharedObjectsMapInternal)
	
private:
	PyObject* unmarshal(MarshalReadStream & stream);

	PyObject* ReadBuffer(MarshalReadStream & stream);

	bool ReadMarshalHeader(MarshalReadStream & stream);

	/**
	 * @brief ReadClassString
	 *
	 * needs some love
	 *
	 * @param[in]
	 * @param[out]
	 * @return
	 */
	PyObject* ReadClassString(MarshalReadStream & stream, bool shared);
	PyObject* ReadClass(MarshalReadStream & stream, bool shared);
	PyObject* ReadNewObject1(MarshalReadStream & stream, bool shared);
	PyObject* ReadNewObject2(MarshalReadStream & stream, bool shared);
	PyObject* ReadPackedRow(MarshalReadStream & stream);
	PyObject* ReadSubStream(MarshalReadStream & stream);
	PyObject* ReadVarInteger(MarshalReadStream & stream, bool shared);

	//////////////////////////////////////////////////////////////

	/* small internal function to do some easy reading */
	PyObject* _ReadPyStringFromStringAndSize(MarshalReadStream & stream);	

	//PyStream	marshalStream;			// container for keeping a marshaled stream
	PyObject*	unmarshalObjects;		// container for keeping the unmarshalled objects

	bool _deleted;

	bool _ReadNewObjList(MarshalReadStream & stream, PyClass & obj);
	bool _ReadNewObjDict(MarshalReadStream & stream, PyClass & obj);

	/**
	 * @brief checkAndInflate is a temp function to handle the possibility that a packets is zipped
	 *
	 * this should really be done somewhere else.
	 *
	 * @return true is check is successfully.. false means something went wrong...
	 */
	bool checkAndInflate( MarshalReadStream & stream );
	uint8 zlibworkbuffer[100000];
};

#endif //_MARSHALSTREAM_H
