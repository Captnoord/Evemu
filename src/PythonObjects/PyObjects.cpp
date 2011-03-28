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

#include "EvemuPCH.h"

#include "Common.h"
#include "NGLog.h"
#include "Log.h"
#include "string_map.h"

#include "PyObjects.h"
#include "PyChameleon.h"
#include "PyObjectDumper.h"

/************************************************************************/
/* PyInt                                                                */
/************************************************************************/
PyInt::PyInt( int32 num ) : PyObject(PyTypeInt), mNumber(num) {}

PyInt & PyInt::operator=( const int num )
{
    mNumber = num;
    return *this;
}

// its a weak hash I know...... take a look at this... can't remember the python stuff
uint32 PyInt::hash()
{
    return mNumber;
}

int32 PyInt::GetValue()
{
    return mNumber;
}

/************************************************************************/
/* PyLong                                                               */
/************************************************************************/
PyLong::PyLong( int64 num ) : PyObject(PyTypeLong), mNumber(num) {}

/* @todo solve the signed/unsigned problem */
PyLong::PyLong( uint64 num ) : PyObject(PyTypeLong), mNumber(num) {}

int64 PyLong::GetValue()
{
    return mNumber;
}

/* Long integer representation.
   The absolute value of a number is equal to
   	SUM(for i=0 through abs(ob_size)-1) ob_digit[i] * 2**(SHIFT*i)
   Negative numbers are represented with ob_size < 0;
   zero is represented by ob_size == 0.
   In a normalized number, ob_digit[abs(ob_size)-1] (the most significant
   digit) is never zero.  Also, in all cases, for all valid i,
   	0 <= ob_digit[i] <= MASK.
   The allocation function takes care of allocating extra memory
   so that ob_digit[0] ... ob_digit[abs(ob_size)-1] are actually available.

   CAUTION:  Generic code manipulating subtypes of PyVarObject has to
   aware that longs abuse  ob_size's sign bit.
*/


typedef uint16 digit;
#define PyLong_SHIFT    15
#define PyLong_BASE     ((digit)1 << PyLong_SHIFT)
#define PyLong_MASK     ((int)(PyLong_BASE - 1))

uint32 PyLong::hash()
{
    uint32 x;
    int i = 0;
    int sign;
    uint8 * henk = (uint8*)&mNumber;

    /* get the actual size of the long integer */
    for (int j = 0; j < 8; j++) {
        if(!henk[j]) {
            i = j + 1;
            break;
        }
    }

    if (mNumber == 0)
        i = 0;

    /* This is designed so that Python ints and longs with the
    same value hash to the same value, otherwise comparisons
    of mapping keys will turn out weird */

    //i = mNumber;
    sign = 1;
    x = 0;
    if (i < 0) {
        sign = -1;
        i = -(i);
    }
#define LONG_BIT_PyLong_SHIFT	(8*sizeof(uint32) - PyLong_SHIFT)
    /* The following loop produces a C long x such that (unsigned long)x
    is congruent to the absolute value of v modulo ULONG_MAX.  The
    resulting x is nonzero if and only if v is. */
    while (--i >= 0) {
        /* Force a native long #-bits (32 or 64) circular shift */
        x = ((x << PyLong_SHIFT) & ~PyLong_MASK) | ((x >> LONG_BIT_PyLong_SHIFT) & PyLong_MASK);
        x += henk[i];//v->ob_digit[i];
        /* If the addition above overflowed (thinking of x as
        unsigned), we compensate by incrementing.  This preserves
        the value modulo ULONG_MAX. */
        if ((unsigned long)x < henk[i])//v->ob_digit[i])
            x++;
    }
#undef LONG_BIT_PyLong_SHIFT
    x = x * sign;
    if (x == -1)
        x = -2;
    return x;
}

/************************************************************************/
/* PyFloat                                                              */
/************************************************************************/
PyFloat::PyFloat() : PyObject(PyTypeReal), mNumber(0.0) {}
PyFloat::PyFloat( float num ) : PyObject(PyTypeReal), mNumber(num) {}
PyFloat::PyFloat( double num ) : PyObject(PyTypeReal), mNumber(num) {}

double PyFloat::GetValue()
{
    return mNumber;
}

// hashing a float? lol....
uint32 PyFloat::hash()
{
    //ASCENT_HARDWARE_BREAKPOINT;
    /* warning... floats make terrible hash value's */
    Log.Warning("PyFloat", " floats make terrible hash value's ");
    return *((uint32*)&mNumber);
}

