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

#ifndef UNMARSHAL_REFERENCE_MAP_H
#define UNMARSHAL_REFERENCE_MAP_H

#include "Common.h"

/**
 * MarshalReferenceMap.h only implement the UnmarshalReferenceMap, the Marshal reference tech
 * isn't researched/implemented for the moment. a cpp file should be created from the code in
 * this header but for the moment this will do.
 * @note because we don't own the pointer / own the memory of the referenced objects. Its unclear when the refered object is deleted so
 * we have to eighter keep the Object here ( own the pointer ) or make sure that the referenced object know he is a referenced object so
 * it isn't accidentally deleted.
 */

/**
 * The UnmarshalReferenceMap class keeps track of referenced objects within
 * a marshal stream.
 * 
 * @author Captnoord
 */
class UnmarshalReferenceMap
{
public:

	UnmarshalReferenceMap() : expectedObjectsCount(0), storedObjectCount(0), storeObjectIndex(0)
	{
		mReferenceObjects = NULL;
	}

	/**
	 * Constructor that sets the expected object count of referenced objects.
	 * @param objectCount is the number of expected referenced objects within the marshal stream, its also known in the client as as MapCount.
	 */
	UnmarshalReferenceMap(const uint32 objectCount) : expectedObjectsCount(objectCount), storedObjectCount(0), storeObjectIndex(0)
	{
		assert(expectedObjectsCount < 0x100); // kinda normal..... 256 referenced objects otherwise crash
		mReferenceObjects = new void*[expectedObjectsCount];
	}

	/**
	 * Get a stored referenced Object.
	 * @param location is the index number of the referenced object.
	 * @return A referenced PyRep base Object.
	 */
	bool GetStoredObject(uint32 location, void** object)
	{
		if (location == 0 || location > storedObjectCount )
			return false;

		*object = mReferenceObjects[location-1];
		return true;
	}

	/**
	 * Stores a referenced Object.
	 * @param object is the object that is marked as a object that has many references.
	 */
	bool StoreReferencedObject(void* object)
	{
		if (storeObjectIndex < expectedObjectsCount)
			return false;
		
		mReferenceObjects[storeObjectIndex] = object;
		storeObjectIndex++;

		storedObjectCount = storeObjectIndex;
		return true;
	}

	/**
	 * Get the current stored objects.
	 * @return A unsigned integer that represents the amount of referenced Objects stored.
	 */
	uint32 GetStoredCount()
	{
		return storedObjectCount;
	}

	/**
	 * Get the expected referenced object count.
	 * @return A unsigned integer that represents the expected object count.
	 */
	uint32 GetMaxStoredCount()
	{
		return expectedObjectsCount;
	}

	/**
	 * @brief Set the amount of shared objects
	 *
	 * 
	 *
	 * @param[in]
	 */
	void SetSharedObjectCount(uint32 sharedObjectCount)
	{
		// this shouldn't happen and if it does.. this prop will generate a crash...
		if(mReferenceObjects != NULL)
			SafeDeleteArray(mReferenceObjects);

		mReferenceObjects = new void*[expectedObjectsCount];
		expectedObjectsCount = sharedObjectCount;
	}

protected:
	/** 
	 * keeps track of the amount of objects that are actually stored
	 */
	uint32 storedObjectCount;

	/**
	 * keeps track of the store index
	 */
	uint32 storeObjectIndex;

	/**
	 * max amount of objects that are referenced objects in this stream
	 * this value is supplied by the client (regarding the 'unmarshal' process).
	 */
	uint32 expectedObjectsCount;

	/**
	 * pointer container to keep track of the pointers...
	 * and of course: "we do not own the pointers in this list"
	 */
	void** mReferenceObjects;
};

#endif//UNMARSHAL_REFERENCE_MAP_H
