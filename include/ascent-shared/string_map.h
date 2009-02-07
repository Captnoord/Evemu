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
public:
	/* 2 generic string hash functions */
	enum hashfunc
	{
		djb2,
		sdbm,
	};
	
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
	 * @param[in] str is the to hash string.
	 * @param[in] element is the element that needs to be stored under the hash.
	 */
	void insert(const char *str, T element)
	{
		uint32 _hash = (*hash)(str);
		// check if there is a collision
		if(_hashCollision(_hash)== true)
		{
			sLog.Error("string_map hash function has detected a hash collision, please change the hash function");
			return;
		}

		// if there's not insert it.
		mContainer.insert(std::make_pair(_hash, element));
	}

	/**
	 * @brief searches the hash table for a key name.
	 *
	 * 
	 *
	 * @param[in] str the key name of the element that we are searching.
	 * @return we return the element or NULL if we haven't found anything.
	 */
	T &find(const char* str)
	{
		uint32 _hash = (*hash)(str);
		_StringMap::iterator Itr = mContainer.find(_hash);
		if (Itr != mContainer.end())
		{
			return Itr->second;
		}
		return NULL;
	}

	/**
	 * @brief 
	 *
	 * 
	 *
	 * @param[in] str is the to hash string.
	 * @return a found element or a new element.
	 */
	T &operator[](const char* str) {
		uint32 _hash = (*hash)(str);
		return mContainer[_hash];
	}

	/**
	 * @brief returns the size of the string map.
	 *
	 * 
	 *
	 * @return the amount of elements in the map.
	 */
	size_t size() {return mContainer.size();}

	/**
	 * @brief 
	 *
	 * 
	 *
	 * @param[in]
	 * @param[out]
	 * @return
	 */
	void clear() {mContainer.clear();}

private:
	/**
	 * @brief small function to detect hash collisions
	 *
	 * 
	 *
	 * @param[in] hash the freshly calculated hash value that needs to be checked.
	 * @return true if there is a collision and false if there isn't.
	 */
	bool _hashCollision(uint32 hash)
	{
		// if there isn't anything in the container there can't be a hash collision.
		if (mContainer.size() == 0)
			return false;

		_StringMap::iterator Itr = mContainer.find(hash);
		if (Itr != mContainer.end())
		{
			return true;
		}
		return false;
	}

private:
	typedef std::map<uint32, T>			_StringMap;
	
	_StringMap mContainer;
	// function pointer
	uint32 (*hash)(const char*);
};

#endif //_STRINGMAP_H
