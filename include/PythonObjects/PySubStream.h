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

#ifndef _PYSUBSTREAM_H
#define _PYSUBSTREAM_H

#pragma pack(push,1)

/**
 * \class PySubStream
 *
 * @brief totally unclear if its needed
 *
 * this class only keeps track of the pointer to the sub stream.
 * we also do not own external pointers so we do a memcpy
 *
 * @author Captnoord
 * @date February 2009
 */

//static int SubStreamCounter = 0;
class PySubStream
{
    uint8 mType;
    size_t mRefcnt;
    uint32 (PySubStream::*mHash)();
    size_t mLen;
    void* mData;
public:
    uint8 gettype();
    void IncRef();
    void DecRef();
    uint32 hash();
public:
    PySubStream();
    PySubStream(uint8* data, size_t len);
    ~PySubStream();
    uint8* content();
    size_t size();
    bool set(uint8 * data, size_t len);
private:
    uint32 _hash();
};

#pragma pack(pop)

#endif //_PYSUBSTREAM_H
