#ifndef _MARSHALSTREAM_H
#define _MARSHALSTREAM_H

class PyObject;
class PyStream;
class MarshalReadStream;
class UnmarshalReferenceMap;

#include "MarshalReferenceMap.h"

/* although I don't like it we need a marshal stream class.. */
class MarshalStream
{
public:
	MarshalStream();
	~MarshalStream();
	PyObject* load(MarshalReadStream & stream);
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
	PyObject* unmarshal(MarshalReadStream & stream);

	PyObject* ReadBuffer(MarshalReadStream & stream);

	bool ReadMarshalHeader(MarshalReadStream & stream);

	/**
	 * @brief ReadClassString
	 *
	 * needs some love
	 *
	 * @param[in]
	 * @param[out]
	 * @return
	 */
	PyObject* ReadClassString(MarshalReadStream & stream, BOOL shared);
	PyObject* ReadClass(MarshalReadStream & stream, BOOL shared);
	PyObject* ReadNewObject1(MarshalReadStream & stream, BOOL shared);
	PyObject* ReadNewObject2(MarshalReadStream & stream, BOOL shared);
	PyObject* ReadPackedRow(MarshalReadStream & stream);
	PyObject* ReadSubStream(MarshalReadStream & stream);
	PyObject* ReadVarInteger(MarshalReadStream & stream, BOOL shared);

	//////////////////////////////////////////////////////////////

	/* small internal function to do some easy reading */
	PyObject* _ReadPyStringFromStringAndSize(MarshalReadStream & stream);	

	//PyStream	marshalStream;			// container for keeping a marshaled stream
	PyObject*	unmarshalObjects;		// container for keeping the unmarshalled objects

	bool _deleted;

	bool _ReadNewObjList(MarshalReadStream & stream, PyClass & obj);
	bool _ReadNewObjDict(MarshalReadStream & stream, PyClass & obj);

	/**
	 * @brief checkAndInflate is a temp function to handle the possibility that a packets is zipped
	 *
	 * this should really be done somewhere else.
	 *
	 * @return true is check is successfully.. false means something went wrong...
	 */
	bool checkAndInflate( MarshalReadStream & stream );
	uint8 zlibworkbuffer[100000];
};

#endif //_MARSHALSTREAM_H
