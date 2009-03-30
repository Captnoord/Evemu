#include "EvemuPCH.h"
#include "DBRowModule.h"
#include "PyObjectDumper.h"

// experiment raw data

/*	DBRow::new
	arg: packet header

	if (header->type == tuple)
	{
		itrCount = header->size();
		if (itrCount == 0)
		{
			PyErr_SetString(PyExc_ValueError, "expected at least one argument");
			return 0;
		}
		//argument = (PyInstanceObject *)tHeader->ob_item[0]->ob_refcnt;
		argument = header[0][0];
	}
	else
	{
		
	}
*/

/* this stuff should be handled by the dbrowdescriptor I think.... I have no clue... */
PyObject* DBRowModule::parseraw( MarshalStream& stream, PyObject* header, uint8 * data, size_t len, size_t & virtualFieldCount, size_t & fieldCount )
{
	mReadIndex = 0;
	mReadBufferSize = len;
	mReadBuffer = data;
	mVirtualFieldCount = 0;

	PyClass * bas = (PyClass*)header;
	PyTuple & classdata = *bas->getbases();

	/* this seem to happen to the rowlist stuff.... debug stuff */
	if (bas->getbases() == NULL)
	{
		printf("hack stuff dbrowlist stuff \n");
		Dump(stdout, (PyObject*)bas, 0);
		return NULL;
	}

	assert(classdata.gettype() == PyTypeTuple);

	PyTuple& classdataterror = *((PyTuple*)classdata[1].getPyObject());
	PyTuple& tuple = *((PyTuple*)classdataterror[0].getPyObject());

	//Dump(stdout, (PyObject*)&tuple, 0);

	if (tuple.size() == 0)
		ASCENT_HARDWARE_BREAKPOINT;

	PyTuple & result_tuple = *PyTuple_New(tuple.size());
	// will go wrong
	for (int i = 0; i < (int)tuple.size(); i++)
	{
		PyTuple& field = (PyTuple&)tuple[i].getPyObjRef();

		/* this tuple is required to have 2 leaves because it has a field name and a field type */
		assert(field.size() == 2); // needs to be 2

		PyInt& fieldType = (PyInt&)field[1].getPyObjRef();
		PyObject* resObj = ReadRawDbField(stream, fieldType, mVirtualFieldCount);
		
		/*if (resObj != NULL)
			Dump(stdout, (PyObject*)resObj, 0);
		else
			printf("broked\n");*/
		
		if (resObj == NULL)
			continue;
		
		result_tuple[i] = resObj; // store the damn thing...
	}
	//Dump(stdout, (PyObject*)&result_tuple, 0);
	virtualFieldCount = mVirtualFieldCount;
	fieldCount = tuple.size();
	return (PyObject*)&result_tuple;
}

PyObject* DBRowModule::ReadRawDbField( MarshalStream& stream, PyInt &type, size_t &virtualFieldCount )
{
#define CHECK_SIZE(size) if(mReadIndex + size > mReadBufferSize) {\
	/*HexAsciiModule::print_hexview(stdout, (char*)mReadBuffer, mReadBufferSize);*/\
	assert(mReadIndex + size <= mReadBufferSize); }
