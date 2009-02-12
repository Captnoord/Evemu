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

#ifndef _MARSHALWRITESTREAM_H
#define _MARSHALWRITESTREAM_H

#define MWS_DEFAULT_BUFFER_SIZE 0x10000

/**
 * \class MarshalWriteStream
 *
 * @brief This class wraps us all the write stuff related to the marshal stream.
 *
 * 
 *
 * @author Captnoord
 * @date February 2009
 */
class MarshalWriteStream
{
public:
	MarshalWriteStream();
	~MarshalWriteStream();

	size_t size() {return mWriteIndex;}
	size_t allocatedsize() {return mAllocatedBufferSize;}
private:
	size_t mWriteIndex;
	size_t mAllocatedBufferSize;
	void* mBuffer;
};

#endif //_MARSHALWRITESTREAM_H