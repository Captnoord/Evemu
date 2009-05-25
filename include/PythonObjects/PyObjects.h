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

#ifndef _PYOBJECTS_H
#define _PYOBJECTS_H

enum PyType
{
	PyTypeNone,			//0
	PyTypeBool,			//1
	PyTypeInt,			//2
	PyTypeLong,			//3
	PyTypeReal,			//4
	PyTypeString,		//5
	PyTypeUnicode,		//6
	PyTypeDict,			//7
	PyTypeTuple,		//8
	PyTypeList,			//9
	PyTypeSubStream,	//10
	PyTypeClass,		//11
	PyTypeModule,		//12
	PyTypePackedRow,	//13
	PyTypeSubStruct,	//14
	PyTypeDeleted,		//15 // must be last, and can max be 16...
};

// object hash function
//#define hashfunc(x) uint32 (*x)(void);
typedef uint32 (*hashfunc)(void);

class PyInt;
class PyDict;
class PyList;
class PyTuple;
class PyPackedRow;
class PyLong;
class PyFloat;
class PyBool;
class PyClass;
class PyObject;
class PyChameleon;
class PyUnicodeUCS2;

#pragma pack(push,1)

/**
 * \class PyBaseNone
 *
 * @brief a initial class for a nothing object
 *
 * nothing special.
 *
 * @author Captnoord.
 * @date January 2009
 */
class PyBaseNone
{
public:
	uint8 gettype();
	void IncRef();
	void DecRef();
	uint32 hash();
private:
	uint8 mType;
	size_t mRefcnt;
	uint32 (PyBaseNone::*mHash)();
public:
	PyBaseNone();
	~PyBaseNone();
	uint32 _hash();
};

// a single global static declaration of the PyNone object
//static PyBaseNone PyNone;

/* universal object to represent the a single unidentified object. */
/* a universal type cast class to get the type of a object
 * the trick is that this class has the same mem map of all the objects...
 * WARNING... NOW THE REALLY TRICKY PART IS COMMING
 */
class PyObject
{
public:
	uint8 gettype();
	void IncRef();
	void DecRef();
	uint32 hash();
private:
	uint8 mType;
	size_t mRefcnt;
	uint32 (PyObject::*mHash)();
public:
	PyObject();
	~PyObject();
};

/**
 * \class PyInt
 *
 * @brief a object that represents a integer
 *
 * nothing special
 *
 * @author Captnoord.
 * @date January 2009
 */
class PyInt
{
public:
	uint8 gettype();
	void IncRef();
	void DecRef();
	
	/** simple hash function, atm this one is pretty weak.
	 */
	uint32 hash();
private:
	uint8 mType;
	size_t mRefcnt;
	uint32 (PyInt::*mHash)();
public:
	explicit PyInt(int32 num );
	PyInt &operator = (const int num);
	~PyInt();
	int32 GetValue();
private:
	int32 mNumber;
	uint32 _hash();
};

class PyLong
{
public:
	uint8 gettype();
	void IncRef();
	void DecRef();
	uint32 hash();
private:
	uint8 mType;
	size_t mRefcnt;
	uint32 (PyLong::*mHash)();
public:
	explicit PyLong(int64 & num);
	explicit PyLong(uint64 & num);
	~PyLong();
	int64 GetValue();
private:
	int64 mNumber;
	uint32 _hash();
};

class PyFloat
{
public:
	uint8 gettype();
	void IncRef();
	void DecRef();
	uint32 hash();
private:
	uint8 mType;
	size_t mRefcnt;
	uint32 (PyFloat::*mHash)();
public:
	explicit PyFloat();
	explicit PyFloat(float num);
	explicit PyFloat(double num);
	~PyFloat();
	double GetValue();
private:
	double mNumber;
	uint32 _hash();
};

class PyString
{
public:
	uint8 gettype();
	void IncRef();
	void DecRef();
	uint32 hash();
private:
	uint8 mType;
	size_t mRefcnt;
	uint32 (PyString::*mHash)();
public:
	explicit PyString();
	explicit PyString(const char* str);
	explicit PyString(const char* str, size_t len);
	explicit PyString(std::string& str);
	~PyString();
	bool set(const char* str, size_t len);
	const char* content();
	const size_t length();

	/**
	 * \brief appends a string to the end of the PyString
	 *
	 * 
	 *
	 * @param[in] str the std string that should be appended to the PyString.
	 * @return this function should return the "this" object.
	 */
	PyString& append ( const std::string& str );

