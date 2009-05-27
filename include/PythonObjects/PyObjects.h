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

#ifndef _PYOBJECTS_H
#define _PYOBJECTS_H

enum PyType
{
    PyTypeNone,         //0
    PyTypeBool,         //1
    PyTypeInt,          //2
    PyTypeLong,         //3
    PyTypeReal,         //4
    PyTypeString,       //5
    PyTypeUnicode,      //6
    PyTypeDict,         //7
    PyTypeTuple,        //8
    PyTypeList,         //9
    PyTypeSubStream,    //10
    PyTypeClass,        //11
    PyTypeModule,       //12
    PyTypePackedRow,    //13
    PyTypeSubStruct,    //14
    PyTypeDeleted,      //15 // must be last, and can max be 16...
};

// object hash function
//#define hashfunc(x) uint32 (*x)(void);
typedef uint32 (*hashfunc)(void);

class PyInt;
class PyDict;
class PyList;
class PyTuple;
class PyPackedRow;
class PyString;
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
    uint8 mType;
    size_t mRefcnt;
    uint32 (PyBaseNone::*mHash)();
public:
    uint8 gettype();
    void IncRef();
    void DecRef();
    uint32 hash();
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


/* dictionary entry */
struct PyDictEntry
{
    /* uint32 hashvalue; */
    PyObject* key;
    PyObject* obj;
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
    uint8 mType;
    size_t mRefcnt;
    uint32 (PyInt::*mHash)();
    int32 mNumber;
public:
    uint8 gettype();
    void IncRef();
    void DecRef();

    /** simple hash function, atm this one is pretty weak.
     */
    uint32 hash();

public:
    explicit PyInt(int32 num );
    PyInt &operator = (const int num);
    ~PyInt();
    int32 GetValue();
private:
    uint32 _hash();
};

class PyLong
{
    uint8 mType;
    size_t mRefcnt;
    uint32 (PyLong::*mHash)();
    int64 mNumber;
public:
    uint8 gettype();
    void IncRef();
    void DecRef();
    uint32 hash();
public:
    explicit PyLong(int64 num);
    explicit PyLong(uint64 num);
    ~PyLong();
    int64 GetValue();
private:
    uint32 _hash();
};

class PyFloat
{
    uint8 mType;
    size_t mRefcnt;
    uint32 (PyFloat::*mHash)();
    double mNumber;
public:
    uint8 gettype();
    void IncRef();
    void DecRef();
    uint32 hash();
public:
    explicit PyFloat();
    explicit PyFloat(float num);
    explicit PyFloat(double num);
    ~PyFloat();
    double GetValue();
private:
    uint32 _hash();
};

class PyBool
{
    uint8 mType;
    size_t mRefcnt;
    uint32 (PyBool::*mHash)();
    bool mCheck;
public:
    uint8 gettype();
    void IncRef();
    void DecRef();
    uint32 hash();
public:
    explicit PyBool(bool check);
    ~PyBool();
    bool operator==(const bool check);
private:
    uint32 _hash();
};

#pragma pack(pop)

/************************************************************************/
/* Small portion of the Python API so we are able to handle various    */
/* tasks better and a bit cleaner.                                      */
/* @note we don't aim to mimic the python API perfectly.                */
/************************************************************************/
int64 _PyLong_AsInt64(PyLong& number);

/**
 * @brief
 *
 *
 *
 * @param[in] number is the number that needs to be converted into a object
 * @return the converted object.
 * @todo we should create a unified integer object that can contain both a 64 bits value or a 32 bits value.
 */
PyLong* PyLong_FromLongLong(int64 & number);

/**
 * @brief
 *
 *
 *
 * @param[in]
 * @param[out]
 * @return
 */
PyInt* PyInt_FromLong(int32 number);

/**
 * @brief
 *
 *
 *
 * @param[in]
 * @param[out]
 * @return
 */
PyFloat* PyFloat_FromDouble(double & number);

/**
 * @brief
 *
 *
 *
 * @param[in]
 * @param[out]
 * @return
 */
PyString* PyString_FromStringAndSize(const char* str, size_t len);

/**
 * @brief
 *
 *
 *
 * @param[in]
 * @param[out]
 * @return
 */
PyTuple* PyTuple_New(size_t elementCount);

/**
 * @brief
 *
 *
 *
 * @param[in]
 * @param[out]
 * @return
 */
PyList* PyList_New(int elementCount);

/**
 * @brief
 *
 *
 *
 * @param[in]
 * @param[out]
 * @return
 */
PyDict* PyDict_New();

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
bool _PyLong_AsByteArray(PyLong& number, const uint8* buffer, size_t* size);

/**
 * @brief
 *
 *
 *
 * @param[in]
 * @param[out]
 * @return
 */
PyLong* _ByteArray_AsPyLong(const uint8* buffer, size_t size);

PyLong* PyLong_FromLong(int64 number);

bool _PyLong_AsInt64(PyObject* number, int64& dst_num);

int64 _PyLong_AsInt64(PyLong& number);

// a wrapper for hashing "objects"
uint32 PyObject_Hash(PyObject* obj);

#endif //_PYOBJECTS_H
