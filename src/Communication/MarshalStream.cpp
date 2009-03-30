#include "Common.h"
#include "ascent.h"

#include "EvemuPCH.h"

#include "EveMarshalOpcodes.h"
#include "PyStringTable.h"

#include <zlib.h>

#include "DBRowModule.h"

/* macro's that help debugging exceptions */
#define EVEMU_EXTRA_DEBUG
#ifdef _DEBUG
#  ifdef EVEMU_EXTRA_DEBUG
#    define MARSHALSTREAM_RETURN_NULL {ASCENT_HARDWARE_BREAKPOINT; return NULL;}
#    define MARSHALSTREAM_RETURN(p) {PyObject * x = ((PyObject*)p); assert(x != NULL); return x;}
//#  define MARSHALSTREAM_RETURN(p) {PyObject * x = ((PyObject*)p); assert(x != NULL); Dump(stdout, (PyObject*)x, 0); return x;}
//#  define MARSHALSTREAM_RETURN(p) {PyObject * x = ((PyObject*)p); assert(x != NULL); Dump(fp_debug, (PyObject*)x, 0); return x;}
#  else
#    define MARSHALSTREAM_RETURN_NULL return NULL;
#    define MARSHALSTREAM_RETURN(p) return (PyObject*)p;
# endif//EVEMU_EXTRA_DEBUG
#else
#  ifdef EVEMU_EXTRA_DEBUG
#    define MARSHALSTREAM_RETURN_NULL {sLog.String(__FUNCTION__" :returning NULL"); return NULL;}
#    define MARSHALSTREAM_RETURN(p) { PyObject * x = ((PyObject*)p); if (x == NULL) {sLog.Error(__FUNCTION__" :returning NULL");} return x; }
#  else
#    define MARSHALSTREAM_RETURN_NULL return NULL;
#    define MARSHALSTREAM_RETURN(p) return (PyObject*)p;
#  endif//EVEMU_EXTRA_DEBUG
#endif//_DEBUG

/* macro's that help debugging exceptions */
#ifdef _DEBUG
//#  define unmarshalState(x, y) {sLog.String("State:"#x"\toffset:0x%X", y.tell());}
#  define unmarshalState(x, y) /*{x, y}*/
#else
#  define unmarshalState(x, y) /*{x, y}*/
#endif//_DEBUG

MarshalStream::MarshalStream() : PyIntZero(0), PyIntOne(1), PyIntMinOne(-1), PyFloatZero(0.0),PyIntMinusOne(-1),
	Py_TrueStruct(true), Py_ZeroStruct(false) {}

MarshalStream::~MarshalStream() {}

PyObject* MarshalStream::load(ReadStream & stream)
{
	/* first time check for byte code because I don't want to contaminate checkAndInflate */
	if (checkForBytecode(stream) == true)
		return NULL;

	if (checkAndInflate(stream) == false)
		MARSHALSTREAM_RETURN_NULL;

	/* second time check for byte code because its possible it was zipped */
	if (checkForBytecode(stream) == true)
		return NULL;

	if (ReadMarshalHeader(stream) == false)
	{
		Log.Error("MarshalStream", "[load] Unable to read and initialize the packet header");
		return NULL;
	}

	PyObject* unmarshaledData = unmarshal(stream);
	MARSHALSTREAM_RETURN(unmarshaledData);
}

bool MarshalStream::ReadMarshalHeader( ReadStream & stream )
{
	char marshalTilde;
	if (!stream.read1(marshalTilde))
	{
		Log.Error("MarshalStream", "[ReadMarshalHeader]Can't read %d elements of %d bytes, only have %d bytes left", 1, 1, stream.size() - stream.tell());
		return false;
	}

	if (marshalTilde != '~')
	{
		//Log.Error("MarshalStream", "[ReadMarshalHeader]invalid marshal header, missing tilde");
		Log.Error("MarshalStream", "[ReadMarshalHeader]invalid marshal header, missing tilde: 0x%X", marshalTilde);
		return false;
	}

	int32 sharedObjectCount;
	if (!stream.read4(sharedObjectCount))
	{
		Log.Error("MarshalStream", "[ReadMarshalHeader]Can't read %d elements of %d bytes, only have %d bytes left", 1, 4, stream.size() - stream.tell());
		return false;
	}

	/* check if this packet contains referenced objects */
	if (sharedObjectCount <= 0)
		return true;

	mReferencedObjectsMap.SetOrderMapSize(sharedObjectCount);
	mReferencedObjectsMap.SetSharedObjectCount(sharedObjectCount);
	
	size_t tReadIndex2 = stream.tell();
	if ( (signed int)(stream.size() - tReadIndex2) / 4 < sharedObjectCount )
	{
		Log.Error("MarshalStream", "[ReadMarshalHeader]Too little data in marshal stream, %u bytes. I really wanted at least %u bytes total, mapcount in header is %d", stream.size(), tReadIndex2 + 4 * sharedObjectCount, sharedObjectCount);
		return false;
	}

	stream.setpayloadsize(stream.size() - (4 * sharedObjectCount));

	size_t sharedObjectIndex = stream.buffersize() - 4 * sharedObjectCount;

	int32* sharedObjectBegin = (int32*)&stream.content()[sharedObjectIndex];
	int32 loopcounter = 0;
	while ( *sharedObjectBegin >= 1 && *sharedObjectBegin <= sharedObjectCount )
	{
		mReferencedObjectsMap.SetObjectOrder(loopcounter, *sharedObjectBegin);
		++loopcounter;
		++sharedObjectBegin;
		
		if ( loopcounter >= sharedObjectCount )
			return true;
	}

	Log.Error("MarshalStream", "Bogus map data in marshal stream.");

	return false;
}

