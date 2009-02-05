#include "ascent.h"

#include "Common.h"
#include "NGLog.h"
#include "Log.h"
#include "string_map.h"
#include "PyObjects.h"

/************************************************************************/
/* PyChameleon code                                                     */
/************************************************************************/
PyChameleon::PyChameleon()
{
	mIsEmpty = true;
	mType = PyTypeNone;
	mIsNew = false;
}

PyChameleon::~PyChameleon()
{
	OnDelete();
	mType = PyTypeDeleted;
	mPayload = NULL;
}

PyChameleon &PyChameleon::operator=(const char* const str)
{
	_settype(PyTypeString);
	mPayload = (void*)new PyString(str);
	mIsNew = true;
	mIsEmpty = false;
	return *this;
}

PyChameleon &PyChameleon::operator=(const std::string &str)
{
	_settype(PyTypeString);
	mPayload = (void*)new PyString(str.c_str(), str.size());
	mIsNew = true;
	mIsEmpty = false;
	return *this;
}

PyChameleon &PyChameleon::operator=(const uint32 number)
{
	_settype(PyTypeInt);
	mPayload = (void*)new PyInt(number);
	mIsNew = true;
	mIsEmpty = false;
	return *this;
}

PyChameleon &PyChameleon::operator=(const int number)
{
	_settype(PyTypeInt);
	mPayload = (void*)new PyInt(number);
	mIsNew = true;
	mIsEmpty = false;
	return *this;
}

PyChameleon &PyChameleon::operator=(const float number)
{
	_settype(PyTypeReal);
	mPayload = (void*)new PyFloat(number);
	mIsNew = true;
	mIsEmpty = false;
	return *this;
}

PyChameleon &PyChameleon::operator=(const PyDict* pointer)
{
	_settype(PyTypeDict);
	mPayload = (void*)pointer;
	mIsEmpty = false;
	return *this;
}

PyChameleon &PyChameleon::operator=(const PyList* pointer)
{
	_settype(PyTypeDict);
	mPayload = (void*)pointer;
	mIsEmpty = false;
	return *this;
}

PyChameleon &PyChameleon::operator=(const PyTuple* pointer)
{
	_settype(PyTypeTuple);
	mPayload = (void*)pointer;
	mIsEmpty = false;
	return *this;
}

PyChameleon &PyChameleon::operator=(const PyObject* pointer)
{
	PyObject *xxx = *((PyObject**)&pointer);
	uint8 xtype = xxx->gettype();
	_settype(xtype);
	mPayload = (void*)pointer;
	mIsEmpty = false;
	return *this;
}

PyChameleon &PyChameleon::operator=(PyObject& pointer)
{
	ASCENT_HARDWARE_BREAKPOINT;
	mIsEmpty = false;
	return *this;
}

bool PyChameleon::OnDelete()
{
	if (mIsNew != true)
		return false;

	switch(mType)
	{
	case PyTypeNone:
		delete ((PyBaseNone*)mPayload);
		return true;
	case PyTypeBool:
		delete ((PyBool*)mPayload);
		return true;
	case PyTypeInt:
		delete ((PyInt*)mPayload);
		return true;
	case PyTypeReal:
		delete ((PyFloat*)mPayload);
		return true;
	case PyTypeString:
		delete ((PyString*)mPayload);
		return true;
	case PyTypeUnicode:
		delete ((PyUnicodeUCS2*)mPayload);
		return true;
	case PyTypeDict:
		delete ((PyDict*)mPayload);
		return true;
	case PyTypeTuple:
		delete ((PyTuple*)mPayload);
		return true;
	case PyTypeList:
		delete ((PyList*)mPayload);
		return true;
	case PyTypeSubStream:
		delete ((PySubStream*)mPayload);
		return true;
	case PyTypeClass:
		delete ((PyClass*)mPayload);
		return true;
	default:
		return false;
	}
	return false;
}

uint8 PyChameleon::gettype()
{
	return mType;
}

bool PyChameleon::isempty()
{
	return mIsEmpty;
}

void PyChameleon::_settype( uint8 type )
{
	assert(mType == PyTypeNone); // O we don't want to reuse the object so only 1 thingy...
	mType = type;
}
/************************************************************************/
/* PyString code                                                        */
/************************************************************************/
PyString::PyString() : type(PyTypeString), mStr(NULL) {}
PyString::PyString(const char* str) : type(PyTypeString)
{
	if (str == NULL)
		return;

	size_t len = strlen(str);
	if (len == 0)
		return;

	mStr = (char*)ASCENT_MALLOC(len+1);
	strncpy(mStr, str, len);
	mStr[len] = '\0';
}

