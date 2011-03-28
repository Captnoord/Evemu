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
class PySubStream;
class PyString;

typedef PyObject * (*ternaryfunc)(PyObject *, PyObject *, PyObject *);

//#pragma pack(push,1)

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
    PyObject(PyType type);
    virtual ~PyObject();
    virtual uint32 hash() = 0; // pure virtual because different objects have different hash functions...
    virtual size_t size() {

        // debug assert to make sure its only called by objects that really have size shit.....
        switch(mType)
        {
        case PyTypeNone:
        case PyTypeBool:
        case PyTypeInt:
        case PyTypeLong:
        case PyTypeReal:
            return -1;
        case PyTypeString:
        case PyTypeUnicode:
        case PyTypeDict:
        case PyTypeTuple:
        case PyTypeList:
        case PyTypeSubStream:

        case PyTypeClass:
        case PyTypeModule:
        case PyTypePackedRow:
        case PyTypeSubStruct:
            ASCENT_HARDWARE_BREAKPOINT;
            break;

        }
        return -1;
    }; // most objects have a size function....

    uint8 gettype();
    void IncRef();
    void DecRef();
    size_t GetRef();


private:
    uint8 mType;
    size_t mRefcnt;
};

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
class PyBaseNone : public PyObject
{
public:
    PyBaseNone();
    ~PyBaseNone(){}

    uint32 hash();
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
class PyInt : public PyObject
{
public:
    PyInt(int32 num );
    ~PyInt(){}

    PyInt &operator = (const int num);
    int32 GetValue();

    /** simple hash function, atm this one is pretty weak.
    */
    uint32 hash();
private:
    int32 mNumber;
};

class PyLong : public PyObject
{
public:
    PyLong(int64 num);
    PyLong(uint64 num);
    ~PyLong(){}

    int64 GetValue();
    uint32 hash();
private:
    int64 mNumber;
};

class PyFloat : public PyObject
{
public:
    PyFloat();
    PyFloat(float num);
    PyFloat(double num);
    ~PyFloat(){}

    double GetValue();
    uint32 hash();
private:
    double mNumber;
};

class PyBool : public PyObject
{
public:
    PyBool(bool check);
    ~PyBool();
    bool operator==(const bool check);
    uint32 hash();
private:
    bool mCheck;
};



class PyList : public PyObject
{
public:
    PyList();
    PyList(int elementCount);
    ~PyList();
    PyChameleon &operator[](const int index);
    size_t size();
    bool add(PyObject* obj);
    uint32 hash();
private:
    std::vector<PyChameleon*> mList;
public:
    typedef std::vector<PyChameleon*>::iterator iterator;
    iterator begin();
    iterator end();
};

/* dictionary entry */
struct PyDictEntry
{
    /* uint32 hashvalue; */
    PyObject* key;
    PyObject* obj;
};

/*class PyDict
{
public:
uint8 gettype();
void IncRef();
void DecRef();
size_t GetRef();
uint32 hash();
private:
uint8 mType;
size_t mRefcnt;
uint32 (PyDict::*mHash)();
public:
PyDict();
~PyDict();
PyChameleon operator[](const char* keyName);

bool set_item(const char* key_name, PyObject* obj);
bool set_item(PyObject* key, PyObject* obj);
PyObject* get_item(PyObject* key);
PyObject* get_item(const char* key_name);

size_t size();

private:

typedef std::map<uint32, PyDictEntry*>	DictMap;
typedef DictMap::iterator				DictMapItr;
public:
typedef DictMap::iterator				iterator;
iterator begin();
iterator end();
private:

DictMap mDict;
//mMappingMode makes it possible to simply store the objects
bool	mMappingMode;
uint32	mMappingIndex;

uint32 _hash();
}; */


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

static int SubStreamCounter = 0;
class PySubStream : public PyObject
{
public:
    PySubStream();
    PySubStream(uint8* data, size_t len);
    ~PySubStream();
    uint32 hash();
    uint8* content();
    size_t size();
    bool set(uint8 * data, size_t len);
private:
    void* mData;
    size_t mLen;
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
class PyInstance : public PyObject
{
public:
    PyInstance();
    ~PyInstance();
    uint32 hash();
private:
    PyClass* mClass;
    PyDict* mDict;
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
class PyModule : public PyObject
{
public:
    PyModule();
    ~PyModule();
    uint32 hash();
    PyString* mModuleName;
private:
};



/**
* \class PyPackedRow
*
* @brief this class represents a packet row... only for data parsing..
*
* @author Captnoord
* @date February 2009
*/
class PyPackedRow : public PyObject
{
public:
    PyPackedRow();
    ~PyPackedRow();
    uint32 hash();

    /* raw data containing the normal field data fields */
    uint8* mRawFieldData;			/* also known as 'blob' */
    size_t mRawFieldDataLen;		/* also known as 'blob_length' */

private:
    PyClass* mHeader;
    PyTuple* rawPayLoad;

    // experimental stuff
    std::vector<PyChameleon*> flowers; // payload / virtual row set
public:
    typedef std::vector<PyChameleon*>::iterator iterator;
    iterator begin();
    iterator end();
    size_t size();

    /* magical function */
    bool init(PyObject* header);

    bool setheader(PyClass * obj);
    bool setRawPayLoad(PyObject* tuple);
    PyObject* getRawPayLoad();

    bool addleaf(PyObject* leaf);

    PyObject* GetLeaf(int i);

    PyClass*getheader();

    // generic field stuff
    PyObject* GetFieldObject(int index);
};

class PySubStruct : public PyObject
{
public:
    PySubStruct();
    ~PySubStruct();
    uint32 hash();

    PyObject * getPyObject();
    bool setPyObject(PyObject* obj);
private:
    PyObject* payload;
};

//#pragma pack(pop)

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
static PyList* PyList_New(int elementCount)
{
    PyList * list = new PyList(elementCount);
    return list;
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
            integerSize = uint8(i);
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

// function to do the basic object checking...
static bool PY_TYPE_CHECK(PyObject* object, uint8 type)
{
    if (object == NULL || object->gettype() != type)
        return false;
    return true;
};

PyObject * PyObject_CallObject(PyObject *callable_object, PyObject *args);

#endif //_PYOBJECTS_H
