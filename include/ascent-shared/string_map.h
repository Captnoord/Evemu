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
	 * @brief insert a element with a string keyname.
	 *
	 * 
	 *
	 * @param[in]
	 */
	void insert(const char *str, T item)
	{
		uint32 _hash = (*hash)(str);
		// check if there is a collision
		if(_hashCollision(_hash)== true)
		{
			sLog.Error("string_map hash function has detected a hash collision, please change the hash function");
			return;
		}

		// if there's not insert it.
		mContainer.insert(std::make_pair(_hash, item));
	}

	/* a find function with a bug:P */

	/**
	 * @brief 
	 *
	 * 
	 *
	 * @param[in]
	 * @return
	 */
	T find(const char* str)
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
	 * @param[in]
	 * @param[out]
	 * @return
	 */
	T &operator[](const char* str) {
		uint32 _hash = (*hash)(str);
		return mContainer[_hash];
	}

	/**
	 * @brief 
	 *
	 * 
	 *
	 * @return
	 */
	size_t size() {return mContainer.size();}

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
	typedef std::map<uint32, T>		_StringMap;
	//typedef std::map<uint32, T>::iterator	_StringMapItr;

	_StringMap mContainer;
	// function pointer
	uint32 (*hash)(const char*);
};

#endif //_STRINGMAP_H