PyObject* MarshalStream::unmarshal( ReadStream & stream )
{
	if (stream.size() - stream.tell() >= 1)
	{
		uint8 opcode;
		stream.read1(opcode);
		//if (((opcode >> 6) & 0xFFFFFF01) != 0)
		//	printf("ref obj: [0X%x]\n", opcode & 0x3F);

		switch ( opcode & 0x3F )
		{
			case Op_PyNone:
			{
				unmarshalState(Op_PyNone, stream);
				PyNone.IncRef();
				MARSHALSTREAM_RETURN(&PyNone);
			}

			case Op_PyClassString:
			{
				unmarshalState(Op_PyClassString, stream);
				MARSHALSTREAM_RETURN(ReadClassString(stream, (opcode & 0x40) != 0));
			}

			case Op_PyLongLong:
			{
				unmarshalState(Op_PyLongLong, stream);
				int64 number;
				if (!stream.read8(number))
					MARSHALSTREAM_RETURN_NULL;
				MARSHALSTREAM_RETURN(PyLong_FromLongLong(number));
			}

			case Op_PyLong:
			{
				unmarshalState(Op_PyLong, stream);
				int32 number;
				if (!stream.read4(number))
					MARSHALSTREAM_RETURN_NULL;
				MARSHALSTREAM_RETURN(PyInt_FromLong(number));
			}

			case Op_PySignedShort:
			{
				unmarshalState(Op_PySignedShort, stream);
				int16 number;
				if (!stream.read2(number))
					MARSHALSTREAM_RETURN_NULL;
				MARSHALSTREAM_RETURN(PyInt_FromLong(number));
			}

			case Op_PyByte:
			{
				unmarshalState(Op_PyByte, stream);
				int8 number;
				if (!stream.read1(number))
					MARSHALSTREAM_RETURN_NULL;
				MARSHALSTREAM_RETURN(PyInt_FromLong(number));
			}

			// original code also has a reference counter of this object in the following cases.
			case Op_PyMinusOne:
			{
				unmarshalState(Op_PyMinusOne, stream);
				PyIntMinusOne.IncRef();
				MARSHALSTREAM_RETURN(&PyIntMinusOne);
			}
			
			case Op_PyZeroInteger:
			{
				unmarshalState(Op_PyZeroInteger, stream);
				PyIntZero.IncRef();
				MARSHALSTREAM_RETURN(&PyIntZero);
			}
			
			case Op_PyOneInteger:
			{
				unmarshalState(Op_PyOneInteger, stream);
				PyIntOne.IncRef();
				MARSHALSTREAM_RETURN(&PyIntOne);
			}
			
			case Op_PyFloat:
			{
				unmarshalState(Op_PyFloat, stream);
				double number;
				if (!stream.read8(number))
					MARSHALSTREAM_RETURN_NULL;
				MARSHALSTREAM_RETURN(PyFloat_FromDouble(number));
			}

			case Op_PyZeroFloat:
			{
				unmarshalState(Op_PyZeroFloat, stream);
				PyFloatZero.IncRef();
				MARSHALSTREAM_RETURN(&PyFloatZero);
			}
			
			case Op_PyTrue:
			{
				unmarshalState(Op_PyTrue, stream);
				Py_TrueStruct.IncRef();
				MARSHALSTREAM_RETURN(&Py_TrueStruct);
			}
			
			case Op_PyFalse:
			{
				unmarshalState(Op_PyFalse, stream);
				Py_ZeroStruct.IncRef();
				MARSHALSTREAM_RETURN(&Py_ZeroStruct);
			}
			
			case Op_PyLongString:
			{
				unmarshalState(Op_PyLongString, stream);
				MARSHALSTREAM_RETURN(ReadBuffer(stream));
			}
			
			case Op_PyEmptyString:
			{
				unmarshalState(Op_PyEmptyString, stream);
				PyStringEmpty.IncRef();
				MARSHALSTREAM_RETURN(&PyStringEmpty);
			}
			
			case Op_PyCharString:
			{
				unmarshalState(Op_PyCharString, stream);
				char CharString;
				if (!stream.read1(CharString))
					MARSHALSTREAM_RETURN_NULL;
				MARSHALSTREAM_RETURN(PyString_FromStringAndSize(&CharString, 1));
			}

			case Op_PyShortString:
			{
				unmarshalState(Op_PyShortString, stream);
				uint8 strlen;
				char* strptr;
				if (!stream.read1(strlen))
					MARSHALSTREAM_RETURN_NULL;
				if (!stream.readString(&strptr, strlen))
					MARSHALSTREAM_RETURN_NULL;
				MARSHALSTREAM_RETURN(PyString_FromStringAndSize(strptr, strlen));
			}

			case Op_PyStringTableItem:
			{
				unmarshalState(Op_PyStringTableItem, stream);
				uint8 index;
				if (!stream.read1(index))
					MARSHALSTREAM_RETURN_NULL;
	
				PyString * ret = NULL;
				if (!sPyStringTable.LookupPyString(index, ret) && ret == NULL)
				{
					Log.Error("MarshalStream", "Invalid string table index %d", index);
					MARSHALSTREAM_RETURN_NULL;
				}
				MARSHALSTREAM_RETURN(ret);
			}

			case Op_PyUnicodeString:
			{
				unmarshalState(Op_PyUnicodeString, stream);
				uint32 strlen;
				wchar_t* strptr;
				if (!stream.readSizeEx(strlen))
					MARSHALSTREAM_RETURN_NULL;
	
				if (!stream.readWstring(&strptr, strlen))
					MARSHALSTREAM_RETURN_NULL;
	
				MARSHALSTREAM_RETURN(PyUnicodeUCS2_FromWideChar(strptr, strlen));
			}

			case Op_PyUnicodeEmptyString:
			{
				unmarshalState(Op_PyUnicodeEmptyString, stream);
				MARSHALSTREAM_RETURN(PyUnicodeUCS2_FromWideChar(0, 0));
			}

			case Op_PyUnicodeCharString:
			{
				unmarshalState(Op_PyUnicodeCharString, stream);
				wchar_t singleWChar;
				if (!stream.read2(singleWChar))
					MARSHALSTREAM_RETURN_NULL;
				MARSHALSTREAM_RETURN(PyUnicodeUCS2_FromWideChar(&singleWChar, 1));
			}

			case Op_PyUnicodeUTF8String:
			{
				unmarshalState(Op_PyUnicodeUTF8String, stream);
				uint32 strlen;
				char* strptr;
				if (!stream.readSizeEx(strlen))
					MARSHALSTREAM_RETURN_NULL;
	
				if (!stream.readString(&strptr, strlen))
					MARSHALSTREAM_RETURN_NULL;
	
				MARSHALSTREAM_RETURN(PyUnicodeUCS2_DecodeUTF8(strptr, strlen));
			}

			/* binary blob */
			case Op_PyBuffer:
			{
				unmarshalState(Op_PyBuffer, stream);
				
				ASCENT_HARDWARE_BREAKPOINT;
				MARSHALSTREAM_RETURN_NULL;
			}

			case Op_PyEmptyTuple:
			{
				unmarshalState(Op_PyEmptyTuple, stream);
				MARSHALSTREAM_RETURN(PyTuple_New(0));
			}
			
			case Op_PyOneTuple:
			{
				unmarshalState(Op_PyOneTuple, stream);
				PyTuple& tuple = *PyTuple_New(1);
				if ((opcode & 0x40) != 0)
				{
					tuple.IncRef();
					if(!mReferencedObjectsMap.StoreReferencedObject(&tuple))
						MARSHALSTREAM_RETURN_NULL;
				}
	
				// warning recursive function...
				tuple[0] = unmarshal(stream);
				assert(tuple[0].getPyObject() != (PyObject*)&tuple);

				MARSHALSTREAM_RETURN(&tuple);
			}

			case Op_PyTwoTuple:
			{
				unmarshalState(Op_PyTwoTuple, stream);
				PyTuple& tuple = *PyTuple_New(2);
				if ((opcode & 0x40) != 0)
				{
					tuple.IncRef();
					if(!mReferencedObjectsMap.StoreReferencedObject(&tuple))
						MARSHALSTREAM_RETURN_NULL;
				}
	
				// warning double recursive function...
				for (int i = 0; i < 2; i++)
				{
					PyObject* Itr = unmarshal(stream);
					assert(Itr != (PyObject*)&tuple);
					tuple[i] = Itr;
				}

				MARSHALSTREAM_RETURN(&tuple);
			}
			
			case Op_PyTuple:
			{
				unmarshalState(Op_PyTuple, stream);
				uint32 elementCount;
				if (!stream.readSizeEx(elementCount))
					MARSHALSTREAM_RETURN_NULL;
	
				PyTuple& tuple = *PyTuple_New(elementCount);
				if ((opcode & 0x40) != 0)
				{
					tuple.IncRef();
					if(!mReferencedObjectsMap.StoreReferencedObject(&tuple))
						MARSHALSTREAM_RETURN_NULL;
				}
	
				// warning double recursive function...
				for (uint32 i = 0; i < elementCount; i++)
				{
					PyObject* Itr = unmarshal(stream);
					assert(Itr != (PyObject*)&tuple);
					tuple[i] = Itr;
				}

				MARSHALSTREAM_RETURN(&tuple);
			}

			case Op_PyEmptyList:
			{
				unmarshalState(Op_PyEmptyList, stream);
				MARSHALSTREAM_RETURN(PyList_New(0));
			}

			case Op_PyOneList:
			{
				unmarshalState(Op_PyOneList, stream);
				PyList &list = *PyList_New(1);
				if (&list == NULL)
					MARSHALSTREAM_RETURN_NULL;
				
				if ((opcode & 0x40) != 0)
				{
					list.IncRef();
					if(!mReferencedObjectsMap.StoreReferencedObject(&list))
						MARSHALSTREAM_RETURN_NULL;
				}
				// recursive function...
				list[0] = unmarshal(stream);
				assert(list[0].getPyObject() != (PyObject*)&list);

				MARSHALSTREAM_RETURN(&list);
			}

			case Op_PyList:
			{
				unmarshalState(Op_PyList, stream);
				uint32 elementCount;
				if (!stream.readSizeEx(elementCount))
					MARSHALSTREAM_RETURN_NULL;
	
				PyList &list = *PyList_New(elementCount);
				if (&list == NULL)
					MARSHALSTREAM_RETURN_NULL;
	
				if ((opcode & 0x40) != 0)
				{
					list.IncRef();
					if(!mReferencedObjectsMap.StoreReferencedObject(&list))
						MARSHALSTREAM_RETURN_NULL;
				}
	
				// n recursive function call
				for (uint32 i = 0; i < elementCount; i++)
				{
					PyObject* itr = unmarshal(stream);
					assert(itr != (PyObject*)&list);
					if (itr == NULL)
						MARSHALSTREAM_RETURN_NULL;

					list[i] = itr;
				}
				MARSHALSTREAM_RETURN(&list);
			}
			
			/* this handler is a lot more complicated than I have implemented here... will do for now... */
			case Op_PyDict:
			{
				unmarshalState(Op_PyDict, stream);
				uint32 elementCount;
				if (!stream.readSizeEx(elementCount))
					MARSHALSTREAM_RETURN_NULL;
	
				PyDict & dict = *PyDict_New();
				if ((opcode & 0x40) != 0)
				{
					dict.IncRef();
					if(!mReferencedObjectsMap.StoreReferencedObject(&dict))
						MARSHALSTREAM_RETURN_NULL;
				}
	
				// whoo tricky stuff....
				for (uint32 i = 0; i < elementCount; i++)
				{
					PyObject* keyPayload = unmarshal(stream);			// Payload
					PyObject* keyName = unmarshal(stream);				// the keyname

					assert(keyName);

					dict.set_item(keyName, keyPayload);

					keyName->DecRef();
				}

				MARSHALSTREAM_RETURN(&dict);
			}

			/* read a object, it also has a check for reference mapping */
			case Op_PyModuleInstance:
			{
				unmarshalState(Op_PyModuleInstance, stream);
				MARSHALSTREAM_RETURN(ReadClass(stream, (opcode >> 6) & 0xFFFFFF01));
			}

			/* unknown isn't handled but what we know of them is that its related to the cPickle system */
			case Op_PyBinairy:
			{
				unmarshalState(Op_PyBinairy, stream);
				MARSHALSTREAM_RETURN_NULL;
			}
			
			/* need to implement custom callbacks and reading stuff... but for the server this doesn't seem to usefull.. */
			case Op_PyClassInstance:
			{
				//Unmarshal stream contains custom data but I have no callback method

				unmarshalState(Op_PyClassInstance, stream);
				PySubStruct * obj = new PySubStruct();
				PyObject * tobj = unmarshal(stream);
				assert(obj->setPyObject(tobj));
				tobj->DecRef();
				
				MARSHALSTREAM_RETURN(obj);
			}

			/**/
			case Op_PyLoadcPickledObject:
			{
				unmarshalState(Op_PyLoadcPickledObject, stream);
				/* |extended size|stream| */
				//MARSHALSTREAM_RETURN(PyObject_CallMethod(unpickledObject, "load", 0);
				MARSHALSTREAM_RETURN_NULL;
			}
		
			case Op_cPicked:
			{
				unmarshalState(Op_cPicked, stream);
				MARSHALSTREAM_RETURN_NULL;
			}

			case Op_PySavedStreamElement:
			{
				unmarshalState(Op_PySavedStreamElement, stream);
				uint32 index;
				if (!stream.readSizeEx(index))
					MARSHALSTREAM_RETURN_NULL;
				PyObject* obj;
				if (!mReferencedObjectsMap.GetStoredObject(index,&obj))
				{
					Log.Error("MarshalStream", "(Op_PySavedStreamElement)there seems to be a Invalid TY_REFECENCE in the stream");
					MARSHALSTREAM_RETURN_NULL;
				}
				obj->IncRef();
				MARSHALSTREAM_RETURN(obj);
			}

			/* this is fun as check the clients packet hash.... */
			case Op_PyChecksumedStream:
			{
				unmarshalState(Op_PyChecksumedStream, stream);
				uint32 clientHash;
				if (!stream.read4(clientHash))
					MARSHALSTREAM_RETURN_NULL;

				Bytef* data = &stream.content()[stream.tell()];
				uInt len = (uInt)stream.buffersize() - stream.tell();
				uint32 serverHash = adler32(1, data, len);
				if (clientHash != serverHash)
				{
					Log.Error("MarshalStream", "(Op_PyChecksumedStream) the checksum fails... client send us a 'oepsie' or he's just plain stupid");
					MARSHALSTREAM_RETURN_NULL;
				}
				else
				{
					MARSHALSTREAM_RETURN(unmarshal(stream));
				}
			}

			case Op_UpdateObject:
				unmarshalState(Op_UpdateObject, stream);
				MARSHALSTREAM_RETURN(ReadCallUpdateObject(stream, (opcode >> 6) & 1));

			case Op_NewObject:
				unmarshalState(Op_NewObject, stream);
				MARSHALSTREAM_RETURN(ReadCallObjectNew(stream, (opcode >> 6) & 1));

			case Op_PyDBRow:
				unmarshalState(Op_PyDBRow, stream);
				MARSHALSTREAM_RETURN(ReadPackedRow(stream));

			case Op_PySubStream:
				unmarshalState(Op_PySubStream, stream);
				MARSHALSTREAM_RETURN(ReadSubStream(stream));

			case Op_PyVarInteger:
				unmarshalState(Op_PyVarInteger, stream);
				MARSHALSTREAM_RETURN(ReadVarInteger(stream, (opcode >> 6) & 1));

				/* iterator sequence stop tag */
			case Op_PyStopIter:
				MARSHALSTREAM_RETURN_NULL;
				return NULL;

			default:
			{
				//Log.Error("MarshalStream", "Invalid type tag %d, '%c' in stream.", opcode, *((char*)&opcode));
				Log.Error("MarshalStream", "Invalid type tag %d, raw 0x%X, char '%c' at:0x%X", opcode,opcode,*((char*)&opcode), stream.tell());
				MARSHALSTREAM_RETURN_NULL;
			}
		}
		MARSHALSTREAM_RETURN_NULL;
	}

	Log.Error("MarshalStream", "not enough data in the stream to read a additional character");
	MARSHALSTREAM_RETURN_NULL;
}

