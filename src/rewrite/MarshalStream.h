#ifndef _MARSHALSTREAM_H
#define _MARSHALSTREAM_H

class PyObject;
class PyStream;
class PyReadStream;
class UnmarshalReferenceMap;

#include "MarshalReferenceMap.h"

/* although I don't like it we need a marshal stream class.. */
class MarshalStream
{
public:
	MarshalStream();
	~MarshalStream();
	void load(PyReadStream & stream);
	void save(PyObject * object);

	


protected:

	PyInt PyIntZero;
	PyInt PyIntOne;
	PyInt PyIntMinOne;
	PyInt PyIntInfinite;

	PyFloat PyFloatZero;
	PyBool	Py_TrueStruct;
	PyBool	Py_ZeroStruct;

	PyString PyStringEmpty;

	UnmarshalReferenceMap sharedObjectsMapInternal;
#define sharedObjectsMap (sharedObjectsMapInternal)
	
private:
	PyObject* unmarshal(PyReadStream & stream);

	void* readNewObject1(PyReadStream & stream, bool shared);
	void* readNewObject2(PyReadStream & stream, bool shared);

	PyObject* readVarInteger(PyReadStream & stream, bool shared);

	PyString* ReadBuffer(PyReadStream & stream);

	template<typename T>
	PyObject* return_fixer(T * object)
	{
		return (PyObject*)object;
	}

	bool ReadMarshalHeader(PyReadStream & stream);

	//PyStream	marshalStream;			// container for keeping a marshaled stream
	PyObject*	unmarshalObjects;		// container for keeping the unmarshalled objects

	bool _deleted;
};

#endif //_MARSHALSTREAM_H