	/**
	 * \brief appends a string to the end of the PyString
	 *
	 * 
	 *
	 * @param[in] str the std string that should be appended to the PyString.
	 * @param[in] pos where the string should be appended.
	 * @param[in] n how much of the string should be appended.
	 * @return this function should return the "this" object.
	 */
	PyString& append ( const std::string& str, size_t pos, size_t n );

	bool resize(size_t size);

	bool operator==(const char *str1);
	PyString &operator=(const char *str);
	char &operator[](const int index);
private:

	char* mStr;
	size_t mStrLen;
	uint32 mHashValue;
	uint32 _hash();
};

class PyBool
{
public:
	uint8 gettype();
	void IncRef();
	void DecRef();
	uint32 hash();
private:
	uint8 mType;
	size_t mRefcnt;
	uint32 (PyBool::*mHash)();
public:
	explicit PyBool(bool check);
	~PyBool();
	bool operator==(const bool check);
private:
	bool mCheck;
	uint32 _hash();
};

/**
 * \class PyTuple
 *
 * @brief PyTuple is one of the most used objects within python. Its a object to store objects in.
 *
 * http://en.wikipedia.org/wiki/Tuple
 *
 * @note one of the design choices was to use a vector to store the pointers.
 * @note this at first hand only cause big slowdowns when allocating a big amount of items.
 * @author Captnoord
 * @date March 2009
 */
class PyTuple
{
/* limit the tuple to 1000 items */
#define PY_TUPLE_ELEMENT_MAX 1000
public:
	uint8 gettype();
	void IncRef();
	void DecRef();
	uint32 hash();
private:
	uint8 mType;
	size_t mRefcnt;
	uint32 (PyTuple::*mHash)();
public:
	explicit PyTuple();
	explicit PyTuple(size_t elementCount);
	~PyTuple();

	/**
	 * \brief operator overload for easy object access and storage
	 *
	 * nothing much to tell about this function, it returns a PyChameleon object reference.
	 *
	 * @param[in] index is the location of the required object.
	 * @return always returns a PyChameleon object even if there isn't a object stored (so it can be used to store objects).
	 */
	PyChameleon &operator[](const int index);
	PyObject* GetItem(const int index);

	int GetInt(const int index);
	double GetFloat(const int index);
	/**
	 * Slow GetString function
	 */
	std::string GetString(const int index);

	/**
	 * get item as std::string
	 */
	bool GetString(const int index, std::string & str);

	template<typename T>
	void set_item(const int index, T * object);

	void set_str(const int index, const char* str);
	void set_str(const int index, const char* str, const size_t len);

	/**
	 * \brief a VA function for getting multiple object from a tuple.
	 *
	 * a VA function for getting multiple object from a tuple.
	 *
	 * @param[in] format is the VA string containing the expected object types.
	 * @param[out] ... this VA field contains the pointers to the VA items.
	 * @return the number of 'scanned' objects and 0 of a error has happened.
	 */
	int scanf(const char * format, ...);

	// returns the element count
	size_t size();

	// clears the tuple from all objects
	void clear();

	bool resize(size_t elementCount);
private:
	typedef std::vector<PyChameleon*> TupleVector;
public:
	typedef TupleVector::iterator iterator;
	typedef TupleVector::const_iterator const_iterator;
	iterator begin() {return mTuple.begin();}
	iterator end() {return mTuple.end();}
private:
	TupleVector mTuple;

	// tuple hash function
	uint32 _hash();
};

class PyList
{
public:
	uint8 gettype();
	void IncRef();
	void DecRef();
	uint32 hash();
private:
	uint8 mType;
	size_t mRefcnt;
	uint32 (PyList::*mHash)();
public:
	explicit PyList();
	explicit PyList(int elementCount);
	~PyList();
	PyChameleon &operator[](const int index);
	size_t size();
	bool add(PyObject* obj);
private:
	std::vector<PyChameleon*> mList;
	typedef std::vector<PyChameleon*>::iterator iterator;
	uint32 _hash();
};

/* dictionary entry */
struct PyDictEntry
{
	/* uint32 hashvalue; */
	PyObject* key;
	PyObject* obj;
};

class PyDict
{
public:
	uint8 gettype();
	void IncRef();
	void DecRef();
	uint32 hash();
private:
	uint8 mType;
	size_t mRefcnt;
	uint32 (PyDict::*mHash)();
public:
	PyDict();
	~PyDict();
	PyChameleon operator[](const char* keyName);