PyObject* MarshalStream::ReadBuffer( ReadStream & stream )
{
	uint32 size;
	if (!stream.readSizeEx(size))
		MARSHALSTREAM_RETURN_NULL;

	uint8* buffer;
	if (!stream.readBuffer(&buffer, size))
		MARSHALSTREAM_RETURN_NULL;

	PyString* ret = PyString_FromStringAndSize((char*)buffer, size);
	MARSHALSTREAM_RETURN(ret);
}

/* this still can go wrong... */
PyObject* MarshalStream::ReadClass( ReadStream & stream, BOOL shared )
{
	PyClass * classObj = new PyClass();

	if (shared != FALSE)
	{
		classObj->IncRef();
		mReferencedObjectsMap.StoreReferencedObject(classObj);
	}

	if (classObj == NULL)
		MARSHALSTREAM_RETURN_NULL;

	PyString * className = (PyString *)unmarshal(stream);
	PyTuple * bases = (PyTuple*)unmarshal(stream);

	if(!classObj->setname(className))
	{
		ASCENT_HARDWARE_BREAKPOINT;
		className->DecRef();
		classObj->DecRef();
		bases->DecRef();
		MARSHALSTREAM_RETURN_NULL;
	}

	if(!classObj->setbases(bases))
	{
		ASCENT_HARDWARE_BREAKPOINT;
		className->DecRef();
		classObj->DecRef();
		bases->DecRef();
		MARSHALSTREAM_RETURN_NULL;
	}

	MARSHALSTREAM_RETURN(classObj);
}

