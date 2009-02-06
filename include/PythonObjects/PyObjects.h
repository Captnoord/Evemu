#ifndef _PYOBJECTS_H
#define _PYOBJECTS_H

enum PyType
{
	PyTypeNone,
	PyTypeBool,
	PyTypeInt,
	PyTypeReal,
	PyTypeString,
	PyTypeUnicode,
	PyTypeDict,
	PyTypeTuple,
	PyTypeList,
	PyTypeSubStream,
	PyTypeClass,
	PyTypeDeleted, // must be last
};

class PyInt;
class PyDict;
class PyList;
class PyTuple;
class PyPackedRow;
class PyLong;
class PyFloat;
class PyBool;

/*
PyClass
PyObject
*/

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
	PyBaseNone() : type(PyTypeNone) {}
	~PyBaseNone(){type = PyTypeDeleted;}
	uint8 gettype(){return type;}
private:
	uint8 type;
};

// a single global static declaration of the PyNone object
static PyBaseNone PyNone;

/* universal object to represent the a single unidentified object. */
/* a universal type cast class to get the type of a object
 * the trick is that this class has the same mem map of all the objects...
 * WARNING... NOW THE REALLY TRICKY PART IS COMMING
 */
class PyObject
{
public:
	uint8 gettype(){return type;}
	~PyObject(){ASCENT_ASSERT(false);}
private:
	uint8 type;
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
	PyInt(int32 num, bool infinite = false);
	PyInt &operator=(const int num);
	~PyInt();

	uint8 gettype(){return type;}
	bool IsInfinite() {return isInfinite;}
private:
	uint8 type;
	int32 number;
	bool isInfinite;
};

/**
 * \class PyChameleon
 *
 * @brief the mythical lizard that can color to his surrounding.
 *
 * trys to be a universal objects for python types.
 *
 * @author Captnoord
 * @date January 2009
  */
class PyChameleon
{
public:
	PyChameleon();
	~PyChameleon();

	uint8 gettype();
	bool isempty();

	/**
	 * @brief 
	 *
	 * 
	 *
	 * @param[in]
	 * @param[out]
	 * @return
	 */
	PyChameleon &operator=(const char* const str);
	PyChameleon &operator=(const std::string &str);

	PyChameleon &operator=(const wchar_t* const str);
	PyChameleon &operator=(const std::wstring &str);

	PyChameleon &operator=(const uint32 number);
	PyChameleon &operator=(const int number);
	PyChameleon &operator=(const float number);
	PyChameleon &operator=(const PyDict* pointer);
	PyChameleon &operator=(const PyList* pointer);
	PyChameleon &operator=(const PyTuple* pointer);

	PyChameleon &operator=(const PyObject* pointer);
	PyChameleon &operator=(PyObject& pointer);

protected:

	void _settype(uint8 type);
	bool OnDelete();

private:
	uint8 mType;
	void* mPayload;
	bool mIsEmpty;		// indicates a non active PyChameleon
	bool mIsNew;		// just debug stuff..
};

/**
 * \class PyErrorChameleon
 *
 * @brief the PyErrorChameleon class is a VERY important class related to error detection and checking.
 *
 * 
 *
 * @author Captnoord.
 * @date January 2009
 * @note should this be a singleton???
 */
class PyErrorChameleon : public PyChameleon
{
public:
	PyErrorChameleon() : PyChameleon() {}
	
	template<typename T>
	PyErrorChameleon &operator=(T& str)
	{
		Log.Error("PyErrorChameleon", "Error we are used... thats not good... hmm.. there must have been something wrong...");
		return *this;
	}
};

static PyErrorChameleon PyErrorIterator;

class PyLong
{
public:
	PyLong(int64 & num);
	~PyLong();
	uint8 gettype();
	int64 getnumber();

private:
	uint8 type;
	int64 number;
};

class PyFloat
{
public:
	PyFloat();
	PyFloat(float num);
	PyFloat(double & num);
	~PyFloat();
	uint8 gettype();
private:
	uint8 type;
	double number;
};

class PyString
{
public:
	PyString();
	PyString(const char* str);
	PyString(const char* str, size_t len);
	PyString(std::string& str);
	~PyString();
	uint8 gettype();
	bool set(const char* str, size_t len);
	const char* content();
private:
	uint8 type;
	char* mStr;
};

class PyUnicodeUCS2
{
public:
	PyUnicodeUCS2();
	PyUnicodeUCS2(const wchar_t* str);
	PyUnicodeUCS2(const wchar_t* str, size_t len);
	PyUnicodeUCS2(std::wstring& str);
	~PyUnicodeUCS2();
	uint8 gettype();
	bool set(const char* str, size_t len);
	bool resize(size_t newsize);
	wchar_t * content();
private:
	uint8 type;
	wchar_t* mStr;
};

class PyBool
{
public:
	PyBool(bool check);
	~PyBool();
	uint8 gettype();
private:
	uint8 type;
	bool mCheck;
};