PyString::PyString(const char* str, size_t len) : type(PyTypeString)
{
	if (str == NULL)
		return;

	if (len == 0)
		return;

	mStr = (char*)ASCENT_MALLOC(len+1);
	strncpy(mStr, str, len);
	mStr[len] = '\0';
}

PyString::PyString(std::string& str) : type(PyTypeString)
{
	if (str.size() == 0)
		return;

	mStr = (char*)ASCENT_MALLOC(str.size()+1);
	strncpy(mStr, str.c_str(), str.size());
	mStr[str.size()] = '\0';
}

PyString::~PyString()
{
	if (mStr != NULL)
		ASCENT_FREE(mStr);
	mStr = NULL;
	type = PyTypeDeleted;
}

bool PyString::set(const char* str, size_t len)
{
	if (str == NULL || len == 0)
		return false;

	if (mStr != NULL)
		mStr = (char*)ASCENT_REALLOC(mStr, len+1);
	else
		mStr = (char*)ASCENT_MALLOC(len+1);
	
	if (mStr == NULL)
		return false;

	strncpy(mStr, str, len);
	mStr[len] = '\0';
	return true;
}

const char* PyString::content()
{
	return mStr;
}

uint8 PyString::gettype()
{
	return type;
}
PyUnicodeUCS2::PyUnicodeUCS2() : type(PyTypeUnicode), mStr(NULL) {}
PyUnicodeUCS2::PyUnicodeUCS2(const wchar_t* str) : type(PyTypeUnicode)
{
	size_t len = wcslen(str);
	mStr = (wchar_t*)ASCENT_MALLOC((len+1) * 2);
	ASCENT_MEMCPY(mStr, str, (len+1)*2);
}

PyUnicodeUCS2::PyUnicodeUCS2(const wchar_t* str, size_t len) : type(PyTypeUnicode)
{
	if (str == NULL || len == 0)
	{
		mStr = NULL;
		return;
	}

	ASCENT_HARDWARE_BREAKPOINT;
	mStr = (wchar_t*)ASCENT_MALLOC((len+1) * 2);
	ASCENT_MEMCPY(mStr, str, (len+1)*2);
}

PyUnicodeUCS2::PyUnicodeUCS2(std::wstring& str) : type(PyTypeUnicode)
{
	ASCENT_HARDWARE_BREAKPOINT;
	mStr = (wchar_t*)ASCENT_MALLOC((str.size()+1) * 2);
	ASCENT_MEMCPY(mStr, str.c_str(), (str.size()+1)*2);
}

PyUnicodeUCS2::~PyUnicodeUCS2()
{
	if (mStr != NULL)
		ASCENT_FREE(mStr);
	mStr = NULL;
	type = PyTypeDeleted;
}

bool PyUnicodeUCS2::set(const char* str, size_t len)
{
	if (str == NULL || len == 0)
		return false;

	if (mStr != NULL)
		mStr = (wchar_t*)ASCENT_REALLOC(mStr,(len+1) * 2);
	else
		mStr = (wchar_t*)ASCENT_MALLOC((len+1) * 2);

	ASCENT_MEMCPY(mStr, str, (len+1)*2);
	return true;
}

bool PyUnicodeUCS2::resize(size_t newsize)
{
	if (mStr != NULL)
		mStr = (wchar_t*)ASCENT_REALLOC(mStr,(newsize+1) * 2);
	else
		mStr = (wchar_t*)ASCENT_MALLOC((newsize+1) * 2);
	
	if (mStr != NULL)
		return true;
	return false;
}

wchar_t* PyUnicodeUCS2::content()
{
	return mStr;
}

uint8 PyUnicodeUCS2::gettype()
{
	return type;
}
PyInt::PyInt( int32 num, bool infinite /*= false*/ ) : isInfinite(isInfinite)
{
	type = PyTypeInt;
	number = num;
}

PyInt & PyInt::operator=( const int num )
{
	number = num;
	return *this;
}

PyInt::~PyInt()
{
	type = PyTypeDeleted;
}

/************************************************************************/
/* PyLong                                                               */
/************************************************************************/
PyLong::PyLong( int64 & num ) : number(num), type(PyTypeInt) {}
PyLong::~PyLong() { type = PyTypeDeleted; }

uint8 PyLong::gettype()
{
	return type;
}

int64 PyLong::getnumber()
{
	return number;
}

/************************************************************************/
/* PyFloat                                                              */
/************************************************************************/
PyFloat::PyFloat() : type(PyTypeReal), number(0.0) {}
PyFloat::PyFloat( float num ) : type(PyTypeReal), number(num) {}
PyFloat::PyFloat( double & num ) : type(PyTypeReal), number(num) {}