PyObject* MarshalStream::ReadCallUpdateObject( ReadStream & stream, BOOL shared )
{
	PyClass * classObj = new PyClass();

	if (classObj == NULL)
		MARSHALSTREAM_RETURN_NULL;

	if (shared != FALSE)
	{
		classObj->IncRef();
		mReferencedObjectsMap.StoreReferencedObject(classObj);
	}

	PyTuple * bases = (PyTuple *)unmarshal(stream);
	if (bases == NULL)
		MARSHALSTREAM_RETURN_NULL;

	PyString * debugName = new PyString("[DEBUG] class update");
	classObj->setname(debugName);

	classObj->setbases(bases);
	ReadNewObjList(stream, *classObj);
	ReadNewObjDict(stream, *classObj);

	MARSHALSTREAM_RETURN(classObj);
}

PyObject* MarshalStream::ReadCallObjectNew( ReadStream & stream, BOOL shared )
{
	PyClass * classObj = new PyClass();

	if (shared != FALSE)
	{
		classObj->IncRef();
		mReferencedObjectsMap.StoreReferencedObject(classObj);
	}

	PyTuple * bases = (PyTuple *)unmarshal(stream);

	PyString * debugName = new PyString("[DEBUG] class new");
	classObj->setname(debugName);

	classObj->setbases(bases);

	ReadNewObjList(stream, *classObj);
	ReadNewObjDict(stream, *classObj);

	MARSHALSTREAM_RETURN(classObj);
}