	/**
	 * \brief set_item adds or sets a database entry.
	 *
	 * PyDict::set_item handles the adding and setting of object in
	 * mapped and non mapped dictionary's's.
	 *
	 * @param[in]
	 * @param[out]
	 * @return
	 */
	template<typename T>
	bool set_item(const char* key_name, T* obj)
	{
		if (key_name == NULL || obj == NULL)
		{
			/* something broke so debug it */
			ASCENT_HARDWARE_BREAKPOINT;
			return false;
		}

		if (mMappingMode == true)
		{
			/* create a new dictionary entry */
			PyDictEntry * entry = new PyDictEntry;
			entry->key = (PyObject*)new PyString(key_name);
			entry->obj = (PyObject*)obj;
			mDict.insert(std::make_pair(mMappingIndex++, entry));
			obj->IncRef();
		}
		else
		{
			/* IMPLEMENT THIS ONE..... */
			ASCENT_HARDWARE_BREAKPOINT;
			/* do the same as the set_item function that takes a PyObject as a key, but do some tricks with it */

			size_t str_len = strlen(key_name);

			/* test this and check these */
			uint32 hsh = Utils::Hash::sdbm_hash(key_name, (int)str_len);
			uint32 hsh1 = Utils::Hash::sdbm_hash(key_name);
		}

		return true;
	}

	bool set_item(PyObject* key, PyObject* obj);
	PyObject* get_item(PyObject* key);
	PyObject* get_item(const char* key_name);

	bool scanf(const char * keyName, const char * format, ...);

	bool get_buffer(const char * keyName, char* dst, size_t dst_len);
	bool set_int(const char * keyName, int number);
	bool set_double(const char * keyName, double number);
	bool set_str(const char * keyName, const char* str);
	bool set_bool(const char * keyName, bool check);

	size_t size();

	/* atm only changing static mapped dict to hashed dict */
	void update();

private:

	typedef std::map<uint32, PyDictEntry*>	DictMap;
	typedef DictMap::iterator				DictMapItr;
public:
	typedef DictMap::iterator				iterator;
	iterator begin();
	iterator end();
private:

	DictMap mDict;
	/** mMappingMode makes it possible to simply store the objects
	 */
	bool	mMappingMode;
	uint32	mMappingIndex;

	uint32 _hash();
};

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
public:
	uint8 gettype();
	void IncRef();
	void DecRef();
	uint32 hash();
private:
	uint8 mType;
	size_t mRefcnt;
	uint32 (PySubStream::*mHash)();
public:
	PySubStream();
	PySubStream(uint8* data, size_t len);
	~PySubStream();
	uint8* content();
	size_t size();
	bool set(uint8 * data, size_t len);
private:
	void* mData;
	size_t mLen;
	uint32 _hash();
};

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
public:
	uint8 gettype();
	void IncRef();
	void DecRef();
	uint32 hash();
private:
	uint8 mType;
	size_t mRefcnt;
	uint32 (PyClass::*mHash)();
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
//private:
protected:
	PyTuple		*mBases;/* A tuple of class objects */
	PyDict		*mDict;	/* A dictionary */
	PyString	*mName;	/* A string */

	// derived object call info...
	PyList		*mDirivedCallList;
	PyDict		*mDirivedCallDict;
	uint32 _hash();
};

/**
* \class PyInstance
*
* @brief
*
* blaat I dono what I am doing.... part 2. :D
*
* @author Captnoord
* @date March 2009
*/
class PyInstance
{
public:
	uint8 gettype();
	void IncRef();
	void DecRef();
	uint32 hash();
private:
	uint8 mType;
	size_t mRefcnt;
	uint32 (PyInstance::*mHash)();
public:
	PyInstance();
	~PyInstance();
private:
	PyClass* mClass;
	PyDict* mDict;
	/* PyList weak reference list */
	uint32 _hash();
};

/**
 * \class PyModule
 *
 * @brief
 *
 * blaat I dono what I am doing.... part 2. :D
 *
 * @author Captnoord
 * @date March 2009
 */
class PyModule
{
public:
	uint8 gettype();
	void IncRef();
	void DecRef();
	uint32 hash();
private:
	uint8 mType;
	size_t mRefcnt;
	uint32 (PyModule::*mHash)();
public:
	PyModule();
	~PyModule();
	PyString* mModuleName;
private:
	uint32 _hash();
};



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
public:
	uint8 gettype();
	void IncRef();
	void DecRef();
	uint32 hash();
