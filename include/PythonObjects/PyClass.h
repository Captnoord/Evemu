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
    Author:     Captnoord
*/

#ifndef _PYCLASS_H
#define _PYCLASS_H

#pragma pack(push,1)

/**
 * \class PyClassObject
 *
 * @brief a class object similar to the python one
 *
 * blaat I dono what I am doing....
 *
 * @author Captnoord
 * @date February 2009
 */
class PyClass
{
    uint8 mType;
    size_t mRefcnt;
    uint32 (PyClass::*mHash)();
public:
    uint8 gettype();
    void IncRef();
    void DecRef();
    uint32 hash();
public:
    PyClass();
    ~PyClass();
    bool setname(PyString* name);
    bool setbases(PyTuple* tuple);
    bool setdict(PyDict* dict);
    bool setDirList(PyList * list);
    bool setDirDict(PyDict * dict);

    PyString* getname();
    PyTuple* getbases();
    PyDict* getdict();
    PyList * getDirList();
    PyDict * getDirDict();
protected:
    PyDict      *mDict; /* A dictionary */
    PyString    *mName; /* A string */
    PyTuple     *mBases;/* A tuple of class objects */

    // derived object call info...
    PyList      *mDirivedCallList;
    PyDict      *mDirivedCallDict;
    uint32 _hash();
};

#pragma pack(pop)

#endif // _PYCLASS_H
