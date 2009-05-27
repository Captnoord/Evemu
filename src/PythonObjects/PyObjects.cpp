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

#include "EvemuPCH.h"

/*
#include "Common.h"
#include "NGLog.h"
#include "Log.h"
#include "string_map.h"
*/

/************************************************************************/
/* PyInt                                                                */
/************************************************************************/
PyInt::PyInt( int32 num ) : mType(PyTypeInt), mRefcnt(1),
                            mHash(&PyInt::_hash), mNumber(num)
{
}

PyInt & PyInt::operator=( const int num )
{
    mNumber = num;
    mType = PyTypeInt;
    return *this;
}

PyInt::~PyInt()
{
    mType = PyTypeDeleted;
    mHash = NULL;
}

void PyInt::IncRef()
{
    mRefcnt++;
}

void PyInt::DecRef()
{
    mRefcnt--;
    if (mRefcnt <= 0)
        PyDelete(this);
}

uint8 PyInt::gettype()
{
    return mType;
}

uint32 PyInt::hash()
{
    return (this->*mHash)();
}

uint32 PyInt::_hash()
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
PyLong::PyLong( int64 num ) : mType(PyTypeLong), mRefcnt(1),
                              mHash(&PyLong::_hash), mNumber(num)
{
}

/* @todo solve the signed/unsigned problem */
PyLong::PyLong( uint64 num ) : mType(PyTypeLong), mRefcnt(1),
                               mHash(&PyLong::_hash) , mNumber(num)
{
}

PyLong::~PyLong()
{
    mType = PyTypeDeleted;
}

uint8 PyLong::gettype()
{
    return mType;
}

int64 PyLong::GetValue()
{
    return mNumber;
}

void PyLong::IncRef()
{
    mRefcnt++;
}

void PyLong::DecRef()
{
    mRefcnt--;
    if (mRefcnt <= 0)
        PyDelete(this);
}

uint32 PyLong::hash()
{
    return (this->*mHash)();
}

typedef uint16 digit;
#define PyLong_SHIFT    15
#define PyLong_BASE     ((digit)1 << PyLong_SHIFT)
#define PyLong_MASK     ((int)(PyLong_BASE - 1))

uint32 PyLong::_hash()
{
    uint32 x;
    int64 i;
    int sign;
    uint8 * henk = (uint8*)&mNumber;

    /* This is designed so that Python ints and longs with the
    same value hash to the same value, otherwise comparisons
    of mapping keys will turn out weird */
    i = mNumber;
    sign = 1;
    x = 0;
    if (i < 0) {
        sign = -1;
        i = -(i);
    }
#define LONG_BIT_PyLong_SHIFT   (8*sizeof(uint32) - PyLong_SHIFT)
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
PyFloat::PyFloat() : mType(PyTypeReal), mRefcnt(1),
                     mHash(&PyFloat::_hash) , mNumber(0.0)
{
}

PyFloat::PyFloat( float num ) : mType(PyTypeReal), mRefcnt(1),
                                mHash(&PyFloat::_hash), mNumber(num)
{
}

PyFloat::PyFloat( double num ) : mType(PyTypeReal), mRefcnt(1),
                                 mHash(&PyFloat::_hash), mNumber(num)
{
}

PyFloat::~PyFloat()
{
    mType = PyTypeDeleted;
}

uint8 PyFloat::gettype()
{
    return mType;
}

void PyFloat::IncRef()
{
    mRefcnt++;
}

void PyFloat::DecRef()
{
    mRefcnt--;
    if (mRefcnt <= 0)
        PyDelete(this);
}

double PyFloat::GetValue()
{
    return mNumber;
}

uint32 PyFloat::hash()
{
    return (this->*mHash)();
}

uint32 PyFloat::_hash()
{
    ASCENT_HARDWARE_BREAKPOINT;
}

/************************************************************************/
/* PyBool                                                               */
/************************************************************************/
PyBool::PyBool( bool check ) : mType(PyTypeBool), mRefcnt(1),
                               mHash(&PyBool::_hash), mCheck(check)
{
}

PyBool::~PyBool()
{
    mType = PyTypeDeleted;
}

uint8 PyBool::gettype()
{
    return mType;
}

void PyBool::IncRef()
{
    mRefcnt++;
}

void PyBool::DecRef()
{
    mRefcnt--;
    if (mRefcnt <= 0)
        PyDelete(this);
}

bool PyBool::operator==( const bool check )
{
    return (mCheck == check);
}

uint32 PyBool::hash()
{
    return (this->*mHash)();
}

uint32 PyBool::_hash()
{
    ASCENT_HARDWARE_BREAKPOINT;
}

createFileSingleton( PyBaseNone );

void PyBaseNone::IncRef()
{
    mRefcnt++;
}

void PyBaseNone::DecRef()
{
    mRefcnt--;
    if (mRefcnt <= 0)
    {
        printf("PyNone delete: 0x%p\n", this);
        PyDelete(this);
    }
}

uint8 PyBaseNone::gettype()
{
    return mType;
}

PyBaseNone::PyBaseNone() : mType(PyTypeNone), mRefcnt(1)
{
    mHash = &PyBaseNone::_hash;
}

PyBaseNone::~PyBaseNone()
{
    mType = PyTypeDeleted;
}

uint32 PyBaseNone::hash()
{
    return (this->*mHash)();
}

uint32 PyBaseNone::_hash()
{
    return Utils::Hash::sdbm_hash("PyNone");
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
    /* WTF mode this should never ever happen */
    ASCENT_HARDWARE_BREAKPOINT;
    ASCENT_ASSERT(false);
}

PyObject::PyObject()
{
    /* make sure this class is never allocated */
    ASCENT_HARDWARE_BREAKPOINT;
    ASCENT_ASSERT(false);
}

uint32 PyObject::hash()
{
    return (this->*mHash)();
}

uint32 PyObject_Hash( PyObject* obj )
{
    return obj->hash();
}

/************************************************************************/
/* Small portion of the Python API so we are able to handle various    */
/* tasks better and a bit cleaner.                                      */
/* @note we don't aim to mimic the python API perfectly.                */
/************************************************************************/
//static int64 _PyLong_AsInt64(PyLong& number);

/**
 * @brief
 *
 *
 *
 * @param[in] number is the number that needs to be converted into a object
 * @return the converted object.
 * @todo we should create a unified integer object that can contain both a 64 bits value or a 32 bits value.
 */
PyLong* PyLong_FromLongLong(int64 & number)
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
PyInt* PyInt_FromLong(int32 number)
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
PyFloat* PyFloat_FromDouble(double & number)
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
PyString* PyString_FromStringAndSize(const char* str, size_t len)
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
PyTuple* PyTuple_New(size_t elementCount)
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
PyList* PyList_New(int elementCount)
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
PyDict* PyDict_New()
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
bool _PyLong_AsByteArray(PyLong& number, const uint8* buffer, size_t* size)
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
PyLong* _ByteArray_AsPyLong(const uint8* buffer, size_t size)
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

PyLong* PyLong_FromLong(int64 number)
{
    return new PyLong(number);
}

bool _PyLong_AsInt64(PyObject* number, int64& dst_num)
{
    if (number == NULL)
        return false;

    if (number->gettype() != PyTypeLong)
        return false;

    dst_num = ((PyLong*)number)->GetValue();
    return true;
}

int64 _PyLong_AsInt64(PyLong& number)
{
    return number.GetValue();
}