private:
	uint8 mType;
	size_t mRefcnt;
	uint32 (PyPackedRow::*mHash)();

public:
	PyPackedRow();
	~PyPackedRow();

	/* raw data containing the normal field data fields */
	uint8* mRawFieldData;			/* also known as 'blob' */
	size_t mRawFieldDataLen;		/* also known as 'blob_length' */

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

class PySubStruct
{
public:
	uint8 gettype();
	void IncRef();
	void DecRef();
	uint32 hash();
private:
	uint8 mType;
	size_t mRefcnt;
	uint32 (PySubStruct::*mHash)();
public:
	PySubStruct();
	~PySubStruct();

	PyObject * getPyObject();
	bool setPyObject(PyObject* obj);
	uint32 _hash();

private:
	PyObject* payload;
};

#pragma pack(pop)

/************************************************************************/
/* Small portion of the Python API so we are able to handle various    */
/* tasks better and a bit cleaner.                                      */
/* @note we don't aim to mimic the python API perfectly.                */
/************************************************************************/
static int64 _PyLong_AsInt64(PyLong& number);

/**
 * @brief 
 *
 * 
 *
 * @param[in] number is the number that needs to be converted into a object
 * @return the converted object.
 * @todo we should create a unified integer object that can contain both a 64 bits value or a 32 bits value.
 */
static PyLong* PyLong_FromLongLong(int64 & number)
{
	PyLong * num = new PyLong(number);
	return num;
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
static PyInt* PyInt_FromLong(int32 number)
{
	PyInt * num = new PyInt(number);
	return num;
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
static PyFloat* PyFloat_FromDouble(double & number)
{
	PyFloat * num = new PyFloat(number);
	return num;
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
static PyString* PyString_FromStringAndSize(const char* str, size_t len)
{
	PyString * string = new PyString(str, len);
	return string;
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
static PyTuple* PyTuple_New(size_t elementCount)
{
	PyTuple * tuple = new PyTuple(elementCount);
	return tuple;
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
static PyList* PyList_New(int elementCount)
{
	PyList * list = new PyList(elementCount);
	return list;
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
static PyDict* PyDict_New()
{
	PyDict * dict = new PyDict();
	return dict;
}

/**
 * @brief this function converts a PyLong into a ByteArray.
 *
 * This function converts a PyLong into a ByteArray, its used by the marshal stream class.
 *
 * @param[in] number is the PyLong object that represents the number that is changed into a ByteArray
 * @param[in][out] buffer allocated outside the function
 * @param[in] size the size of the buffer allocated.
 * @param[out] size the size of the allocated buffer used.
 * @return true if no error has happened and false if there's something wrong with the conversion.
 */
static bool _PyLong_AsByteArray(PyLong& number, const uint8* buffer, size_t* size)
{
	if (buffer == NULL)
		return false;

	if (size == NULL)
		return false;

	if (*size == 0)
		return 0;

	int64 num = _PyLong_AsInt64(number);
	
	uint8 integerSize = 0;
	uint8* num_buff = (uint8*)&num;
	for (int i = 7; i > 0; i--)
	{
		if (num_buff[i] != 0)
		{
			integerSize = i;
			break;
		}
	}

	if (*size < integerSize)
		return false;

	ASCENT_MEMCPY((void*)buffer, (void*)&num, integerSize);

	*size = integerSize;

	return true;
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
static PyLong* _ByteArray_AsPyLong(const uint8* buffer, size_t size)
{
	/* sanity checks */
	if (buffer == NULL)
		return false;
	
	if (size == 0 || size > 8)
		return 0;

	int64 intval = (1LL << (8 * size)) - 1;
	intval &= *((const uint64 *) buffer);

	PyLong * num = new PyLong(intval);
	return num;
}

static PyLong* PyLong_FromLong(int64 number)
{
	return new PyLong(number);
}

static bool _PyLong_AsInt64(PyObject* number, int64& dst_num)
{
	if (number == NULL)
		return false;

	if (number->gettype() != PyTypeLong)
		return false;
	
	dst_num = ((PyLong*)number)->GetValue();
	return true;
}

static int64 _PyLong_AsInt64(PyLong& number)
{
	return number.GetValue();
}

// a wrapper for hashing "objects"
uint32 PyObject_Hash(PyObject* obj);

#endif //_PYOBJECTS_H
