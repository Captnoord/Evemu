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
class PyClass : public PyObject
{
public:
    PyClass();
    ~PyClass(); // this doesn't do shit... bleh...
    void destruct() {};

    uint32 hash();

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

    // optional for normal classes but required for InstanceClasses
    PyClass* New(){return NULL;}
    int UpdateDict(PyObject* bases){return -1;}

    // important for the inner workings of this all
    bool init(PyObject* state) {ASCENT_HARDWARE_BREAKPOINT;}
    PyTuple* GetState() {return NULL;};
protected:
    PyTuple		*mBases;/* A tuple of class objects */
    PyDict		*mDict;	/* A dictionary, used to store 'self.xxxx' variable info */
    PyString	*mName;	/* A string */

    // instance object info...
    PyDict		*mInDict;
    PyList		*mWeakRefList;

    ternaryfunc tp_call;
};


#endif // _PYCLASS_H