/************************************************************************/
/* PyBool                                                               */
/************************************************************************/
PyBool::PyBool( bool check ) : PyObject(PyTypeBool),  mCheck(check) {}

bool PyBool::operator==( const bool check )
{
    return (mCheck == check);
}

PyBool::~PyBool()
{

}

uint32 PyBool::hash()
{
    ASCENT_HARDWARE_BREAKPOINT;
}



/************************************************************************/
/* PyTuple                                                              */
/************************************************************************/
PyTuple::PyTuple() : PyObject(PyTypeTuple) {}

PyTuple::PyTuple( size_t elementCount ) : PyObject(PyTypeTuple)
{
    if (elementCount > PY_TUPLE_ELEMENT_MAX)
    {
        Log.Error("PyTuple", "constructor is requested to allocate a stupid amount of elements: %d", elementCount);
        assert(false);
        return;
    }

    resize(elementCount);
}

PyTuple::~PyTuple()
{
    clear();
}

/* this is kinda slow because we are resizing a vector */
/*
PyChameleon & PyTuple::operator[]( const int index )
{
    if (index < 0)
        return PyErrorIterator;
    if (index+1 > (int)mTuple.size())
        mTuple.resize(index+1);
    PyChameleon & itr = *mTuple[index];
    return itr;
}
*/

size_t PyTuple::size()
{
    return mTuple.size();
}

void PyTuple::clear()
{
    iterator itr = mTuple.begin();
    for (; itr != mTuple.end(); itr++)
    {
        PyChameleon* obj = *itr;
        SafeDelete(obj);
    }
    mTuple.clear();
}

uint32 PyTuple::hash()
{
    PyTuple & tuple = *(PyTuple*)this;
    uint32 hash = 5381;
    for (int i = 0; i < (int)tuple.size(); i++)
    {
        uint32 hashChunk = PyObject_Hash(tuple.GetItem(i));
        hash = (hash << 3) + hashChunk;
    }
    return hash;
}

/* quite tricky */
bool PyTuple::resize( size_t newTupleSize )
{
    size_t currentTupleSize = mTuple.size();

    /* if we are required to resize to our current size, don't do shit*/
    if (newTupleSize == currentTupleSize)
        return true;

    /* we need to grow or shrink */
    if (newTupleSize > currentTupleSize)
    {
        // debug breakpoint for the situation we are resizing a already excisting object
        if (currentTupleSize != 0)
            ASCENT_HARDWARE_BREAKPOINT;

        /* we need to grow (this one is easy) */
        mTuple.resize(newTupleSize);

        for (size_t i = currentTupleSize; i < newTupleSize; i++)
        {
            mTuple[int(i)] = new PyChameleon();
        }
    }
    else
    {
        ASCENT_HARDWARE_BREAKPOINT;
        /* we need to shrink (a bit harder) */
        for(size_t i = currentTupleSize; i > newTupleSize; i--)
        {
            PyChameleon* obj = mTuple[int(i)];
            SafeDelete(obj);
        }
        mTuple.resize(newTupleSize);
    }
    return true;
}

PyObject* PyTuple::GetItem( const int index )
{
    if (index < 0)
        return NULL;
    if (index+1 > (int)mTuple.size())
        mTuple.resize(index+1);

    PyChameleon * itr = mTuple[index];
    return itr->getPyObject();
}

int32 PyTuple::GetItem_asInt( const int index )
{
    if (index < 0)
        return 0;
    if (index > (int)mTuple.size())
        return 0;

    PyChameleon * itr = mTuple[index];
    if (itr == NULL)
        return 0;
    PyObject* obj = itr->getPyObject();
    if (obj == NULL || obj->gettype() != PyTypeInt)
        return 0;

    return ((PyInt*)obj)->GetValue();
}

PyTuple* PyTuple::GetItem_asPyTuple( const int index )
{
    if (index < 0)
        return NULL;
    if (index > (int)mTuple.size())
        return NULL;

    PyChameleon * itr = mTuple[index];
    if (itr == NULL)
        return NULL;
    PyObject* obj = itr->getPyObject();
    if (obj == NULL || obj->gettype() != PyTypeTuple)
        return NULL;

    return (PyTuple*)obj;
}

PyList* PyTuple::GetItem_asPyList( const int index )
{
    if (index < 0)
        return NULL;
    if (index > (int)mTuple.size())
        return NULL;

    PyChameleon * itr = mTuple[index];
    if (itr == NULL)
        return NULL;
    PyObject* obj = itr->getPyObject();
    if (obj == NULL || obj->gettype() != PyTypeList)
        return NULL;

    return (PyList*)obj;
}

