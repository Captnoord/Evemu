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

PyChameleon &PyChameleon::operator=(const char* const str)
{
	_settype(PyTypeString);
	mPayload = (void*)new PyString(str);
	mIsNew = true;
	return *this;
}

PyChameleon &PyChameleon::operator=(const std::string &str)
{
	_settype(PyTypeString);
	mPayload = (void*)new PyString(str.c_str(), str.size());
	mIsNew = true;
	return *this;
}

PyChameleon &PyChameleon::operator=(const uint32 number)
{
	_settype(PyTypeInt);
	mPayload = (void*)new PyInt(number);
	mIsNew = true;
	return *this;
}

PyChameleon &PyChameleon::operator=(const int number)
{
	_settype(PyTypeInt);
	mPayload = (void*)new PyInt(number);
	mIsNew = true;
	return *this;
}

PyChameleon &PyChameleon::operator=(const float number)
{
	_settype(PyTypeReal);
	mPayload = (void*)new PyFloat(number);
	mIsNew = true;
	return *this;
}

PyChameleon &PyChameleon::operator=(const PyDict* pointer)
{
	_settype(PyTypeDict);
	mPayload = (void*)pointer;
	return *this;
}

PyChameleon &PyChameleon::operator=(const PyList* pointer)
{
	_settype(PyTypeDict);
	mPayload = (void*)pointer;
	return *this;
}

PyChameleon &PyChameleon::operator=(const PyTuple* pointer)
{
	_settype(PyTypeTuple);
	mPayload = (void*)pointer;
	return *this;
}

PyChameleon &PyChameleon::operator=(const PyObject* pointer)
{
	PyObject *xxx = *((PyObject**)&pointer);
	uint8 xtype = xxx->gettype();
	_settype(xtype);
	mPayload = (void*)pointer;
	return *this;
}

PyChameleon &PyChameleon::operator=(PyObject& pointer)
{

	return *this;
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

	mStr = (char*)malloc(len+1);
	strncpy(mStr, str, len);
	mStr[len] = '\0';
}

PyString::PyString(const char* str, size_t len) : type(PyTypeString)
{
	if (str == NULL)
		return;

	if (len == 0)
		return;

	mStr = (char*)malloc(len+1);
	strncpy(mStr, str, len);
	mStr[len] = '\0';
}

PyString::PyString(std::string& str) : type(PyTypeString)
{
	if (str.size() == 0)
		return;

	mStr = (char*)malloc(str.size()+1);
	strncpy(mStr, str.c_str(), str.size());
	mStr[str.size()] = '\0';
}

PyString::~PyString()
{
	if (mStr != NULL)
		free(mStr);
	mStr = NULL;
	type = PyTypeDeleted;
}

bool PyString::set(const char* str, size_t len)
{
	if (str == NULL || len == 0)
		return false;

	if (mStr != NULL)
		mStr = (char*)realloc(mStr, len+1);
	else
		mStr = (char*)malloc(len+1);
	
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

PyUnicodeUCS2::PyUnicodeUCS2() : type(PyTypeUnicode), mStr(NULL) {}
PyUnicodeUCS2::PyUnicodeUCS2(const wchar_t* str) : type(PyTypeUnicode)
{
	size_t len = wcslen(str);
	mStr = (wchar_t*)malloc((len+1) * 2);
	memcpy(mStr, str, (len+1)*2);
}

PyUnicodeUCS2::PyUnicodeUCS2(const wchar_t* str, size_t len) : type(PyTypeUnicode)
{
	if (str == NULL || len == 0)
	{
		mStr = NULL;
		return;
	}

	mStr = (wchar_t*)malloc((len+1) * 2);
	memcpy(mStr, str, (len+1)*2);
}

PyUnicodeUCS2::PyUnicodeUCS2(std::wstring& str) : type(PyTypeUnicode)
{
	mStr = (wchar_t*)malloc((str.size()+1) * 2);
	memcpy(mStr, str.c_str(), (str.size()+1)*2);
}

PyUnicodeUCS2::~PyUnicodeUCS2()
{
	if (mStr != NULL)
		free(mStr);
	mStr = NULL;
	type = PyTypeDeleted;
}

bool PyUnicodeUCS2::set(const char* str, size_t len)
{
	if (str == NULL || len == 0)
		return false;

	if (mStr != NULL)
		mStr = (wchar_t*)realloc(mStr,(len+1) * 2);
	else
		mStr = (wchar_t*)malloc((len+1) * 2);

	memcpy(mStr, str, (len+1)*2);
	return true;
}

bool PyUnicodeUCS2::resize(size_t newsize)
{
	if (mStr != NULL)
		mStr = (wchar_t*)realloc(mStr,(newsize+1) * 2);
	else
		mStr = (wchar_t*)malloc((newsize+1) * 2);
	
	if (mStr != NULL)
		return true;
	return false;
}

wchar_t* PyUnicodeUCS2::content()
{
	return mStr;
}