PyFloat::~PyFloat()
{
	type = PyTypeDeleted;
}

uint8 PyFloat::gettype()
{
	return type;
}

/************************************************************************/
/* PyBool                                                               */
/************************************************************************/
PyBool::PyBool( bool check ) : type(PyTypeBool), mCheck(check) {}

PyBool::~PyBool()
{
	type = PyTypeDeleted;
}

uint8 PyBool::gettype()
{
	return type;
}

/************************************************************************/
/* PyTuple                                                              */
/************************************************************************/
PyTuple::PyTuple() : type(PyTypeTuple) {}
PyTuple::PyTuple( int elementCount ) : type(PyTypeTuple)
{
	if (elementCount > PY_TUPLE_ELEMENT_MAX)
	{
		Log.Error("PyTuple", "constructor is requested to allocate a stupid amount of elements: %d", elementCount);
		assert(false);
		return;
	}
	
	mTuple.resize(elementCount);
}

PyTuple::~PyTuple()
{
	type = PyTypeDeleted;
}

PyChameleon & PyTuple::operator[]( const int index )
{
	if (index < 0)
		return PyErrorIterator;
	if (index > (int)mTuple.size())
		mTuple.resize(index);
	return mTuple[index];
}

size_t PyTuple::size()
{
	return mTuple.size();
}

uint8 PyTuple::gettype()
{
	return type;
}

/************************************************************************/
/* PyList                                                               */
/************************************************************************/
PyList::PyList() : type(PyTypeList) {}
PyList::PyList( int elementCount ) : type(PyTypeList)
{
	mList.resize(elementCount);
}

PyList::~PyList()
{
	type = PyTypeDeleted;
}

PyChameleon & PyList::operator[]( const int index )
{
	if (index < 0)
		return PyErrorIterator;
	if (index > (int)mList.size())
		mList.resize(index);
	return mList[index];
}

uint8 PyList::gettype()
{
	return type;
}

/************************************************************************/
/* PyDict                                                               */
/************************************************************************/
PyDict::PyDict() : type(PyTypeDict) {}
PyDict::~PyDict()
{
	type = PyTypeDeleted;
}

uint8 PyDict::gettype()
{
	return type;
}

PyChameleon & PyDict::operator[]( const char* keyName )
{
	if (keyName == NULL || *keyName == '\0')
		return PyErrorIterator;

	return mDict[keyName];
}

/************************************************************************/
/* PySubStream                                                          */
/************************************************************************/
PySubStream::PySubStream() : type(PyTypeSubStream), mData(NULL), mLen(0) {}
PySubStream::PySubStream( uint8* data, size_t len ) : type(PyTypeSubStream), mData(NULL), mLen(0)
{
	if (data == NULL)
		return;

	if (len == 0)
		return;

	mLen = len;
	mData = ASCENT_MALLOC(mLen);
	ASCENT_MEMCPY(mData, data, mLen);
}

PySubStream::~PySubStream()
{
	if (mData != NULL)
		SafeFree(mData);

	mLen = 0;
	type = PyTypeDeleted;
}

uint8 PySubStream::gettype()
{
	return type;
}

uint8* PySubStream::content()
{
	return (uint8*)mData;
}

bool PySubStream::set( uint8 * data, size_t len )
{
	mLen = len;
	if (mData != NULL)
		mData = ASCENT_REALLOC(mData, mLen);
	else
		mData = ASCENT_MALLOC(mLen);

	if (mData == NULL)
		return false;

	ASCENT_MEMCPY(mData, data, mLen);
	return true;
}

/************************************************************************/
/* PyClass                                                              */
/************************************************************************/
PyClass::PyClass() : type(PyTypeClass), mDict(NULL), mName(NULL), mBases(NULL), mDirivedCallList(NULL), mDirivedCallDict(NULL) {}

PyClass::~PyClass()
{
	type = PyTypeDeleted;
}

uint8 PyClass::gettype()
{
	return type;
}

bool PyClass::setname( PyString* name )
{
	ASCENT_ASSERT(mName == NULL);
	if (name == NULL)
		return false;
	mName = name;
	return true;
}

bool PyClass::setbases( PyTuple* tuple )
{
	ASCENT_ASSERT(mBases == NULL);
	if (tuple == NULL)
		return false;
	mBases = tuple;
	return true;
}

bool PyClass::setdict( PyDict* dict )
{
	ASCENT_ASSERT(mDict == NULL);
	if (dict == NULL)
		return false;
	mDict = dict;
	return true;
}

bool PyClass::setDirList( PyList * list )
{
	ASCENT_ASSERT(mDirivedCallList == NULL);
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