PyObject* MarshalStream::ReadPackedRow( ReadStream & stream )
{
	PyPackedRow * packedRow = NULL;
	PyClass* obj1 = (PyClass*)unmarshal(stream);
	if (obj1 == NULL)
		MARSHALSTREAM_RETURN_NULL;

	packedRow = new PyPackedRow();
	packedRow->init((PyObject*)obj1);

	assert(obj1->gettype() == PyTypeClass);
		
	size_t size;
	if (!stream.readSizeEx(size))
	{
		obj1->DecRef();
		packedRow->DecRef();
		MARSHALSTREAM_RETURN_NULL;
	}

	uint8* data;
	if (!stream.readBuffer(&data, size))
	{
		obj1->DecRef();
		packedRow->DecRef();
		MARSHALSTREAM_RETURN_NULL;
	}

	size_t outsize = 0;
	uint8 *outbuff = NULL;

	size_t virtualFieldCount = 0;
	size_t fieldCount = 0;
	PyObject* rawPayload = NULL;

	if (size > 0)
	{
		outsize = (size * 8) + 0x100;
		size_t bufferSize = outsize;

		outbuff = (uint8*)ASCENT_MALLOC(outsize);
		
		assert(outbuff);

		if (!RleModule::decode(data, size, outbuff, &outsize))
		{
			SafeFree(outbuff);

			obj1->DecRef();
			Log.Error("MarshalStream", "error happened in the 'Rle' decoder");
			MARSHALSTREAM_RETURN_NULL;
		}

		packedRow->mRawFieldData = outbuff;
		packedRow->mRawFieldDataLen = outsize;

		/*size_t guessedSize = DBRowModule::GetRawFieldSizeFromHeader(((PyTuple*)obj1->getbases())->GetItem(1));

		if (outsize != guessedSize)
		{
			Dump(stdout, ((PyTuple*)obj1->getbases())->GetItem(1), 0);

			HexAsciiModule::print_hexview(stdout, (char*)outbuff, outsize);
			HexAsciiModule::print_hexview(stdout, (char*)data, size);

			RleModule::zeropackbuff testBuff;
			RleModule::UnpackZeroCompressed(data, size, testBuff);
			HexAsciiModule::print_hexview(stdout, (char*)&testBuff[0], testBuff.size());

			int henk = 3;

			//ASCENT_HARDWARE_BREAKPOINT;
		}*/

		memset(&outbuff[outsize+1],0,100);
		// seems to be a bit wierd at first but it seems to be correct, what should be checked is how this results in correct stuff.
		outsize = (size * 8) + 0x100;

		//FILE * dbrowTest = fopen("dbrowtest.txt", "wb");
		//HexAsciiModule::print_hexview(dbrowTest, (char*)outbuff, size);
		//fclose(dbrowTest);
		//HexAsciiModule::print_hexview(stdout, (char*)outbuff, size);
		rawPayload = DBRowModule::parseraw(*this, (PyObject*)obj1, outbuff, outsize, virtualFieldCount, fieldCount);
		//SafeFree(outbuff);

		//SafeFree(outbuff);
	}
	
	packedRow->setheader(obj1);
	packedRow->setRawPayLoad(rawPayload);

	/* evil bit we force the row tuple to swallow the itr chunks / objects */

	/* strings related to the following code
	 * "extra virtual columns at the end"
	 * "return size of data, and number of extra columns"
	 */
	if (virtualFieldCount > 0)
	{
		size_t intergralOffset = fieldCount - virtualFieldCount;
		for (size_t i = 0; i < virtualFieldCount; i++)
		{
			PyObject* henk = unmarshal(stream);
			packedRow->addleaf(henk);
		}
	}
		
	MARSHALSTREAM_RETURN(packedRow);
}

PyObject* MarshalStream::ReadSubStream( ReadStream & stream )
{
	uint32 size = 0;
	if (!stream.readSizeEx(size))
		MARSHALSTREAM_RETURN_NULL;

	PySubStream* object = new PySubStream();
	uint8* data;
	if (!stream.readBuffer(&data, size))
	{
		object->DecRef();
		MARSHALSTREAM_RETURN_NULL;
	}

	if(!object->set(data, size))
	{
		object->DecRef();
		MARSHALSTREAM_RETURN_NULL;
	}

	MARSHALSTREAM_RETURN(object);
}

PyObject* MarshalStream::ReadVarInteger( ReadStream & stream, BOOL shared )
{
	uint32 len = 0;
	if(!stream.readSizeEx(len))
		MARSHALSTREAM_RETURN_NULL;
	
	/* we don't have big int implemented so crash if it goes crazy */
	ASCENT_ASSERT(len <= 8);
	
	PyLong* object = NULL;
	if(len == 0)
	{
		object = PyLong_FromLong(len);
	}
	else
	{
		uint8 *buffer = NULL;
		if (!stream.readBuffer(&buffer, len))
			MARSHALSTREAM_RETURN_NULL;

		object = _ByteArray_AsPyLong(buffer, len);
	}
	
	if (shared != FALSE)
	{
		object->IncRef();
		mReferencedObjectsMap.StoreReferencedObject(object);
	}

	MARSHALSTREAM_RETURN(object);
}

