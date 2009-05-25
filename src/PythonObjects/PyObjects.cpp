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

/************************************************************************/
/* PyString code                                                        */
/************************************************************************/
PyString::PyString() : mType(PyTypeString), mStr(NULL), mStrLen(0), mHashValue(0), mRefcnt(1)
{
	mHash = &PyString::_hash;
}

PyString::PyString(const char* str) : mType(PyTypeString), mStr(NULL), mStrLen(0), mHashValue(0) , mRefcnt(1)
{
	mHash = &PyString::_hash;
	size_t len = strlen(str);
	set(str, len);
}

PyString::PyString(const char* str, size_t len) : mType(PyTypeString), mStr(NULL), mStrLen(0), mHashValue(0), mRefcnt(1)
{
	mHash = &PyString::_hash;
	set(str, len);
}

PyString::PyString(std::string& str) : mType(PyTypeString), mStr(NULL),  mStrLen(0), mHashValue(0), mRefcnt(1)
{
	mHash = &PyString::_hash;
	set(str.c_str(), str.size());
}

PyString::~PyString()
{
	SafeFree(mStr);

	mType = PyTypeDeleted;
	mStrLen = 0;
	mHash = NULL;
}

bool PyString::set(const char* str, size_t len)
{
	if (str == NULL || len == 0)
		return false;

	if (mStr != NULL)
		mStr = (char*)ASCENT_REALLOC(mStr, len+1);
	else
		mStr = (char*)ASCENT_MALLOC(len+1);

	mStrLen = len;
	
	if (mStr == NULL)
		return false;

	ASCENT_MEMCPY(mStr, str, mStrLen);
	mStr[mStrLen] = '\0';
	return true;
}

const char* PyString::content()
{
	return mStr;
}

uint8 PyString::gettype()
{
	return mType;
}

void PyString::IncRef()
{
	mRefcnt++;
}

void PyString::DecRef()
{
	mRefcnt--;
	if (mRefcnt <= 0)
		PyDelete(this);
}

const size_t PyString::length()
{
	return mStrLen;
}

/* atm only borrowing from the slightly bloated append function.. */
PyString& PyString::append( const std::string& str )
{
	return append(str, mStrLen, str.size());
}

/* the trick is that we can also append a string to a empty string. */
PyString& PyString::append( const std::string& str, size_t pos, size_t n )
{
	// we are appending a empty string
	if(mStrLen == 0)
	{
		if (n > str.size())
			n = str.size();

		// we can use the set function as we are empty
		set(str.c_str(), n);
		return *this;
	}
	else // we aren't a empty string... complicated
	{
		size_t newSize = mStrLen + n;
		assert(mStr); // we should have a existing piece of memory

		mStr = (char*)ASCENT_REALLOC(mStr, newSize + 1); // realloc... it may move memory around...
		
		// the point where its added
		char* crossA = &mStr[pos];

		// the point where it sends
		char* crossB = &mStr[pos + n];

		// chunk size
		size_t crossSize = mStrLen - pos;

		// first move that chunk
		memcpy(crossB,crossA, crossSize);

		// append the string
		memcpy(crossA,str.c_str(), n);

		mStrLen = newSize;
		mStr[mStrLen] = '\0';
		return *this;
	}
}

bool PyString::resize( size_t size )
{
	/* handling rare conditions separately */
	if (mStr != NULL && size == 0)
	{
		SafeFree(mStr);
		mStrLen = size;
		return true;
	}

	mStrLen = size;

	if (mStr != NULL)
		mStr = (char*)ASCENT_REALLOC(mStr, mStrLen+1);
	else
		mStr = (char*)ASCENT_MALLOC(mStrLen+1);

	if (mStr == NULL)
		return false;

	mStr[mStrLen] = '\0';
	return true;

}

bool PyString::operator==( const char *str1 )
{
	char* yup = strstr(mStr, str1);
	if (yup == NULL)
		return false;

	return true;
}

PyString & PyString::operator=( const char *str )
{
	size_t len = strlen(str);
	set(str, len);
	return *this;
}

uint32 PyString::hash()
{
	return (this->*mHash)();
}

uint32 PyString::_hash()
{
	if (mHashValue == 0)
	{
		mHashValue = Utils::Hash::sdbm_hash(mStr, (int)mStrLen);
		return mHashValue;

	}
	return mHashValue;
}

