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

#ifndef _OBJECTCACHING_H
#define _OBJECTCACHING_H

/** CachedObject
 * wrapper around a cached object.
 * @note if we derive from PyClass we are not adding extra variable. This is a design choice and unless its clear 
 * we are required to add extra fields. We add optional fields to PyClass.
 */
class CachedObject : public PyClass
{
public:
	// this constructor shouldn't be used for now
#if 0
	CachedObject(bool shared, const char* objectID, PyObject* objectVersion, PyInt * nodeID, PyObject *object, PyObject * /*pickle*/, bool compressed ) : PyClass()
	{
		ASCENT_HARDWARE_BREAKPOINT;
		ASCENT_ASSERT(mBases);
		ASCENT_ASSERT(mName);
		
		mName = new PyString("objectCaching.CachedObject");
		mBases = new PyTuple(7);
		PyTuple & bases = *mBases;
		bases[0] = objectVersion;		//object version.
		bases[1] = new PyBaseNone;		//the pickle
		bases[2] = nodeID;				//nodeID
		bases[3] = shared;				//shared
		bases[4] = object;				//object
		bases[5] = compressed;			//compressed
		bases[6] = objectID;			//objectID
	}
#endif
	
	CachedObject(bool shared, const char* objectID, PyObject *object, PyObject* objectVersion = NULL );

	/**
	 * this function generates a object version tuple.
	 *
	 * the first item of the tuple is hard coded for now.
	 *
	 * @param[in]
	 * @param[out]
	 * @return
	 * @todo change the first item of the tuple into a timestamp of some sort.
	 * @todo change the second items sha salt into something every server owner can change.
	 */
	PyObject* GenerateObjectVersion(WriteStream& stream);
};

/** ObjectCachingSvc
 * this class is a manager class that keeps track of and contains the cached objects.
 * singleton
 */
class ObjectCachingSvc : public Singleton<ObjectCachingSvc>
{
public:

	/**
	 * GetCachedObject does how its named. It gets a cached object from a objectID.
	 */
	PyObject* GetCachedObject(PyObject * objectID /*PyString*/);

	/**
	 * GetCachedObjectVersion returns the version of the object
	 * equal to the objectID.
	 */
	PyObject* GetCachedObjectVersion(PyObject * objectID);

	/*
		Acquires a cached object, or returns None if no such object can be found.
		If required, will acquire it from the next node.  The object has
		an ID of objectID, a version of objectVersion, and nodeID is the sol
		server node that is known to have the object, if all else fails.
		@note atm this is a simple function.
	*/
	PyObject* GetCachableObject(PyObject* shared /* PyBool */, PyObject * objectID /* PyString */, PyObject * objectVersion /* PyTuple(2) */, PyObject * nodeID /* PyInt */)
	{
		PyObject* searchResult = mCachedObjects.get_item(objectID);
		return searchResult;
	}

	/**
	 * cache a object.
	 */
	//void CacheObject(CachedObject* object) {}

	void CacheObject(PyObject* object, const char * objectID );

private:
	PyDict mCachedObjects; // keeps track of the cached objects
	PyBaseNone PyNone;
};

#endif // _OBJECTCACHING_H

#define sObjectCachingSvc ObjectCachingSvc::getSingleton()