PyObject* MarshalStream::ReadClassString( ReadStream & stream, BOOL shared )
{
	PyString* objectName = (PyString*)ReadBuffer(stream);
	objectName->append("[CLASS STRING]", 0, 14);

	/* bad bad bad boy, don't do a "new" here... because the class needed to return already exists */
	PyClass * GlobalClass = new PyClass();
	GlobalClass->setname(objectName);

	if (shared != FALSE)
	{
		GlobalClass->IncRef();
		mReferencedObjectsMap.StoreReferencedObject(GlobalClass);
	}
	
	MARSHALSTREAM_RETURN(GlobalClass);
}

bool MarshalStream::checkAndInflate( ReadStream & stream )
{
	char idChar;
	if (!stream.peek1(idChar))
	{
		Log.Error("MarshalStream", "Unable to peek 1 byte of the stream and check if its compressed");
		return false;
	}

	/* check if the packet its compressed */
	if (idChar != '~' )
	{
		if (idChar != char(0x78))
		{
			Log.Error("MarshalStream", "unknown idChar something fishy is going on..... panic");
			return false;
		}
		else
		{
			u_long  sourcelen = (u_long)stream.size();
			Bytef * source = stream.content();

			/* One of the key things of the ZLIB stuff is that we 'sometimes' don't know the size of the uncompressed data.
			 * My idea is to fix this regarding the first phase of the parsing of the data (the parser) is to go trough a
			 * couple of output buffer size. First buffer size would be 4x the initial buffer size, implying that the
			 * compression ratio is about 75%. The second buffer size would be 8x the initial buffer size, implying that the
			 * compression ratio is about 87.5%. The third and last buffer size is 16x the initial buffer size implying that
			 * the compression ratio is about 93.75%. This theory is really stupid because there is no way to actually know.
			 */

			uint32 bufferMultiplier = 4;
			u_long outBufferLen = sourcelen * bufferMultiplier;
			u_long allocatedBufferLen = outBufferLen;
#ifndef PACKET_PARSER_MEMORY_ON_DRUGS
			Bytef * outBuffer = (Bytef *)ASCENT_MALLOC(outBufferLen);
#else
			Bytef * outBuffer = (Bytef *)sBufferPool.GetBuffer(outBufferLen);
#endif
			
			int zlibUncompressResult = uncompress(outBuffer, &outBufferLen, source, sourcelen);
			
			if (zlibUncompressResult == Z_BUF_ERROR)
			{
				int loop_limiter = 0;
				while(zlibUncompressResult == Z_BUF_ERROR)
				{
					/* because this code is a possible fuck up, we add a check */
					if (loop_limiter++ > 100)
					{
						zlibUncompressResult = Z_MEM_ERROR;
						Log.Error("MarshalStream", "uncompress increase buffer overflow safe mechanism");
						break;
					}

#ifdef PACKET_PARSER_MEMORY_ON_DRUGS
					sBufferPool.DumpBuffer(outBuffer, allocatedBufferLen);
#endif

					bufferMultiplier*=2;
					outBufferLen = sourcelen * bufferMultiplier;

					Log.Warning("MarshalStream","uncompress failed and we now do the increase buffer size trick: %d times. Resizing from: %u to %u", loop_limiter, outBufferLen / bufferMultiplier,outBufferLen);
					
					allocatedBufferLen = outBufferLen;
#ifndef PACKET_PARSER_MEMORY_ON_DRUGS
					outBuffer = (Bytef*)ASCENT_REALLOC(outBuffer, outBufferLen); // resize the output buffer
#else
					outBuffer = (Bytef *)sBufferPool.GetBuffer(outBufferLen);
#endif
					zlibUncompressResult = uncompress(outBuffer, &outBufferLen, source, sourcelen); // and try it again
				}

				if (zlibUncompressResult != Z_OK)
				{
					Log.Error("MarshalStream", "uncompress went wrong ***PANIC***");
#ifndef PACKET_PARSER_MEMORY_ON_DRUGS
					free(outBuffer);
#else
					sBufferPool.DumpBuffer(outBuffer, allocatedBufferLen);
#endif
					return false;
				}
			}
			else if (zlibUncompressResult != Z_OK)
			{
				Log.Error("MarshalStream", "uncompress went wrong ***PANIC***");
#ifndef PACKET_PARSER_MEMORY_ON_DRUGS
				free(outBuffer);
#else
				sBufferPool.DumpBuffer(outBuffer, allocatedBufferLen);
#endif
				return false;
			}
			
			/* if we get here it usually means uncompress was successful. */
			stream.set(outBuffer, outBufferLen);

#ifndef PACKET_PARSER_MEMORY_ON_DRUGS
			free(outBuffer);
#else
			sBufferPool.DumpBuffer(outBuffer, allocatedBufferLen);
#endif
		}
	}
	return true;
}

bool MarshalStream::ReadNewObjList( ReadStream & stream, PyClass & obj )
{
	PyList * list = PyList_New(0);
	PyObject * listItr = NULL;
	bool healthyExit = false;
	while (true)
	{
		char thingy;
		if (stream.peek1(thingy))
		{
			if (thingy != Op_PyStopIter)
			{
				listItr = unmarshal(stream);
			}
			else
			{
				listItr = NULL;

				if(!stream.seek(1, SEEK_CUR))
				{
					Log.Error("MarshalStream", "unable to skip '-' in ReadNewObjList");
					assert(false);
				}
				else
				{
					healthyExit = true;
				}
			}
		}
		else
		{
			listItr = NULL;
			Log.Error("MarshalStream", "unable to peek for itr stop char in ReadNewObjList");
			assert(false);
		}

		if (healthyExit == true)
			break;

		if (listItr == NULL)
			return false;

		list->add(listItr);
	}

	obj.setDirList(list);

	return true;
}

bool MarshalStream::ReadNewObjDict( ReadStream & stream, PyClass & obj )
{
	PyDict * dict = PyDict_New();
	PyObject * dictItr = NULL;
	bool healthyExit = false;
	while (true)
	{
		char thingy;
		if (stream.peek1(thingy))
		{
			if (thingy != Op_PyStopIter)
			{
				dictItr = unmarshal(stream);
			}
			else
			{
				dictItr = NULL;

				if(!stream.seek(1, SEEK_CUR))
				{
					Log.Error("MarshalStream", "unable to skip '-'");
					assert(false);
				}
				else
				{
					healthyExit = true;
				}
			}
		}
		else
		{
			dictItr = NULL;
			Log.Error("MarshalStream", "unable to peek for itr stop char");
			assert(false);
		}

		if (healthyExit == true)
			break;

		if (dictItr == NULL)
			return false;

		PyObject * keyName = unmarshal(stream);

		dict->set_item(dictItr, keyName);
		keyName->DecRef();
	}

	obj.setDirDict(dict);

	return true;
}