char &PyString::operator[]( const int index )
{
	if (mStr != NULL && (int)mStrLen > index)
		return mStr[index];

	/* @todo throw error*/
	ASCENT_HARDWARE_BREAKPOINT;
	/* not all routes return a char, this is a bug */
}
/************************************************************************/
/* PyInt                                                                */
/************************************************************************/
PyInt::PyInt( int32 num ) : mType(PyTypeInt), mRefcnt(1)
{
	mHash = &PyInt::_hash;
	
	mNumber = num;
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
PyLong::PyLong( int64 num ) : mNumber(num), mType(PyTypeLong), mRefcnt(1)
{
	mHash = &PyLong::_hash;
}

/* @todo solve the signed/unsigned problem */
PyLong::PyLong( uint64 num ) : mNumber(num), mType(PyTypeLong), mRefcnt(1)
{
	mHash = &PyLong::_hash;
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
PyFloat::PyFloat() : mType(PyTypeReal), mNumber(0.0), mRefcnt(1)
{
	mHash = &PyFloat::_hash;
}
PyFloat::PyFloat( float num ) : mType(PyTypeReal), mNumber(num), mRefcnt(1)
{
	mHash = &PyFloat::_hash;
}

PyFloat::PyFloat( double num ) : mType(PyTypeReal), mNumber(num), mRefcnt(1)
{
	mHash = &PyFloat::_hash;
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
PyBool::PyBool( bool check ) : mType(PyTypeBool), mCheck(check), mRefcnt(1)
{
	mHash = &PyBool::_hash;
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

/************************************************************************/
/* PyTuple                                                              */
/************************************************************************/
PyTuple::PyTuple() : mType(PyTypeTuple), mRefcnt(1)
{
	mHash = &PyTuple::_hash;
}

PyTuple::PyTuple( size_t elementCount ) : mType(PyTypeTuple), mRefcnt(1)
{
	mHash = &PyTuple::_hash;
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
	mType = PyTypeDeleted;
}

/* this is kinda slow because we are resizing a vector */
PyChameleon & PyTuple::operator[]( const int index )
{
	if (index < 0)
		return PyErrorIterator;
	if (index+1 > (int)mTuple.size())
		mTuple.resize(index+1);
	PyChameleon & itr = *mTuple[index];
	return itr;
}

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

uint8 PyTuple::gettype()
{
	return mType;
}

void PyTuple::IncRef()
{
	mRefcnt++;
}

void PyTuple::DecRef()
{
	mRefcnt--;
	if (mRefcnt <= 0)
		PyDelete(this);
}

uint32 PyTuple::hash()
{
	return (this->*mHash)();
}

uint32 PyTuple::_hash()
{
	PyTuple & tuple = *(PyTuple*)this;
	uint32 hash = 5381;
	for (int i = 0; i < (int)tuple.size(); i++)
	{
		uint32 hashChunk = PyObject_Hash(tuple[i].getPyObject());
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

int PyTuple::GetInt( const int index )
{
	PyChameleon * itr = mTuple[index];
	PyObject * object = itr->getPyObject();
	if (object->gettype() != PyTypeInt)
	{
		Log.Error("PyTuple", "GetInt: trying to get int from a not int location");
		return 0;
	}

	PyInt * intobj = (PyInt*)object;
	return intobj->GetValue();
}

double PyTuple::GetFloat( const int index )
{
	PyChameleon * itr = mTuple[index];
	PyObject * object = itr->getPyObject();
	if (object->gettype() != PyTypeReal)
	{
		Log.Error("PyTuple", "GetFloat: trying to get float from a not float location");
		return 0;
	}

	PyFloat * floatobj = (PyFloat*)object;
	return floatobj->GetValue();

}

std::string PyTuple::GetString( const int index )
{
	PyChameleon * itr = mTuple[index];
	PyObject * object = itr->getPyObject();
	if (object->gettype() != PyTypeString)
	{
		Log.Error("PyTuple", "GetString: trying to get string from a not string location");
		return 0;
	}

	PyString * strobj = (PyString*)object;
	std::string str = strobj->content();
	return str;
}

bool PyTuple::GetString( const int index, std::string & str )
{
	if (index > (int)mTuple.size())
		return false;

	PyChameleon * itr = mTuple[index];
	PyObject * object = itr->getPyObject();

	if (object->gettype() != PyTypeString)
		return false;

	PyString * strobj = (PyString*)object;
	
	str.clear(); // make sure its empty
	str.append(strobj->content(), strobj->length());

	return true;
}

template<typename T>
void PyTuple::set_item(const int index, T * object)
{
	if (index+1 > (int)mTuple.size())
		mTuple.resize(index+1);
	PyChameleon * itr = mTuple[index];
	itr->setPyObject((PyObject*)object);
}

/* @todo add other variable types */
int PyTuple::scanf( const char * format, ... )
{
	/* should not be possible */
	if (format == NULL || *format == '\0')
		return 0;

	size_t formatLen = strlen(format);
	if (formatLen == 0)
		return 0;

	va_list ap;
	va_start(ap, format);

	void* pVar = NULL;
	size_t formatIndex = 0;
	while (formatLen != formatIndex)
	{
		pVar = va_arg( ap, void* );

		if (pVar == NULL)
		{
			va_end(ap);
			return 0;
		}

		if (formatIndex > mTuple.size())
		{
			va_end(ap);
			return 0;
		}

		char tag = format[formatIndex];

		PyObject* foundObject = mTuple[formatIndex]->getPyObject();
		switch(tag)
		{
			/*unicode string*/
		case 'u': // std::wstring
			{
				if (foundObject->gettype() != PyTypeUnicode)
				{
					va_end(ap);
					return 0;
				}
	
				std::wstring * str = (std::wstring *)pVar;
				str->clear();
	
				size_t len = ((PyUnicodeUCS2*)foundObject)->length();
				wchar_t * buff = ((PyUnicodeUCS2*)foundObject)->content();
				str->append(buff, len);
			}
			break;

		case 'i': // int
			{
				if (foundObject->gettype() != PyTypeInt)
				{
					va_end(ap);
					return 0;
				}

				int32 * num = (int32 *)pVar;
				*num = ((PyInt*)foundObject)->GetValue();
			}
			break;

		case 'f': // double
			{
				if (foundObject->gettype() != PyTypeReal)
				{
					va_end(ap);
					return 0;
				}

				double * num = (double *)pVar;
				*num = ((PyFloat*)foundObject)->GetValue();
			}
			break;

		case 's': // std::string
			{
				if (foundObject->gettype() != PyTypeString)
				{
					va_end(ap);
					return 0;
				}
	
				std::string * str = (std::string *)pVar;
				str->clear();
	
				size_t len = ((PyString*)foundObject)->length();
				const char* buff = ((PyString*)foundObject)->content();
				str->append(buff, len);
			}
			break;
		}

		formatIndex++;
	}

	va_end(ap);
	return (int)formatIndex;
}

void PyTuple::set_str( const int index, const char* str )
{
	if (index+1 > (int)mTuple.size())
		mTuple.resize(index+1);
	PyChameleon * itr = mTuple[index];
	PyString *pStr = new PyString(str);
	itr->setPyObject((PyObject*)pStr);
}

void PyTuple::set_str( const int index, const char* str, const size_t len )
{
	if (index+1 > (int)mTuple.size())
		mTuple.resize(index+1);
	PyChameleon * itr = mTuple[index];
	itr->setPyObject((PyObject*)new PyString(str, len)); // GCC is not going to like this
}

void PyTuple::set_int( const int index, const int number )
{
	if (index+1 > (int)mTuple.size())
		mTuple.resize(index+1);
	PyChameleon * itr = mTuple[index];
	itr->setPyObject((PyObject*)new PyInt(number));
}

void PyTuple::set_long( const int index, const long number )
{
	if (index+1 > (int)mTuple.size())
		mTuple.resize(index+1);
	PyChameleon * itr = mTuple[index];
	itr->setPyObject((PyObject*)new PyLong((int64)number));
}

/************************************************************************/
/* PyList                                                               */
/************************************************************************/
PyList::PyList() : mType(PyTypeList), mRefcnt(1)
{
	mHash = &PyList::_hash;
}

PyList::PyList( int elementCount ) : mType(PyTypeList), mRefcnt(1)
{
	mHash = &PyList::_hash;
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
	mType = PyTypeDeleted;
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

uint8 PyList::gettype()
{
	return mType;
}

void PyList::IncRef()
{
	mRefcnt++;
}

void PyList::DecRef()
{
	mRefcnt--;
	if (mRefcnt <= 0)
		PyDelete(this);
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
	return (this->*mHash)();
}

uint32 PyList::_hash()
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

/************************************************************************/
/* PyDict                                                               */
/************************************************************************/
PyDict::PyDict() : mType(PyTypeDict), mMappingMode(true), mMappingIndex(0), mRefcnt(1)
{
	mHash = &PyDict::_hash;
}
PyDict::~PyDict()
{
	int i = 0;
	iterator itr = mDict.begin();
	for (; itr != mDict.end(); itr++)
	{
		PyDictEntry * entry = itr->second;
		assert(entry);
		assert(entry->key);
		assert(entry->obj);

		entry->key->DecRef();
		entry->obj->DecRef();
		SafeDelete(entry);
		i++;
	}

	mDict.clear();
	mType = PyTypeDeleted;
}

uint8 PyDict::gettype()
{
	return mType;
}

PyChameleon PyDict::operator[]( const char* keyName )
{
	ASCENT_HARDWARE_BREAKPOINT;
	if (keyName == NULL || *keyName == '\0')
		return PyErrorIterator;

	uint32 hsh = Utils::Hash::sdbm_hash(keyName);

	PyDictEntry * entry = mDict[hsh];
	if (entry == NULL)
	{
		entry = new PyDictEntry;
		entry->key = (PyObject*)new PyString(keyName);
		entry->obj = NULL;
		mDict[hsh] = entry;
	}

	return entry;
}

void PyDict::IncRef()
{
	mRefcnt++;
}

void PyDict::DecRef()
{
	mRefcnt--;
	if (mRefcnt <= 0)
		PyDelete(this);
}

size_t PyDict::size()
{
	return mDict.size();
}

bool PyDict::set_item( PyObject* key, PyObject* obj )
{
	if (key == NULL || obj == NULL)
	{
		ASCENT_HARDWARE_BREAKPOINT;
		return false;
	}

	if (mMappingMode == true)
	{
		/* create a new dictionary entry */
		PyDictEntry * entry = new PyDictEntry;
		entry->key = key;
		entry->obj = obj;
		mDict.insert(std::make_pair(mMappingIndex++, entry));
		key->IncRef();	// we seem to reuse a object that is already in the system so increase its mojo
		obj->IncRef();
	}
	else
	{
		// hit debug as I want to make sure its working correctly.
		ASCENT_HARDWARE_BREAKPOINT;

		uint32 hsh = PyObject_Hash(key);

		iterator itr = mDict.find(hsh);

		/* check if we are updating a dictionary entry or creating a new one */
		if (itr == mDict.end())
		{
			/* create a new dictionary entry */
			PyDictEntry * entry = new PyDictEntry;
			entry->key = key;
			entry->obj = obj;
			mDict.insert(std::make_pair(hsh, entry));
			key->IncRef();	// we seem to reuse a object that is already in the system so increase its mojo
			obj->IncRef();
		}
		else
		{
			/* update/replace a already existing entry ( bit tricky ) */
			PyDictEntry * entry = itr->second;
			entry->obj->DecRef();
			entry->obj = obj;
			obj->IncRef();
		}
	}
		
	return true;
}

PyObject* PyDict::get_item( PyObject* key )
{
	ASCENT_HARDWARE_BREAKPOINT;
	if (key == NULL)
		return NULL;
	return NULL;
}

uint32 PyDict::hash()
{
	return (this->*mHash)();
}

uint32 PyDict::_hash()
{
	uint32 hsh = 0;
	iterator itr = mDict.begin();
	for (; itr != mDict.end(); itr++)
	{
		PyDictEntry * entry = itr->second;
		hsh|= PyObject_Hash(entry->key);
		hsh = hsh << 3;
		hsh|= PyObject_Hash(entry->obj);
	}

	return hsh;
}

PyDict::iterator PyDict::begin()
{
	return mDict.begin();
}

PyDict::iterator PyDict::end()
{
	return mDict.end();
}

bool PyDict::scanf( const char * keyName, const char* format, ... )
{
	/* useless call.. because the dict need a lookup callback for this.... lol I actualy don't care.*/
	update();

	uint32 hsh = Utils::Hash::sdbm_hash(keyName);

	PyDict::iterator fItr = mDict.find(hsh);
	if (fItr == mDict.end())
		return false;
	
	PyObject * foundObject = ((PyDictEntry *)fItr->second)->obj;

	va_list ap;
	va_start(ap, format);

	//sum += i;
	//count++;
	//i = va_arg( ap, (void*));

	void* pVar = NULL;
	int formatIndex = 0;
	//while (pVar != -1)
	//{
		pVar = va_arg( ap, void*);

		char tag = format[formatIndex];
		switch(tag)
		{
			/*unicode string*/
		case 'u':
			std::wstring * str = (std::wstring *)pVar;
			str->clear();

			size_t len = ((PyUnicodeUCS2*)foundObject)->length();
			wchar_t * buff = ((PyUnicodeUCS2*)foundObject)->content();
			str->append(buff, len);
			break;
		}

		//formatIndex++;
	//}
	
	va_end(ap);
	return true;
}

void PyDict::update()
{
	if(mMappingMode == true)
	{
		//ASCENT_HARDWARE_BREAKPOINT;
		PyDictEntry * entry = NULL;
		iterator itr = mDict.begin();
		iterator dItr;

		size_t dictSize = mDict.size();
		for (size_t i = 0; i < dictSize; i++)
		{
			entry = itr->second;

			uint32 hsh = PyObject_Hash(entry->key);
			dItr = itr++;

			mDict.erase(dItr);
			mDict.insert(mDict.end(), std::make_pair(hsh, entry));
		}
		mMappingMode = false;
	}	
}

bool PyDict::get_buffer( const char * keyName, char* dst, size_t dst_len )
{
	if (keyName == NULL || dst == NULL || dst_len == 0)
		return false;

	update();

	uint32 hsh = Utils::Hash::sdbm_hash(keyName);

	PyDict::iterator fItr = mDict.find(hsh);
	if (fItr == mDict.end())
		return false;

	PyObject * foundObject = ((PyDictEntry *)fItr->second)->obj;

	if (foundObject == NULL)
		return false;

	if (foundObject->gettype() != PyTypeString)
		return false;

	PyString * str = (PyString *)foundObject;
	
	/* check if we have enough room to read the string / buffer */
	if (str->length() > dst_len)
		return false;

	ASCENT_MEMCPY(dst, str->content(), str->length());
	return true;
}

bool PyDict::set_int( const char * keyName, int number )
{
	return set_item(keyName, (PyObject *)new PyInt(number));
}

bool PyDict::set_double( const char * keyName, double number )
{
	return set_item(keyName, (PyObject *)new PyFloat(number));
}

bool PyDict::set_str( const char * keyName, const char* str )
{
	return set_item(keyName, (PyObject *)new PyString(str));
}

bool PyDict::set_bool( const char * keyName, bool check )
{
	return set_item(keyName, (PyObject *)new PyBool(check));
}

/************************************************************************/
/* PySubStream                                                          */
/************************************************************************/
PySubStream::PySubStream() : mType(PyTypeSubStream), mData(NULL), mLen(0), mRefcnt(1)
{
	mHash = &PySubStream::_hash;
}

PySubStream::PySubStream( uint8* data, size_t len ) : mType(PyTypeSubStream), mData(NULL), mLen(0), mRefcnt(1)
{
	mHash = &PySubStream::_hash;

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
	mType = PyTypeDeleted;
}

uint8 PySubStream::gettype()
{
	return mType;
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

void PySubStream::IncRef()
{
	mRefcnt++;
}

void PySubStream::DecRef()
{
	mRefcnt--;
	if (mRefcnt <= 0)
		PyDelete(this);
}

size_t PySubStream::size()
{
	return mLen;
}

uint32 PySubStream::hash()
{
	return (this->*mHash)();
}

uint32 PySubStream::_hash()
{
	return Utils::Hash::sdbm_hash((char*)mData, (int)mLen);
}

/************************************************************************/
/* PyClass                                                              */
/************************************************************************/
PyClass::PyClass() : mType(PyTypeClass), mDict(NULL), mName(NULL), mBases(NULL), mDirivedCallList(NULL), mDirivedCallDict(NULL), mRefcnt(1)
{
	mHash = &PyClass::_hash;
}

PyClass::~PyClass()
{
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
	
	if (mDirivedCallList)
	{
		mDirivedCallList->DecRef();
		mDirivedCallList = NULL;
	}

	if (mDirivedCallDict)
	{
		mDirivedCallDict->DecRef();
		mDirivedCallDict = NULL;
	}

	mType = PyTypeDeleted;
}

uint8 PyClass::gettype()
{
	return mType;
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
	ASCENT_ASSERT(mDirivedCallList == NULL);
	//assert(list->gettype() == PyTypeList);
	if (list == NULL)
		return false;
	mDirivedCallList = list;
	return true;
}

bool PyClass::setDirDict( PyDict * dict )
{
	ASCENT_ASSERT(mDirivedCallDict == NULL);
	if (dict == NULL)
		return false;
	mDirivedCallDict = dict;
	return true;
}

void PyClass::IncRef()
{
	mRefcnt++;
}

void PyClass::DecRef()
{
	mRefcnt--;
	if (mRefcnt <= 0)
		PyDelete(this);
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
	return mDirivedCallList;
}

PyDict * PyClass::getDirDict()
{
	return mDirivedCallDict;
}

uint32 PyClass::hash()
{
	return (this->*mHash)();
}

/**
 * @note this hash is fucked up.
 */
uint32 PyClass::_hash()
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
	
	if (mDirivedCallList)
	{
		hsh|= mDirivedCallList->hash();
		hsh = hsh << (mDirivedCallList->size() / 10);
	}

	if (mDirivedCallDict)
	{
		hsh|= mDirivedCallDict->hash();
		hsh = hsh << (mDirivedCallDict->size() / 10);
	}

	return hsh;
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

/************************************************************************/
/* PyPackedRow                                                          */
/************************************************************************/
PyPackedRow::PyPackedRow() : mType(PyTypePackedRow), mHeader(NULL), mRawFieldData(NULL), mRawFieldDataLen(0), mRefcnt(1)
{
	mHash = &PyPackedRow::_hash;
}

PyPackedRow::~PyPackedRow()
{
	iterator Itr = flowers.begin();
	for (; Itr != flowers.end(); Itr++)
	{
		delete Itr->second;
	}

	flowers.clear();

	mHeader->DecRef();

	if (mRawFieldData)
		SafeFree(mRawFieldData);
}

uint8 PyPackedRow::gettype()
{
	return mType;
}

void PyPackedRow::IncRef()
{
	mRefcnt++;
}

void PyPackedRow::DecRef()
{
	mRefcnt--;
	if (mRefcnt <= 0)
		PyDelete(this);
}

bool PyPackedRow::addleaf( PyObject* leaf )
{
	assert(leaf != NULL);
	
	// fucked up because of some c++ fuckup...
	uint32 hsh = PyObject_Hash(leaf);
	PyChameleon * itr = new PyChameleon();
	itr->setPyObject(leaf);

	flowers.insert(std::make_pair(hsh, itr));
	
	return true;
}

uint32 PyPackedRow::hash()
{
	return (this->*mHash)();
}

uint32 PyPackedRow::_hash()
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
		if(itr->second->getPyObject())
			hsh |= PyObject_Hash(itr->second->getPyObject());
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
/************************************************************************/
/* substruct                                                            */
/************************************************************************/
PySubStruct::PySubStruct() : mType(PyTypeSubStruct), payload(NULL), mRefcnt(1)
{
	mHash = &PySubStruct::_hash;
}

PySubStruct::~PySubStruct()
{
	if (payload)
		payload->DecRef();
}

uint8 PySubStruct::gettype()
{
	return mType;
}

void PySubStruct::IncRef()
{
	mRefcnt++;
}

void PySubStruct::DecRef()
{
	mRefcnt--;
	if (mRefcnt <= 0)
		PyDelete(this);
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

// generic caller
uint32 PySubStruct::hash()
{
	return (this->*mHash)();
}

// class related hash function
uint32 PySubStruct::_hash()
{
	ASCENT_HARDWARE_BREAKPOINT;
}

/************************************************************************/
/* PyModule                                                             */
/************************************************************************/
PyModule::PyModule() : mType(PyTypeModule), mModuleName(NULL), mRefcnt(1)
{
	mHash = &PyModule::_hash;
}

PyModule::~PyModule()
{

}

uint8 PyModule::gettype()
{
	return mType;
}

void PyModule::IncRef()
{
	mRefcnt++;
}

void PyModule::DecRef()
{
	mRefcnt--;
	if (mRefcnt <= 0)
		PyDelete(this);
}

uint32 PyModule::hash()
{
	return (this->*mHash)();
}

uint32 PyModule::_hash()
{
	ASCENT_HARDWARE_BREAKPOINT;
	return 0;
}

uint32 PyObject_Hash( PyObject* obj )
{
	return obj->hash();
}
