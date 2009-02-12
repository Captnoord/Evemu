/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2009 Ascent Team <http://www.ascentemu.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author:		Captnoord
 *
 */

#ifndef _STRINGMAP_H
#define _STRINGMAP_H

/**
* \class string_map
*
* @brief a templated class to wrap around the tree + hash methods.
*
* This class is a wrapper to easily create a string driven hash map.
* I'm not trying to reinvent the hash map or the unorderd_map from std,
* but I'm aiming for a generic string driven map.
*
* @author Captnoord.
* @date January 2009
*/
template<class T>
class string_map
{
private:
	typedef std::map<uint32, T>					StringMap;
	typedef typename StringMap::iterator		StringMapItr;
	typedef typename StringMap::const_iterator	StringMapConstItr;
public:
	/* 2 generic string hash functions */
	enum hashfunc
	{
		djb2,
		sdbm,
	};

	/**
	* @brief default constructor
	*
	* 
	*
	* @param[in] hashProc is a string_map only enum to select the string
	* hash algorithm.
	*/
	string_map(hashfunc hashProc = djb2)
	{
		if(hashProc == djb2)
			hash = Utils::Hash::djb2_hash;
		else
			hash = Utils::Hash::sdbm_hash;
	}

	~string_map()
	{
		hash = NULL;
	}

	/**
	* @brief insert a element with a string key name.
	*
	* 
	*
	* @param[in] str is the NULL terminated string that is used for storing the item.
	* @param[in] item the item that needs to be stored under the hash of the string.
	* @return will return false when a hash collision accured
	*/
	bool insert(const char *str, T item)
	{
		uint32 _hash = (*hash)(str);
		// check if there is a collision
		if(_hashCollision(_hash)== true)
		{
			sLog.Error("string_map hash function has detected a hash collision, please change the hash function");
			return false;
		}

		// if there's not insert it.
		mContainer.insert(std::make_pair(_hash, item));
		return true;
	}

	/**
	* @brief finds a item with a string.
	*
	* 
	*
	* @param[in] str is the name of the item that is stored.
	* @return if found it returns the item. If not it crash related to the type of T.
	* @note smart people will have noticed that I return NULL if a item isn't found. This is a bug NOT a feature.
	* @todo this the return stuff.
	*/
	T find(const char* str)
	{
		uint32 _hash = (*hash)(str);
		StringMapItr Itr = mContainer.find(_hash);
		if (Itr != mContainer.end())
		{
			return Itr->second;
		}
		return NULL;
	}
	/**
	* @brief [] operator.
	*
	* 
	*
	* @param[in] the name of the item that is eghter to be found or to be inserted.
	* @return returns the reference of the found object or a new entry conform the hashed string.
	*/
	T &operator[](const char* str) {
		uint32 _hash = (*hash)(str);
		return mContainer[_hash];
	}

	/**
	* @brief size
	*
	* nothing special its just a size function.
	*
	* @return returns the amount of stored items in the container.
	*/
	size_t size() {return mContainer.size();}

	/**
	* @brief clear.
	*
	* nothing special its just a clear function.
	*
	*/
	void clear() {mContainer.clear();}

	/**
	* @brief retrieves the begin iterator of the item container.
	*
	* nothing special its just a begin function.
	*
	* @return the begin iterator of the item container.
	*/
	StringMapItr begin() {return mContainer.begin();}

	/**
	* @brief retrieves the end iterator of the item container.
	*
	* nothing special its just a end function.
	*
	* @return the end iterator of the item container.
	*/
	StringMapItr end() {return mContainer.end();}

private:
	/**
	* @brief small function to detect hash collisions
	*
	* this function tries to detect hash collisions, returning a true if it happened.
	* the idea about it is quite simple, but mainly because how this class is designed,
	* its not as efficient as it can be. Mainly because you can compare hashes but you
	* can't compare the strings that generate them. So its possible that if we are trying
	* to insert 2 items with the same item name it will report it as a hash collision.
	* Clearly as we all know our logic it isn't so, we need to redesign it.
	*
	* @param[in] hash the freshly calculated hash value that needs to be checked.
	* @return true if there is a collision and false if there isn't.
	*/
	bool _hashCollision(uint32 hash)
	{
		// if there isn't anything in the container there can't be a hash collision.
		if (mContainer.size() == 0)
			return false;

		StringMap::iterator Itr = mContainer.find(hash);
		if (Itr != mContainer.end())
		{
			return true;
		}
		return false;
	}

private:

	StringMap mContainer;

	// function pointer of the hash function.
	uint32 (*hash)(const char*);
};

#endif //_STRINGMAP_H
