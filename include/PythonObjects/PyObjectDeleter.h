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

#ifndef _PYOBJECTDELETER_H
#define _PYOBJECTDELETER_H

/* central deletion system, the only real fuckup in the system */
template<typename T>
static bool PyDelete(T * obj)
{
	uint8 testcase = obj->gettype();
	switch(testcase)
	{
	case PyTypeNone:
		//printf("deleting None: %p\n", obj);
		delete ((PyBaseNone*)obj);
		return true;
	case PyTypeBool:
		//printf("deleting bool: %p\n", obj);
		delete ((PyBool*)obj);
		return true;
	case PyTypeInt:
		//printf("deleting int: %p\n", obj);
		delete ((PyInt*)obj);
		return true;
	case PyTypeLong:
		delete ((PyLong*)obj);
		return true;
	case PyTypeReal:
		//printf("deleting float: %p\n", obj);
		delete ((PyFloat*)obj);
		return true;
	case PyTypeString:
		//PyString * str = (PyString *)obj;
		//printf("deleting string: %s: %p\n", str->content(), obj);
		delete ((PyString*)obj);
		return true;
	case PyTypeUnicode:
		//printf("deleting unicode string: %p\n", obj);
		delete ((PyUnicodeUCS2*)obj);
		return true;
	case PyTypeDict:
		//printf("deleting dict: %p\n", obj);
		delete ((PyDict*)obj);
		return true;
	case PyTypeTuple:
		//printf("deleting tuple: %p\n", obj);
		delete ((PyTuple*)obj);
		return true;
	case PyTypeList:
		//printf("deleting list: %p\n", obj);
		delete ((PyList*)obj);
		return true;
	case PyTypeSubStream:
		//printf("deleting substream: %p\n", obj);
		delete ((PySubStream*)obj);
		return true;
	case PyTypeClass:
		//printf("deleting class: %p\n", obj);
		delete ((PyClass*)obj);
		return true;
	case PyTypePackedRow:
		//printf("deleting packedrow: %p\n", obj);
		delete ((PyPackedRow*)obj);
		return true;
	case PyTypeSubStruct:
		delete ((PySubStruct*)obj);
		return true;
	default:
		ASCENT_HARDWARE_BREAKPOINT;
		return false;
	}
}

#endif // _PYOBJECTDELETER_H