PyString* PyTuple::GetItem_asPyString( const int index )
{
    if (index < 0)
        return NULL;
    if (index > (int)mTuple.size())
        return NULL;

    PyChameleon * itr = mTuple[index];
    if (itr == NULL)
        return NULL;
    PyObject* obj = itr->getPyObject();
    if (obj == NULL || obj->gettype() != PyTypeString)
        return NULL;

    return (PyString*)obj;
}

PySubStream* PyTuple::GetItem_asPySubStream( const int index )
{
    if (index < 0)
        return NULL;
    if (index > (int)mTuple.size())
        return NULL;

    PyChameleon * itr = mTuple[index];
    if (itr == NULL)
        return NULL;
    PyObject* obj = itr->getPyObject();
    if (obj == NULL || obj->gettype() != PyTypeSubStream)
        return NULL;

    return (PySubStream*)obj;
}

PyClass* PyTuple::GetItem_asPyClass( const int index )
{
    if (index < 0)
        return NULL;
    if (index > (int)mTuple.size())
        return NULL;

    PyChameleon * itr = mTuple[index];
    if (itr == NULL)
        return NULL;
    PyObject* obj = itr->getPyObject();
    if (obj == NULL || obj->gettype() != PyTypeClass)
        return NULL;

    return (PyClass*)obj;
}

bool PyTuple::set_item( const int index, PyObject *object )
{
    if (object == NULL)
        ASCENT_HARDWARE_BREAKPOINT;

    if (index < 0)
        return false;
    if (index+1 > (int)mTuple.size())
        mTuple.resize(index+1);

    // we'r storing the object somewhere else so... dec the ref..
    object->IncRef();

    PyChameleon & itr = *mTuple[index];
    itr.setPyObject(object);
    return true;
}

/************************************************************************/
/* PyList                                                               */
/************************************************************************/
PyList::PyList() : PyObject(PyTypeList) {}
PyList::PyList( int elementCount ) : PyObject(PyTypeList)
{
    mList.resize(elementCount);
}

PyList::~PyList()
{
    iterator Itr = mList.begin();
    for (; Itr != mList.end(); Itr++)
    {
        delete *Itr;
    }

    mList.clear();
}

PyChameleon & PyList::operator[]( const int index )
{
    if (index < 0)
        return PyErrorIterator;

    if (index > (int)mList.size())
        mList.resize(index);

    if (mList[index] == NULL)
        mList[index] = new PyChameleon();

    return *mList[index];
}

size_t PyList::size()
{
    return mList.size();
}

bool PyList::add( PyObject* obj )
{
    if (obj == NULL || obj->gettype() == PyTypeDeleted)
        return false;

    PyChameleon * itr = new PyChameleon();
    itr->setPyObject(obj);
    mList.push_back(itr);

    return true;
}

uint32 PyList::hash()
{
    uint32 hsh = 0;
    PyList::iterator itr = mList.begin();
    for (; itr != mList.end(); itr++)
    {
        PyObject* obj = (*itr)->getPyObject();
        assert(obj);
        hsh|= PyObject_Hash(obj);
        hsh = hsh << 3;
    }
    return hsh;
}

PyList::iterator PyList::begin()
{
    return mList.begin();
}

PyList::iterator PyList::end()
{
    return mList.end();
}



/************************************************************************/
/* PySubStream                                                          */
/************************************************************************/
PySubStream::PySubStream() : PyObject(PyTypeSubStream), mData(NULL), mLen(0) {}

PySubStream::PySubStream( uint8* data, size_t len ) : PyObject(PyTypeSubStream), mData(NULL), mLen(0)
{
    if (data == NULL || len == 0)
        return;

    mLen = len;
    mData = ASCENT_MALLOC(mLen+1);
    ASCENT_MEMCPY(mData, data, mLen);
}

PySubStream::~PySubStream()
{
    SafeFree(mData);

    mLen = 0;
}

uint8* PySubStream::content()
{
    return (uint8*)mData;
}

bool PySubStream::set( uint8 * data, size_t len )
{
    if (mData != NULL)
        mData = ASCENT_REALLOC(mData, len+1);
    else
        mData = ASCENT_MALLOC(len+1);

    if (mData == NULL)
    {
        mLen = 0;
        return false;
    }

    mLen = len;

    ASCENT_MEMCPY(mData, data, mLen);
    return true;
}