bool MarshalStream::save( PyObject * object, WriteStream & stream )
{
	if (object == NULL)
		return false;

	stream.write1('~'); // first token
	uint32 refCountPlaceholder = 0;
	stream.write4(refCountPlaceholder);

	marshal(object, stream);

	return true; // crap for now
}

/* helper function for me fucking up */
bool marshalString(const char* str, WriteStream & stream)
{
	size_t str_len = strlen(str);

	if (str_len == 0)
	{
		return stream.writeOpcode(Op_PyEmptyString);
	}
	else if(str_len == 1)
	{
		if (!stream.writeOpcode(Op_PyCharString))
			return false;
		return stream.write1(str[0]);
	}
	else if (str_len < 0xFF)
	{
		if (!stream.writeOpcode(Op_PyShortString))
			return false;

		if (!stream.write1(str_len))
			return false;
		return stream.write(str, str_len);
	}
	else
	{
		/* I can't remember why and how.... */
		ASCENT_HARDWARE_BREAKPOINT;
	}
}

//#  define unmarshalState(x, y) {sLog.String("State:"#x"\toffset:0x%X", y.tell());}

bool MarshalStream::marshal( PyObject * object, WriteStream & stream )
{
	uint8 object_type = ((PyInt*)object)->gettype();
	
	switch (object_type)
	{
	case PyTypeNone:
		{
			return stream.writeOpcode(Op_PyNone);
		} break;

	case PyTypeBool:
		{
			PyBool & b = *(PyBool *)object;
			if (b == true)
				return stream.writeOpcode(Op_PyTrue);
			else
				return stream.writeOpcode(Op_PyFalse);
		} break;

	case PyTypeLong:
		{
			int64 val = ((PyLong *)object)->GetValue();

			if (val < 0)
			{
				/* if our value is smaller than 0 meaning its a negative value we can't send it trough the optimized system so send it as long long */
				if(!stream.writeOpcode(Op_PyLongLong))
					return false;
				return stream.write8(val);
			}
			else
			{
				return WriteVarInteger(stream, object);
			}
			break;
		}
	case PyTypeInt:
		{
			int32 val = ((PyInt *)object)->GetValue();;

			if (val == -1)
			{
				return stream.writeOpcode(Op_PyMinusOne);
			}
			else if (val == 0)
			{
				return stream.writeOpcode(Op_PyZeroInteger);
			}
			else if (val == 1)
			{
				return stream.writeOpcode(Op_PyOneInteger);
			}
			else if ( val + 0x80u > 0xFF )
			{
				if ( val + 0x8000u > 0xFFFF )
				{
					if (!stream.writeOpcode(Op_PyLong))
						return false;
					return stream.write4(val);
				}
				else
				{
					if (!stream.writeOpcode(Op_PySignedShort))
						return false;
					return stream.write2(val);
				}
			}
			else
			{
				if (!stream.writeOpcode(Op_PyByte))
					return false;
				return stream.write1(val);
			}
		} break;

	case PyTypeReal:
		{
			PyFloat & number = *(PyFloat*)object;
			double val = number.GetValue();
			if (val == 0.0)
				return stream.writeOpcode(Op_PyZeroFloat);

			if (!stream.writeOpcode(Op_PyFloat))
				return false;

			return stream.write8(val);
		} break;

	case PyTypeString:
		{
			PyString & str = *(PyString*)object;
			size_t str_len = str.length();
			if (str_len == 0)
			{
				return stream.writeOpcode(Op_PyEmptyString);
			}
			else if(str_len == 1)
			{
				if (!stream.writeOpcode(Op_PyCharString))
					return false;
				return stream.write1(str[0]);
			}

			size_t str_index = sPyStringTable.LookupIndex(str.content());
			if (str_index != -1)
			{
				if (!stream.writeOpcode(Op_PyStringTableItem))
					return false;
				return stream.write1(str_index);
			}
			else if (str_len < 0xFF)
			{
				if (!stream.writeOpcode(Op_PyShortString))
					return false;

				if (!stream.write1(str_len))
					return false;
				return stream.write(str.content(), str_len);
			}
			else
			{
				/* I can't remember why and how.... */
				ASCENT_HARDWARE_BREAKPOINT;
			}
		} break;

	case PyTypeUnicode:
		{
			/*
			v32 = PyUnicodeUCS2_AsUTF8String(ArgList);
			*/
			PyUnicodeUCS2 & str = *(PyUnicodeUCS2*)object;
			size_t str_len = str.length();
			if (str_len == 0)
			{
				return stream.writeOpcode(Op_PyUnicodeEmptyString);
			}
			else if(str_len == 1)
			{
				if (!stream.writeOpcode(Op_PyUnicodeCharString))
					return false;
				return stream.write2(str.content()[0]);
			}

			PyString* utf8str = (PyString*)PyUnicode_AsUTF8String(object);
			if (utf8str != NULL)
			{
				if (utf8str->length() < str_len * 2)
				{
					if (!stream.writeOpcode(Op_PyUnicodeUTF8String))
					{
						utf8str->DecRef();
						return false;
					}

					if (!stream.writeSizeEx(utf8str->length()))
					{
						utf8str->DecRef();
						return false;
					}

					if (stream.write(utf8str->content(), utf8str->length()))
					{
						utf8str->DecRef();
						return true;
					}
					else
					{
						utf8str->DecRef();
						return false;
					}
				}
			}

			/* if we got here we can't save bandwidth by sending the unicode string as UTF8 */
			if (!stream.writeOpcode(Op_PyUnicodeString))
				return false;

			if (!stream.writeSizeEx(str.length()))
				return false;

			return stream.write((char*)str.content(), str.length() * 2);
		} break;

	case PyTypeDict:
		{
			PyDict & dict = *(PyDict*)object;			

			if(!stream.writeOpcode(Op_PyDict))
				return false;
			
			if (!stream.writeSizeEx(dict.size()))
				return false;

			PyDict::iterator itr = dict.begin();
			for (; itr != dict.end(); itr++)
			{
				PyDictEntry * entry = itr->second;

				/* marshal the payload */
				if(!marshal(entry->obj, stream))
					return false;

				/* marshal the key */
				if(!marshal(entry->key, stream))
					return false;
			}
			return true;
		} break;

	case PyTypeTuple:
		{
			PyTuple & tuple = *(PyTuple*)object;
			switch(tuple.size())
			{
			case 0:
				return stream.writeOpcode(Op_PyEmptyTuple);
			case 1:
				if(!stream.writeOpcode(Op_PyOneTuple))
					return false;
				break;
			case 2:
				if(!stream.writeOpcode(Op_PyTwoTuple))
					return false;
				break;
			default:
				if(!stream.writeOpcode(Op_PyTuple))
					return false;

				if(!stream.writeSizeEx(tuple.size()))
					return false;

				break;
			}

			PyTuple::iterator itr = tuple.begin();
			for (; itr != tuple.end(); itr++)
			{
				PyChameleon* leaf = (*itr);
				if (leaf == NULL)
					return false;
				if (leaf->getPyObject() == NULL)
					return false;
				if(!marshal(leaf->getPyObject(), stream))
					return false;
			}
			return true;
		} break;

	case PyTypeList:
		{
			PyList & list = *(PyList*)object;
			switch(list.size())
			{
			case 0:
				return stream.writeOpcode(Op_PyEmptyList);
			case 1:
				if(!stream.writeOpcode(Op_PyOneList))
					return false;
				break;
			default:
				if(!stream.writeOpcode(Op_PyList))
					return false;

				if(!stream.writeSizeEx(list.size()))
					return false;
				break;
			}

			for (size_t i = 0; i < list.size(); i++)
			{
				PyChameleon& leaf = list[int(i)];
				if (&leaf == NULL)
					return false;
				if (leaf.getPyObject() == NULL)
					return false;
				if(!marshal(leaf.getPyObject(), stream))
					return false;
			}
			return true;
		} break;

	case PyTypeSubStream: 
		{
			PySubStream & substream = *(PySubStream*)object;

			if(!stream.writeOpcode(Op_PySubStream))
				return false;

			if (!stream.writeSizeEx(substream.size()))
				return false;

			return stream.write((char*)substream.content(), substream.size());
		} break;

	case PyTypeClass:
		{
			if (!stream.writeOpcode(Op_PyModuleInstance))
				return false;

			PyClass * classObject = (PyClass*)object;
			if(!marshal((PyObject*)classObject->getname(), stream))
				return false;
			if(!marshal((PyObject*)classObject->getbases(), stream))
				return false;
			return true;
		} break;

	case PyTypePackedRow: 
		{
			ASCENT_HARDWARE_BREAKPOINT;
		} break;

	case PyTypeSubStruct: 
		{
			ASCENT_HARDWARE_BREAKPOINT;
		} break;

	case PyTypeDeleted: {assert(false);} break;

	default:
		uint8 sjaak = object->gettype();
		Log.Error("MarshalStream","marshalling unhandled tag[0x%X].... sometying borked..", sjaak);
		
		ASCENT_HARDWARE_BREAKPOINT;
		break;
	}

	//every case should return its own stuff regarding success or not
	ASCENT_HARDWARE_BREAKPOINT;
	return true;
}

