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

#ifndef _PYPACKEDROW_H
#define _PYPACKEDROW_H

#if 0
#pragma pack(push,1)

/**
 * \class PyPackedRow
 *
 * @brief this class represents a packet row... only for data parsing..
 *
 * @author Captnoord
 * @date February 2009
 */
class PyPackedRow
{
    uint8 mType;
    size_t mRefcnt;
    uint32 (PyPackedRow::*mHash)();
public:
    uint8 gettype();
    void IncRef();
    void DecRef();
    uint32 hash();
public:
    PyPackedRow();
    ~PyPackedRow();

    /* raw data containing the normal field data fields */
    size_t mRawFieldDataLen;        /* also known as 'blob_length' */
    uint8* mRawFieldData;           /* also known as 'blob' */

private:
    PyClass * mHeader;
    PyTuple* rawPayLoad;

    // experimental stuff
    std::map<uint32, PyChameleon*> flowers; // payload / virtual row set
public:
    typedef std::map<uint32, PyChameleon*>::iterator iterator;
    iterator begin();
    iterator end();
    size_t size();

    /* magical function */
    bool init(PyObject* header);

    bool setheader(PyClass * obj);
    bool setRawPayLoad(PyObject* tuple);
    PyObject* getRawPayLoad();

    bool addleaf(PyObject* leaf);

    PyClass*getheader();

    // local hash function
    uint32 _hash();
};

#pragma pack(pop)

#endif

#endif //_PYPACKEDROW_H
