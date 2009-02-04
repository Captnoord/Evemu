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
	PyObject* load(PyReadStream & stream);
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

	PyObject* ReadBuffer(PyReadStream & stream);

	bool ReadMarshalHeader(PyReadStream & stream);

	/**
	 * @brief ReadClassString
	 *
	 * needs some love
	 *
	 * @param[in]
	 * @param[out]
	 * @return
	 */
	PyObject* ReadClassString(PyReadStream & stream, BOOL shared);
	PyObject* ReadClass(PyReadStream & stream, BOOL shared);
	PyObject* ReadNewObject1(PyReadStream & stream, BOOL shared);
	PyObject* ReadNewObject2(PyReadStream & stream, BOOL shared);
	PyObject* ReadPackedRow(PyReadStream & stream);
	PyObject* ReadSubStream(PyReadStream & stream);
	PyObject* ReadVarInteger(PyReadStream & stream, BOOL shared);

	//////////////////////////////////////////////////////////////

	/* small internal function to do some easy reading */
	PyObject* _ReadPyStringFromStringAndSize(PyReadStream & stream);	

	//PyStream	marshalStream;			// container for keeping a marshaled stream
	PyObject*	unmarshalObjects;		// container for keeping the unmarshalled objects

	bool _deleted;
};

#endif //_MARSHALSTREAM_H