bool MarshalStream::checkForBytecode( ReadStream & stream )
{
	uint8 byteCodeTag;
	if (!stream.peek1(byteCodeTag))
	{
		Log.Error("MarshalStream","unable to check marshal stream for byte code");
		return false;
	}

	if (byteCodeTag == 0x63 || byteCodeTag == 0xB3)
	{
		printf("dumpting python bytecode to file\n");
		char byteCodePacketName[MAX_PATH];

		/* hash the buffer so we can dump them easily */
		uint32 bytecodeHash = Utils::Hash::djb2_hash((char*)stream.content(), (int)stream.buffersize());
		snprintf(byteCodePacketName, MAX_PATH, "bytecodeStream_%u.pyc", bytecodeHash);
		FILE * fp = fopen(byteCodePacketName, "wb");
		fwrite((char*)stream.content(), stream.buffersize(), 1, fp);
		fclose(fp);
		stream.seek(0, SEEK_END); // we have read everything into the bytecode buffer.... so we now act like we have parsed the entire file
		return true;
	}
	return false;
}

ASCENT_INLINE bool MarshalStream::WriteVarInteger( WriteStream& stream, PyObject* number )
{
	/* check if something went wrong */
	if (number == NULL)
		return false;

	if (number->gettype() != PyTypeLong)
		return false;

	int64 num = ((PyLong*)number)->GetValue();
	int64 tempNum = num;
	bool negative = false;
	if (num < 0)
	{
		negative = true;
		num = -num;
	}

	/* this part checks how many bytes the number is using */
	uint8 integerSize = 0;
	uint8* num_buff = (uint8*)&num;
	for (int i = 8; i >= 1; i--)
	{
		if (num_buff[i-1] != 0)
		{
			integerSize = i;
			break;
		}
	}

	uint8* pTempNum = (uint8*)&tempNum;

	/** the theory about this piece of code: the python var integer system is fucked up...
	 * @todo Captnoord: complete this comment and improve the code.
	 */
	if (pTempNum[integerSize-1] & 0x80 && negative == false)
	{
		integerSize++;
	}
	else if (negative == true)
	{
		num_buff[integerSize-1] |= 0x80; /* @todo implement this in the read function.... argsh(*&^*&^%$ */
	}

	if(!stream.reserve(integerSize + 2)) /* reserve 2 extra because we know we have 2 additional bytes */
		return false;

	if (!stream.writeOpcode(Op_PyVarInteger))
		return false;

	if (!stream.write1(integerSize))
		return false;

	return stream.write((char*)num_buff, integerSize);
}

// undef our return null macro.
#undef MARSHALSTREAM_RETURN_NULL