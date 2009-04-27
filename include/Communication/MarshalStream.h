#ifndef _MARSHALSTREAM_H
#define _MARSHALSTREAM_H

// hack?
#define BOOL bool
/**
 * \class MarshalStream
 *
 * @brief this class is a wrapper for the serialization and deserialization of the python marshal protocol.
 *
 * This class is a wrapper for the serialization and deserialization of the python marshal protocol.
 * For the moment the deserialization is quite complete but not perfect. The serialization still needs
 * loads of love and care but eventually it will get there.
 *
 * @author Captnoord
 * @date March 2009
 */
class MarshalStream
{
public:
	MarshalStream();
	~MarshalStream();

	/**
	 * \brief load loads and deserializes a marshal stream into python objects.
	 *
	 * 
	 *
	 * @param[in] stream a data stream containing the serialized data.
	 * @return the deserialized object or NULL when something went wrong.
	 */
	PyObject* load(ReadStream & stream);

	/**
	 * \brief save deserializes a PyObject into a marshal stream.
	 *
	 * 
	 *
	 * @param[in] object is the python object that requires to be serialized.
	 * @param[out] stream is the data stream containing the serialized object.
	 * @return true if successful and false when something went wrong.
	 */
	bool save(PyObject * object, WriteStream & stream);

	/* common packet python variable that needs to be public, I think */
	PyBaseNone PyNone;

	/* utility function */
	PyBaseNone* GetPyNone();

protected:
	
	/* common packet python variables */
	PyInt PyIntZero;
	PyInt PyIntOne;
	PyInt PyIntMinusOne;

	PyFloat PyFloatZero;
	PyBool	Py_TrueStruct;
	PyBool	Py_ZeroStruct;

	PyString PyStringEmpty;

	/* container to keep track of the referenced objects */
	UnmarshalReferenceMap mReferencedObjectsMap;

private:
	/**
	 * \brief unmarshal this function deserializes the python stream.
	 *
	 * this functions parses the data to objects.
	 *
	 * @param[in] stream is the data read stream that contains the data and keeps info about read index and stream size.
	 * @return returns the deserialized PyObject and if there was a error it returns NULL.
	 */
	PyObject* unmarshal(ReadStream & stream);

	/**
	 * \brief marshal this function attempts to serialize the python objects to a data stream.
	 *
	 * 
	 *
	 * @param[in] object is the object that needs to be serialized.
	 * @param[out] stream is the data stream that contains the serialized object.
	 * @return true if successful and false if a error has happened.
	 */
	bool marshal(PyObject * object, WriteStream & stream);

	/**
	 * @brief checkAndInflate is a temp function to handle the possibility that a packets is zipped
	 *
	 * this should really be done different.
	 *
	 * @param[in] stream is the data stream that contains the data that needs to be checked and inflated.
	 * @return true is check is successfully.. false means something went wrong...
	 */
	bool checkAndInflate( ReadStream & stream );

	/**
	 * \brief ReadMarshalHeader is a internal function that reads the marshal header.
	 *
	 * ReadMarshalHeader is a internal function that reads the marshal header and parses the reference
	 * object data. It also checks if the stream contains the required first tag.
	 *
	 * @param[in] stream is the data stream that contains the header that needs to be read.
	 * @return returns true if successful and false if it wasn't
	 */
	bool ReadMarshalHeader(ReadStream & stream);

	/**
	 * @brief ReadClassString reads the interface string and returns its instance.
	 * 
	 * ReadClassString is actually a function that needs to read the string and find the
	 * global interface instance and return it.
	 *
	 * @param[in] stream the data stream.
	 * @param[in] shared is the object this is true, meaning it should be saved.
	 * @return the deserialized object and NULL if it failed.
	 */
	ASCENT_INLINE PyObject* ReadClassString(ReadStream & stream, BOOL shared);

	/**
	 * \brief ReadClass reads the class instance data.
	 *
	 * no long description
	 *
	 * @param[in] stream the data stream.
	 * @param[in] shared is the object this is true, meaning it should be saved.
	 * @return the deserialized object and NULL if it failed.
	 */
	ASCENT_INLINE PyObject* ReadClass(ReadStream & stream, BOOL shared);

	/**
	 * \brief
	 *
	 * no long description
	 *
	 * @param[in]
	 * @return the deserialized object and NULL if it failed.
	 */
	ASCENT_INLINE PyObject* ReadCallUpdateObject(ReadStream & stream, BOOL shared);

	/**
	 * \brief
	 *
	 * no long description
	 *
	 * @param[in]
	 * @return the deserialized object and NULL if it failed.
	 */
	ASCENT_INLINE PyObject* ReadCallObjectNew(ReadStream & stream, BOOL shared);

	/**
	 * \brief
	 *
	 * no long description
	 *
	 * @param[in]
	 * @return the deserialized object and NULL if it failed.
	 */
	ASCENT_INLINE PyObject* ReadPackedRow(ReadStream & stream);

	/**
	 * \brief
	 *
	 * no long description
	 *
	 * @param[in]
	 * @return the deserialized object and NULL if it failed.
	 */
	ASCENT_INLINE PyObject* ReadSubStream(ReadStream & stream);

	/**
	 * \brief
	 *
	 * no long description
	 *
	 * @param[in]
	 * @return the deserialized object and NULL if it failed.
	 */
	ASCENT_INLINE PyObject* ReadVarInteger(ReadStream & stream, BOOL shared);

	/**
	 * \brief ReadBuffer reads the SizeEx chunk of the stream and reads the string accordingly.
	 *
	 * no long description
	 *
	 * @param[in] 
	 * @return a PyString pointer if successful and NULL if it wasn't.
	 */
	ASCENT_INLINE PyObject* ReadBuffer(ReadStream & stream);

	/**
	 * \brief
	 *
	 * no long description
	 *
	 * @param[in]
	 * @return the deserialized new object list and NULL if it failed.
	 */
	ASCENT_INLINE bool ReadNewObjList(ReadStream & stream, PyClass & obj);

	/**
	 * \brief
	 *
	 * no long description
	 *
	 * @param[in]
	 * @return the deserialized new object dict and NULL if it failed.
	 */
	ASCENT_INLINE bool ReadNewObjDict(ReadStream & stream, PyClass & obj);


	/**
	 * \brief WriteVarInteger is a wrapper function to write a variable integer to the a stream.
	 *
	 * WriteVarInteger is a wrapper function to write a variable integer to the a stream.
	 *
	 * @param[in] stream is the buffer to be written to.
	 * @param[in] number is the PyLong object that needs to be written to the stream.
	 * @return true if successful and false if a error has happened.
	 */
	ASCENT_INLINE bool WriteVarInteger(WriteStream& stream, PyObject* number);

	/** ZLIB utility buffer
	 */
	uint8 *zlibworkbuffer;
};

#endif //_MARSHALSTREAM_H