#define MARSHALSTREAM_RETURN(p) {PyObject * x = ((PyObject*)p); assert(x != NULL); return x;}
//#define shizzle(x) printf("%s, index: 0x%X\n", #x, mReadIndex)
#define shizzle(x) /*x*/

	if (mReadIndex+1 > mReadBufferSize)
	{
		ASCENT_HARDWARE_BREAKPOINT;
		stream.PyNone.IncRef();
		return (PyObject*)&stream.PyNone;
	}

	switch(type.GetValue())
	{
	case DBTYPE_I1:
		{
			shizzle(DBTYPE_I1);
			CHECK_SIZE(1);
			int8 value = 0;
			value = mReadBuffer[mReadIndex];
			mReadIndex++;
			MARSHALSTREAM_RETURN(new PyInt(value));
		} break;
	case DBTYPE_UI1:
		{
			shizzle(DBTYPE_UI1);
			CHECK_SIZE(1);
			uint8 value = 0;
			value = mReadBuffer[mReadIndex];
			mReadIndex++;
			MARSHALSTREAM_RETURN(new PyInt(value));
		} break;
	case DBTYPE_BOOL: 
		{
			shizzle(DBTYPE_BOOL);
			CHECK_SIZE(1);
			uint8 value = 0;
			value = mReadBuffer[mReadIndex];
			mReadIndex++;
			MARSHALSTREAM_RETURN(new PyBool(value != 0));
		} break;
	case DBTYPE_I2: 
		{
			shizzle(DBTYPE_I2);
			CHECK_SIZE(2);
			int16 value = 0;
			value = *((int16*)&mReadBuffer[mReadIndex]);
			mReadIndex+=2;
			MARSHALSTREAM_RETURN(new PyInt(value));
		} break;
	case DBTYPE_UI2: 
		{
			shizzle(DBTYPE_UI2);
			CHECK_SIZE(2);
			int16 value = 0;
			value = *((int16*)&mReadBuffer[mReadIndex]);
			mReadIndex+=2;
			MARSHALSTREAM_RETURN(new PyInt(value));
		} break;
	case DBTYPE_I4: 
		{
			shizzle(DBTYPE_I4);
			CHECK_SIZE(4);
			int32 value = 0;
			value = *((int32*)&mReadBuffer[mReadIndex]);
			mReadIndex+=4;
			MARSHALSTREAM_RETURN(new PyInt(value));
		} break;
	case DBTYPE_UI4:
		{
			shizzle(DBTYPE_UI4);
			CHECK_SIZE(4);
			int32 value = 0;
			value = *((int32*)&mReadBuffer[mReadIndex]);
			mReadIndex+=4;
			MARSHALSTREAM_RETURN(new PyInt(value));
		} break;
	case DBTYPE_R4: 
		{
			shizzle(DBTYPE_R4);
			CHECK_SIZE(4);
			ASCENT_HARDWARE_BREAKPOINT;
		} break;
	case DBTYPE_I8:
		{
			shizzle(DBTYPE_I8);
			CHECK_SIZE(8);
			int64 value = 0;
			value = *((int64*)&mReadBuffer[mReadIndex]);
			mReadIndex+=8;
			MARSHALSTREAM_RETURN(new PyLong(value));
		} break;
	case DBTYPE_R8:
		{
			/* still researching this one */
			shizzle(DBTYPE_R8);
			CHECK_SIZE(8);
			double * value = (double*)&mReadBuffer[mReadIndex];
			
			mReadIndex+=8;

			MARSHALSTREAM_RETURN(new PyFloat(*value));
		} break;

	case DBTYPE_FILETIME:
	case DBTYPE_UI8:
		{
			shizzle(DBTYPE_UI8);
			CHECK_SIZE(8);
			uint64 value = 0;
			value = *((uint64*)&mReadBuffer[mReadIndex]);
			mReadIndex+=8;
			MARSHALSTREAM_RETURN(new PyLong(value));
		} break;

	/* @note i'm unsure if this is int32 or int64, on the other size it would be logic to send it as a int32 as it saves the space
	         but i'm not sure about it.
	*/
	case DBTYPE_CY: 
		{
			shizzle(DBTYPE_CY);
			CHECK_SIZE(8);

			/* still researching this one */

			uint64 henk = *((uint64*)&mReadBuffer[mReadIndex]);
			double sjaak = (double)henk;
			sjaak /= 10000.0;
			
			mReadIndex+=8;
			MARSHALSTREAM_RETURN(new PyFloat(sjaak));
		} break;

	case DBTYPE_BYTES:
	case DBTYPE_STR:
	case DBTYPE_WSTR:
		{
			shizzle(DBTYPE_BYTES);
			virtualFieldCount++;
			return NULL;
		} break;

	default:
		//assert(false);
		ASCENT_HARDWARE_BREAKPOINT;
		break;
	}
	ASCENT_HARDWARE_BREAKPOINT;
	return NULL;
}

size_t DBRowModule::GetRawFieldSizeFromHeader( PyObject* object )
{
	if (object == NULL)
		return -1;

	if (object->gettype() != PyTypeTuple)
		return -1;

	PyTuple * pTuple = (PyTuple*)object;
	if (pTuple->size() == 0)
		return -1;

	/* check if we need to get the sub object */
	if (pTuple->size() == 1)
	{
		PyObject* subObject = (*pTuple)[0].getPyObject();
		if (subObject == NULL)
			return -1;

		if (subObject->gettype() != PyTypeTuple)
			return -1;

		pTuple = (PyTuple*)subObject;
	}

	PyTuple & tuple = *pTuple;

	size_t chunkSize = 0;
	for (size_t i = 0; i < tuple.size(); i++)
	{
		PyTuple & leaf = *(PyTuple*)tuple[int(i)].getPyObject();
		if (leaf.size() != 2)
			ASCENT_HARDWARE_BREAKPOINT;

		PyInt & dbfieldtype = *(PyInt*)leaf[1].getPyObject();
		int32 fieldtype = dbfieldtype.GetValue();
		chunkSize+=GetFieldSizeFromType((uint8)fieldtype);
	}
	return chunkSize;
}

size_t DBRowModule::GetFieldSizeFromType( uint8 type )
{
	switch(type)
	{
	case DBTYPE_I1: return 1;
	case DBTYPE_UI1: return 1;
	//case DBTYPE_UI1: return 2;
	case DBTYPE_BOOL: return 1;
	case DBTYPE_I2: return 2;
	case DBTYPE_UI2: return 2;
	case DBTYPE_I4: return 4;
	case DBTYPE_UI4: return 4;
	case DBTYPE_R4: return 4;
	case DBTYPE_I8: return 8;
	case DBTYPE_R8: return 8;
	case DBTYPE_UI8: return 8;
	case DBTYPE_CY: return 8;
	case DBTYPE_FILETIME: return 8;
	case DBTYPE_BYTES: return 0;
	case DBTYPE_STR: return 0;
	case DBTYPE_WSTR: return 0;
	default:
		ASCENT_HARDWARE_BREAKPOINT;
		break;
	}
	return -1;
}