// we take 20 elements as the maximum amount of elements...
class PyTuple
{
#define PY_TUPLE_ELEMENT_MAX 1000
public:
	PyTuple();
	PyTuple(int elementCount);
	~PyTuple(); //should it contain delete all stuff?????
	uint8 gettype();

	PyChameleon &operator[](const int index);

	// returns the element count
	size_t size();
private:
	uint8 type;
	std::vector<PyChameleon> mTuple;
};

class PyList
{
public:
	PyList();
	PyList(int elementCount);
	~PyList();

	PyChameleon &operator[](const int index);

	uint8 gettype();
private:
	uint8 type;
	std::vector<PyChameleon> mList;
};

class PyDict
{
public:
	PyDict();
	~PyDict();
	uint8 gettype();

	PyChameleon &operator[](const char* keyName);

private:
	uint8 type;
	string_map<PyChameleon> mDict;
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
class PySubStream
{
public:

	PySubStream();
	PySubStream(uint8* data, size_t len);
	~PySubStream();

	uint8 gettype();
	uint8* content();
	bool set(uint8 * data, size_t len);

private:
	uint8 type;
	void* mData;
	size_t mLen;
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
	PyClass();
	~PyClass();

	uint8 gettype();
	bool setname(PyString* name);
	bool setbases(PyTuple* tuple);
	bool setdict(PyDict* dict);
	bool setDirList(PyList * list);
	bool setDirDict(PyDict * dict);

private:
	uint8 type;

	PyTuple		*mBases;/* A tuple of class objects */
	PyDict		*mDict;	/* A dictionary */
	PyString	*mName;	/* A string */
	

	// derived object call info...
	PyList		*mDirivedCallList;
	PyDict		*mDirivedCallDict;
};

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
	return new PyLong(number);
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
	return new PyInt(number);
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
	return new PyFloat(number);
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
	return new PyString(str, len);
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
static PyUnicodeUCS2* PyUnicodeUCS2_FromWideChar(const wchar_t* str, size_t len)
{
	return new PyUnicodeUCS2(str, len);
}

/**
 * @brief Python API cloning, converting a UTF8 string to a unicode UCS2 string.
 *
 * 
 *
 * @note this isn't as close to the native python implementation... we will see how far this will get us.
 * @param[in] str is the const char string that is fed into the function.
 * @param[in] len is the length of the char string that is converted by this function.
 * @return a freshly generated PyUnicodeUCS2 object from the char string
 * @note yea I know this function lacks 1 parameter, which is "const char *errors"
 */
static PyUnicodeUCS2* PyUnicodeUCS2_DecodeUTF8(const char* str, size_t len)
{
	// implementation like these sucks....
	size_t nlen = len;//(strlen(str)+1) * 2;
	
	PyUnicodeUCS2 * retstr = new PyUnicodeUCS2();
	retstr->resize(nlen);
	size_t newSize = mbstowcs(retstr->content(), str, nlen);
	retstr->content()[nlen] = '\0';

	if (newSize != nlen)
	{
		SafeDelete(retstr);
		return NULL;
	}

	return retstr;
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
static PyTuple* PyTuple_New(int elementCount)
{
	return new PyTuple(elementCount);
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
	return new PyList(elementCount);
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
	return new PyDict();
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

	int64 tempNumber = _PyLong_AsInt64(number);

	uint8 integerSize = 0;
#define DoIntegerSizeCheck(x) if (((uint8*)&tempNumber)[x] != 0) integerSize = x + 1;
	DoIntegerSizeCheck(0);
	DoIntegerSizeCheck(1);
	DoIntegerSizeCheck(2);
	DoIntegerSizeCheck(3);
	DoIntegerSizeCheck(4);
	DoIntegerSizeCheck(5);
	DoIntegerSizeCheck(6);
	DoIntegerSizeCheck(7);
#undef  DoIntegerSizeCheck

	if (*size < integerSize)
		return false;

	// just copy it into a temp var, its slower, but I don't want to open up the class.
	
	uint8* pCpy = (uint8*)ASCENT_MEMCPY((void*)buffer, (void*)&tempNumber, integerSize);

	ASCENT_HARDWARE_BREAKPOINT; // for debug purposes
		
	if (pCpy != buffer)
	{
		ASCENT_HARDWARE_BREAKPOINT; // for debug purposes
		return false;
	}
	
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
 * @note I feel so fucked up, mixing unmarshal code with the python api clone.
 */
static PyLong* _ByteArray_AsPyLong(const uint8* buffer, size_t size)
{
	if (buffer == NULL)
		return false;

	// sanity checks
	if (size == 0 || size > 8)
		return 0;

	//uint8 len = buffer[0];

	// I consider this as a error
	//if (len == 0 || len > 8)
	//	return NULL;

	int64 intval = (1LL << (8 * size)) - 1;
	intval &= *((const uint64 *) buffer);

	return new PyLong(intval);
}

static int64 _PyLong_AsInt64(PyLong& number)
{
	return number.getnumber();
}

#endif //_PYOBJECTS_H