size_t PySubStream::size()
{
    return mLen;
}

uint32 PySubStream::hash()
{
    return Utils::Hash::sdbm_hash((char*)mData, (int)mLen);
}

/************************************************************************/
/* PyClass                                                              */
/************************************************************************/
PyClass::PyClass() : PyObject(PyTypeClass), mDict(NULL), mName(NULL), mBases(NULL), mWeakRefList(NULL), mInDict(NULL)
{
    // needed... this is what python called 'self'
    mDict = new PyDict();
}

PyClass::~PyClass()
{
    destruct();

    if (mName)
    {
        mName->DecRef();
        mName = NULL;
    }
    if (mBases)
    {
        mBases->DecRef();
        mBases = NULL;
    }

    if (mDict)
    {
        mDict->DecRef();
        mDict = NULL;
    }

    if (mWeakRefList)
    {
        mWeakRefList->DecRef();
        mWeakRefList = NULL;
    }

    if (mInDict)
    {
        mInDict->DecRef();
        mInDict = NULL;
    }
}

bool PyClass::setname( PyString* name )
{
    // for now we don't do class name object swapping and stuff
    ASCENT_ASSERT(mName == NULL);

    // object must exist
    if (name == NULL)
        return false;

    // name must be a string object
    if (name->gettype() != PyTypeString)
        return false;

    // must not contain null bytes
    if (name->length() == 0)
        return false;

    mName = name;
    return true;
}

bool PyClass::setbases( PyTuple* tuple )
{
    ASCENT_ASSERT(mBases == NULL);

    // object should exist
    if (tuple == NULL)
        return false;

    mBases = tuple;
    return true;
}

bool PyClass::setdict( PyDict* dict )
{
    ASCENT_ASSERT(mDict == NULL);

    // object should exist
    if (dict == NULL)
        return false;

    if (dict->gettype() != PyTypeDict)
        return false;

    mDict = dict;
    return true;
}

bool PyClass::setDirList( PyList * list )
{
    ASCENT_ASSERT(mWeakRefList == NULL);
    //assert(list->gettype() == PyTypeList);
    if (list == NULL)
        return false;
    mWeakRefList = list;
    return true;
}

bool PyClass::setDirDict( PyDict * dict )
{
    ASCENT_ASSERT(mInDict == NULL);
    if (dict == NULL)
        return false;
    mInDict = dict;
    return true;
}

PyString* PyClass::getname()
{
    return mName;
}

PyTuple* PyClass::getbases()
{
    return mBases;
}

PyDict* PyClass::getdict()
{
    return mDict;
}

PyList * PyClass::getDirList()
{
    return mWeakRefList;
}

PyDict * PyClass::getDirDict()
{
    return mInDict;
}

/**
* @note this hash is fucked up.
*/
uint32 PyClass::hash()
{
    uint32 hsh = 0;
    if (mName)
    {
        hsh = mName->hash();
        hsh = hsh << 3;
    }

    if (mBases)
    {
        hsh|= mBases->hash();
        hsh = hsh << (mBases->size() / 3);
    }

    if (mDict)
    {
        hsh|= mDict->hash();
        hsh = hsh << (mDict->size() / 3);
    }

    if (mWeakRefList)
    {
        hsh|= mWeakRefList->hash();
        hsh = hsh << (mWeakRefList->size() / 10);
    }

    if (mInDict)
    {
        hsh|= mInDict->hash();
        hsh = hsh << (mInDict->size() / 10);
    }

    return hsh;
}

void PyObject::IncRef()
{
    mRefcnt++;
}

void PyObject::DecRef()
{
    mRefcnt--;
    if (mRefcnt <= 0)
        PyDelete(this);
}

uint8 PyObject::gettype()
{
    return mType;
}

PyObject::~PyObject()
{
}

PyObject::PyObject(PyType type) : mType(type), mRefcnt(1) {}

size_t PyObject::GetRef()
{
    return mRefcnt;
}

createFileSingleton( PyBaseNone );

PyBaseNone::PyBaseNone() : PyObject(PyTypeNone) {}

// hash a none object? your seriouse?
uint32 PyBaseNone::hash()
{
    //ASCENT_HARDWARE_BREAKPOINT;
    return Utils::Hash::sdbm_hash("PyNone");
}

/************************************************************************/
/* PyPackedRow                                                          */
/************************************************************************/
PyPackedRow::PyPackedRow() : PyObject(PyTypePackedRow), mHeader(NULL), mRawFieldData(NULL), mRawFieldDataLen(0) {}

PyPackedRow::~PyPackedRow()
{
    iterator Itr = flowers.begin();
    for (; Itr != flowers.end(); Itr++)
    {
        delete *Itr;
    }

    flowers.clear();

    if (mHeader)
        mHeader->DecRef();

    if (rawPayLoad)
        rawPayLoad->DecRef();

    if (mRawFieldData)
        SafeFree(mRawFieldData);
}

bool PyPackedRow::addleaf( PyObject* leaf )
{
    assert(leaf != NULL);

    // fucked up because of some c++ fuckup...
    uint32 hsh = PyObject_Hash(leaf);
    PyChameleon * itr = new PyChameleon();
    itr->setPyObject(leaf);

    flowers.push_back(itr);

    return true;
}

uint32 PyPackedRow::hash()
{
    uint32 hsh = 0;

    if (mHeader)
        hsh |= PyObject_Hash((PyObject *)mHeader);

    if (rawPayLoad)
    {
        hsh = hsh << 3;
        hsh |= PyObject_Hash((PyObject *)mHeader);
    }

    iterator itr = flowers.begin();
    for (; itr != flowers.end(); itr++)
    {
        hsh = hsh << 3;
        if((*itr)->getPyObject())
            hsh |= PyObject_Hash((*itr)->getPyObject());
    }
    return hsh;
}

bool PyPackedRow::setheader( PyClass * obj )
{
    if (obj->gettype() != PyTypeClass)
        return false;

    mHeader = obj;
    return true;
}

PyPackedRow::iterator PyPackedRow::begin()
{
    return flowers.begin();
}

PyPackedRow::iterator PyPackedRow::end()
{
    return flowers.end();
}

PyClass* PyPackedRow::getheader()
{
    return mHeader;
}

size_t PyPackedRow::size()
{
    return flowers.size();
}

bool PyPackedRow::setRawPayLoad( PyObject* tuple )
{
    rawPayLoad = (PyTuple*)tuple;
    return true;
}

PyObject* PyPackedRow::getRawPayLoad()
{
    return (PyObject*)rawPayLoad;
}

bool PyPackedRow::init( PyObject* header )
{
    size_t argumentCount = 0;
    if (header->gettype() == PyTypeTuple)
    {
        PyTuple& tulple = *(PyTuple*)header;
        argumentCount = tulple.size();
        if (argumentCount == 0)
        {
            Log.Error("PyPackedRow", "init: expected at least one argument");
            return false;
        }
    }
    else
    {
        argumentCount = 1; // I have no clue
    }

    //mHeader = header;

    return true;
}

PyObject* PyPackedRow::GetFieldObject( int index )
{
    PyTuple & fieldHelper = *rawPayLoad;
    //PyObject * res = fieldHelper[index].getPyObject();
    PyObject * res = fieldHelper.GetItem(index);
    //f ()
    return 0;
}

PyObject* PyPackedRow::GetLeaf( int i )
{
    return flowers[i]->getPyObject();
}

/************************************************************************/
/* substruct                                                            */
/************************************************************************/
PySubStruct::PySubStruct() : PyObject(PyTypeSubStruct), payload(NULL) {}

PySubStruct::~PySubStruct()
{
    if (payload)
        payload->DecRef();
}

PyObject * PySubStruct::getPyObject()
{
    return payload;
}

bool PySubStruct::setPyObject( PyObject* obj )
{
    if (obj == NULL)
        return false;

    payload = obj;
    obj->IncRef();

    return true;
}

// class related hash function
uint32 PySubStruct::hash()
{
    ASCENT_HARDWARE_BREAKPOINT;
}

/************************************************************************/
/* PyModule                                                             */
/************************************************************************/
PyModule::PyModule() : PyObject(PyTypeModule), mModuleName(NULL) {}

PyModule::~PyModule() {}


uint32 PyModule::hash()
{
    ASCENT_HARDWARE_BREAKPOINT;
    return 0;
}

uint32 PyObject_Hash( PyObject* obj )
{
    return obj->hash();
}

// this is so wrong 'callable_object' should only contain the method to call...
// now we are assuming they always call init and new...
PyObject * PyObject_CallObject( PyObject *callable_object, PyObject *args )
{
    assert(callable_object);
    assert(args);
    PyClass * pClass = (PyClass *)callable_object;
    PyClass * pNewClass = pClass->New();
    pNewClass->init(args);
    return pNewClass;
}
