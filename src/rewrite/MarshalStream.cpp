#include "Common.h"
#include "ascent.h"
#include "PyObjects.h"
#include "MarshalStream.h"
#include "EveMarshalOpcodes.h"
#include "PyStream.h"
#include "PyStringTable.h"
#include "MarshalReferenceMap.h"
#include <zlib.h>

// small macro's to catch those NULL pointers... they just shouldn't happen
#ifdef _DEBUG
#  define MARSHALSTREAM_RETURN_NULL {ASCENT_HARDWARE_BREAKPOINT; return NULL;}
#  define MARSHALSTREAM_RETURN(p) {PyObject * x = ((PyObject*)p); assert(x != NULL); return x;}
#else
#  define MARSHALSTREAM_RETURN_NULL {return NULL;}
#  define MARSHALSTREAM_RETURN(p) return (PyObject*)p;
#endif//_DEBUG

#ifdef _DEBUG
#define unmarshalState(x, y) {sLog.String("State:"#x"\toffset:0x%X", y.tell());}
#else
#define unmarshalState(x, y) //x, y
#endif


MarshalStream::MarshalStream() : _deleted(false), PyIntZero(0), PyIntOne(1), PyIntMinOne(-1), PyFloatZero(0.0),PyIntInfinite(-1, true),
	Py_TrueStruct(true), Py_ZeroStruct(false), sharedObjectsMapInternal(NULL) {}

MarshalStream::~MarshalStream()
{
	_deleted = true;
}

PyObject* MarshalStream::load(PyReadStream & stream)
{
	if (!ReadMarshalHeader(stream))
	{
		Log.Error("MarshalStream", "[load] Unable to read and initialize the packet header");
		MARSHALSTREAM_RETURN_NULL;
	}

	PyObject* unmarshaledData = unmarshal(stream);

	MARSHALSTREAM_RETURN(unmarshaledData);
}

bool MarshalStream::ReadMarshalHeader(PyReadStream & stream)
{
	char marshalTilde;
	if (!stream.readChar(marshalTilde))
	{
		Log.Error("MarshalStream", "[ReadMarshalHeader]Can't read %d elements of %d bytes, only have %d bytes left", 1, 1, stream.size() - stream.tell());
		return false;
	}

	if (marshalTilde != '~')
	{
		Log.Error("MarshalStream", "[ReadMarshalHeader]invalid marshal header, missing tilde");
		return false;
	}

	int32 sharedObjectCount;
	if (!stream.readInt4(sharedObjectCount))
	{
		Log.Error("MarshalStream", "[ReadMarshalHeader]Can't read %d elements of %d bytes, only have %d bytes left", 1, 4, stream.size() - stream.tell());
		return false;
	}

	// this packet doesn't have shared objects :D
	if (sharedObjectCount <= 0)
	{
		return true;
	}

	sharedObjectsMapInternal.SetSharedObjectCount(sharedObjectCount);
	
	size_t tReadIndex2 = stream.tell();
	if ( (signed int)(stream.size() - tReadIndex2) / 4 < sharedObjectCount )
	{
		Log.Error("MarshalStream", "[ReadMarshalHeader]Too little data in marshal stream, %u bytes. I really wanted at least %u bytes total, mapcount in header is %d", stream.size(), tReadIndex2 + 4 * sharedObjectCount, sharedObjectCount);
		return false;
	}

	/* dirty stream injection check :D:D:D whooo */
	int32* sharedObjectBegin = (int32*)&stream.content()[stream.size() - 4 * sharedObjectCount];
	int32 loopcounter = 0;
	while ( *sharedObjectBegin >= 1 && *sharedObjectBegin <= sharedObjectCount )
	{
		++loopcounter;
		++sharedObjectBegin;
		if ( loopcounter >= sharedObjectCount )
			return true;
	}

	Log.Error("MarshalStream", "Bogus map data in marshal stream.");

	return false;
}

PyObject* MarshalStream::unmarshal(PyReadStream & stream)
{
	if (stream.size() - stream.tell() >= 1)
	{
		uint8 opcode;
		stream.readInt1(&opcode);
		switch ( opcode & 0x3F )
		{
			case Op_PyNone:
			{
				unmarshalState(Op_PyNone, stream);
				MARSHALSTREAM_RETURN(&PyNone);
			}

			case Op_PyClassString:
			{
				unmarshalState(Op_PyClassString, stream);
				MARSHALSTREAM_RETURN(ReadClassString(stream, (opcode >> 6) & 0xFFFFFF01));
			}

			case Op_PyLongLong:
			{
				unmarshalState(Op_PyLongLong, stream);
				int64 number;
				if (!stream.readInt8(&number))
					MARSHALSTREAM_RETURN_NULL;
				MARSHALSTREAM_RETURN(PyLong_FromLongLong(number));
			}

			case Op_PyLong:
			{
				unmarshalState(Op_PyLong, stream);
				int32 number;
				if (!stream.readInt4(&number))
					MARSHALSTREAM_RETURN_NULL;
				MARSHALSTREAM_RETURN(PyInt_FromLong(number));
			}

			case Op_PySignedShort:
			{
				unmarshalState(Op_PySignedShort, stream);
				int16 number;
				if (!stream.readInt2(&number))
					MARSHALSTREAM_RETURN_NULL;
				MARSHALSTREAM_RETURN(PyInt_FromLong(number));
			}

			case Op_PyByte:
			{
				unmarshalState(Op_PyByte, stream);
				int8 number;
				if (!stream.readInt1(&number))
					MARSHALSTREAM_RETURN_NULL;
				MARSHALSTREAM_RETURN(PyInt_FromLong(number));
			}

			// original code also has a reference counter of this object in the following cases.
			case Op_PyInfinite:
			{
				unmarshalState(Op_PyInfinite, stream);
				MARSHALSTREAM_RETURN(&PyIntInfinite);
			}
			
			case Op_PyZeroInteger:
			{
				unmarshalState(Op_PyZeroInteger, stream);
				MARSHALSTREAM_RETURN(&PyIntZero);
			}
			
			case Op_PyOneInteger:
			{
				unmarshalState(Op_PyOneInteger, stream);
				MARSHALSTREAM_RETURN(&PyIntOne);
			}
			
			case Op_PyFloat:
			{
				unmarshalState(Op_PyFloat, stream);
				double number;
				if (!stream.readDouble8(&number))
					MARSHALSTREAM_RETURN_NULL;
				MARSHALSTREAM_RETURN(PyFloat_FromDouble(number));
			}

			case Op_PyZeroFloat:
			{
				unmarshalState(Op_PyZeroFloat, stream);
				MARSHALSTREAM_RETURN(&PyFloatZero);
			}
			
			case Op_PyTrue:
			{
				unmarshalState(Op_PyTrue, stream);
				MARSHALSTREAM_RETURN(&Py_TrueStruct);
			}
			
			case Op_PyFalse:
			{
				unmarshalState(Op_PyFalse, stream);
				MARSHALSTREAM_RETURN(&Py_ZeroStruct);
			}
			
			case Op_PyBuffer:
			{
				unmarshalState(Op_PyBuffer, stream);
				MARSHALSTREAM_RETURN(ReadBuffer(stream));
			}
			
			case Op_PyEmptyString:
			{
				unmarshalState(Op_PyEmptyString, stream);
				MARSHALSTREAM_RETURN(&PyStringEmpty);
			}
			
			case Op_PyCharString:
			{
				unmarshalState(Op_PyCharString, stream);
				char CharString;
				if (!stream.readChar(&CharString))
					MARSHALSTREAM_RETURN_NULL;
				MARSHALSTREAM_RETURN(PyString_FromStringAndSize(&CharString, 1));
			}

			case Op_PyShortString:
			{
				unmarshalState(Op_PyShortString, stream);
				uint8 strlen;
				char* strptr;
				if (!stream.readInt1(&strlen))
					MARSHALSTREAM_RETURN_NULL;
				if (!stream.readString(&strptr, strlen))
					MARSHALSTREAM_RETURN_NULL;
				MARSHALSTREAM_RETURN(PyString_FromStringAndSize(strptr, strlen));
			}

			case Op_PyStringTableItem:
			{
				unmarshalState(Op_PyStringTableItem, stream);
				uint8 index;
				if (!stream.readInt1(&index))
					MARSHALSTREAM_RETURN_NULL;
	
				PyString * ret = NULL;
				if (!sPyStringTable.LookupPyString(index, ret) && ret == NULL)
				{
					Log.Error("MarshalStream", "Invalid string table index %d", index);
					MARSHALSTREAM_RETURN_NULL;
				}
				MARSHALSTREAM_RETURN(ret);
			}

			/* I'm very unsure about this one...!!!!!!!!!!! */
			case Op_PyUnicodeByteString:
			{
				unmarshalState(Op_PyUnicodeByteString, stream);
				uint8 strlen;
				wchar_t* strptr;
				if (!stream.readInt1(&strlen))
					MARSHALSTREAM_RETURN_NULL;
	
				if (!stream.readWstring(&strptr, strlen))
					MARSHALSTREAM_RETURN_NULL;
	
				MARSHALSTREAM_RETURN(PyUnicodeUCS2_FromWideChar(strptr, strlen));
			}

			case Op_PyEmptyUnicodeString:
			{
				unmarshalState(Op_PyEmptyUnicodeString, stream);
				MARSHALSTREAM_RETURN(PyUnicodeUCS2_FromWideChar(0, 0));
			}

			case Op_PyUnicodeCharString:
			{
				unmarshalState(Op_PyUnicodeCharString, stream);
				wchar_t singleWChar;
				if (!stream.readWchar(&singleWChar))
					MARSHALSTREAM_RETURN_NULL;
				MARSHALSTREAM_RETURN(PyUnicodeUCS2_FromWideChar(&singleWChar, 1));
			}

			case Op_PyUnicodeString:
			{
				unmarshalState(Op_PyUnicodeString, stream);
				uint32 strlen;
				char* strptr;
				if (!stream.readExSize(&strlen))
					MARSHALSTREAM_RETURN_NULL;
	
				if (!stream.readString(&strptr, strlen))
					MARSHALSTREAM_RETURN_NULL;
	
				MARSHALSTREAM_RETURN(PyUnicodeUCS2_DecodeUTF8(strptr, strlen));
			}

			// unknown.. but somehow related to the cPickle implementation
			case Op_Pyunk1:
			{
				unmarshalState(Op_Pyunk1, stream);
				// this function reads a string with extended size from the stream....
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
				ASCENT_HARDWARE_BREAKPOINT;
				PyTuple& tuple = *PyTuple_New(1);
				if ((opcode & 0x40) != 0)
				{
					if(!sharedObjectsMap.StoreReferencedObject(&tuple))
						MARSHALSTREAM_RETURN_NULL;
				}
	
				// warning recursive function...
				tuple[0] = unmarshal(stream);
				MARSHALSTREAM_RETURN(&tuple);
			}

			case Op_PyTwoTuple:
			{
				unmarshalState(Op_PyTwoTuple, stream);
				PyTuple& tuple = *PyTuple_New(2);
				if ((opcode & 0x40) != 0)
				{
					if(!sharedObjectsMap.StoreReferencedObject(&tuple))
						MARSHALSTREAM_RETURN_NULL;
				}
	
				// warning double recursive function...
				for (int i = 0; i < 2; i++)
				{
					tuple[i] = unmarshal(stream);
				}
				MARSHALSTREAM_RETURN(&tuple);
			}
			
			case Op_PyTuple:
			{
				unmarshalState(Op_PyTuple, stream);
				uint32 elementCount;
				if (!stream.readExSize(&elementCount))
					MARSHALSTREAM_RETURN_NULL;
	
				PyTuple& tuple = *PyTuple_New(elementCount);
				if ((opcode & 0x40) != 0)
				{
					if(!sharedObjectsMap.StoreReferencedObject(&tuple))
						MARSHALSTREAM_RETURN_NULL;
				}
	
				// warning double recursive function...
				for (uint32 i = 0; i < elementCount; i++)
				{
					tuple[i] = unmarshal(stream);
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
					if(!sharedObjectsMap.StoreReferencedObject(&list))
						MARSHALSTREAM_RETURN_NULL;
				}
				// recursive function...
				list[0] = unmarshal(stream);
	
				MARSHALSTREAM_RETURN(&list);
			}

			case Op_PyList:
			{
				unmarshalState(Op_PyList, stream);
				uint32 elementCount;
				if (!stream.readExSize(&elementCount))
					MARSHALSTREAM_RETURN_NULL;
	
				PyList &list = *PyList_New(elementCount);
				if (&list == NULL)
					MARSHALSTREAM_RETURN_NULL;
	
				if ((opcode & 0x40) != 0)
				{
					if(!sharedObjectsMap.StoreReferencedObject(&list))
						MARSHALSTREAM_RETURN_NULL;
				}
	
				// n recursive function call
				for (uint32 i = 0; i < elementCount; i++)
				{
					list[i] = unmarshal(stream);
				}
				MARSHALSTREAM_RETURN(&list);
			}
			
			/* this handler is a lot more complicated than I have implemented here... will do for now... */
			case Op_PyDict:
			{
				unmarshalState(Op_PyDict, stream);
				uint32 elementCount;
				if (!stream.readExSize(&elementCount))
					MARSHALSTREAM_RETURN_NULL;
	
				PyDict & dict = *PyDict_New();
				if ((opcode & 0x40) != 0)
				{
					if(!sharedObjectsMap.StoreReferencedObject(&dict))
						MARSHALSTREAM_RETURN_NULL;
				}
	
				// whoo tricky stuff....
				for (uint32 i = 0; i < elementCount; i++)
				{
					PyObject* keyPayload = unmarshal(stream);			// Payload
					PyString* keyName = (PyString*)unmarshal(stream);	// the keyname

					if (keyName->gettype() != PyTypeString)
					{
						Log.Error("marshal::load","bigtime error in the dict unmarshalling we expecting a string..");
						assert(false);
					}
	
					// EVIL HACKY SOLLUTION WARNING!!!!!!!!!!!!!!!!!!!!!!!
					dict[keyName->content()] = keyPayload;
				}
				MARSHALSTREAM_RETURN(&dict);
			}

			/* read a object, it also has a check for reference mapping */
			case Op_PyClassObject:
			{
				unmarshalState(Op_PyClassObject, stream);
				return ReadClass(stream, (opcode >> 6) & 0xFFFFFF01);
			}

			/* unknown isn't handled but what we know of them is that its related to the cPickle system */
			case Op_Pyunk2:
			{
				unmarshalState(Op_Pyunk2, stream);
				MARSHALSTREAM_RETURN_NULL;
			}
			
			/* need to implement custom callbacks and reading stuff... but for the server this doesn't seem to usefull.. */
			case Op_PySubStruct:
			{
				unmarshalState(Op_PySubStruct, stream);
				MARSHALSTREAM_RETURN_NULL;
			}
			
			case Op_PyLoadcPickledObject:
			{
				unmarshalState(Op_PyLoadcPickledObject, stream);
				/* |extended size|stream| */
				//MARSHALSTREAM_RETURN(PyObject_CallMethod(unpickledObject, "load", 0);
				MARSHALSTREAM_RETURN_NULL;
			}
		
			case Op_LoadCachedUnpickledObjectFunction:
			{
				unmarshalState(Op_LoadCachedUnpickledObjectFunction, stream);
				MARSHALSTREAM_RETURN_NULL;
			}

			case Op_PySavedStreamElement:
			{
				unmarshalState(Op_PySavedStreamElement, stream);
				uint32 index;
				if (!stream.readExSize(&index))
					MARSHALSTREAM_RETURN_NULL;
				void* obj;
				if (!sharedObjectsMap.GetStoredObject(index,&obj))
				{
					Log.Error("MarshalStream", "(Op_PySavedStreamElement)there seems to be a Invalid TY_REFECENCE in the stream");
					MARSHALSTREAM_RETURN_NULL;
				}
				MARSHALSTREAM_RETURN(obj);
			}

			/* this is fun as check the clients packet hash.... */
			case Op_PyChecksumedStream:
			{
				unmarshalState(Op_PyChecksumedStream, stream);
				uint32 clientHash;
				if (!stream.readInt4(&clientHash))
					MARSHALSTREAM_RETURN_NULL;

				Bytef* data = &stream.content()[stream.tell()];
				uInt len = (uInt)stream.size() - stream.tell();
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

			case Op_NewObject1:
				unmarshalState(Op_NewObject1, stream);
				MARSHALSTREAM_RETURN(ReadNewObject1(stream, (opcode >> 6) & 1));

			case Op_NewObject2:
				unmarshalState(Op_NewObject2, stream);
				MARSHALSTREAM_RETURN(ReadNewObject2(stream, (opcode >> 6) & 1));

			case Op_PyPackedRow:
				unmarshalState(Op_PyPackedRow, stream);
				MARSHALSTREAM_RETURN(ReadPackedRow(stream));

			case Op_PySubStream:
				unmarshalState(Op_PySubStream, stream);
				MARSHALSTREAM_RETURN(ReadSubStream(stream));
				break;

			case Op_PyVarInteger:
				unmarshalState(Op_PyVarInteger, stream);
				MARSHALSTREAM_RETURN(ReadVarInteger(stream, (opcode >> 6) & 1));

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

PyObject* MarshalStream::ReadBuffer( PyReadStream & stream )
{
	uint32 size;
	if (!stream.readExSize(&size))
		MARSHALSTREAM_RETURN_NULL;

	// doing the creating of the object in 1 evil try
	PyString* ret = PyString_FromStringAndSize((const char*)stream.content()[stream.tell()], size);
	stream.seek(size, STREAM_SEEK_CUR);
	MARSHALSTREAM_RETURN(ret);
}

PyObject* MarshalStream::ReadClass( PyReadStream & stream, BOOL shared )
{
	/* bla bla bla do shared object stuff here..... */

	PyClassObject * classObj = new PyClassObject();

	PyString * command = (PyString *)unmarshal(stream);
	ASCENT_ASSERT(command->type() == PyTypeString);

	PyDict * argument = (PyDict*)unmarshal(stream);
	ASCENT_ASSERT(command->type() == PyTypeDict);

	classObj->setname(command);
	classObj->setdict(argument);

	MARSHALSTREAM_RETURN(classObj);
}

PyObject* MarshalStream::ReadNewObject1( PyReadStream & stream, BOOL shared )
{
	return NULL;
}

PyObject* MarshalStream::ReadNewObject2( PyReadStream & stream, BOOL shared )
{
	return NULL;
}

PyObject* MarshalStream::ReadPackedRow( PyReadStream & stream )
{
	return NULL;
}

PyObject* MarshalStream::ReadSubStream( PyReadStream & stream )
{
	uint32 size = 0;
	if (!stream.readExSize(&size))
		MARSHALSTREAM_RETURN_NULL;

	PySubStream* object = new PySubStream();
	uint8* data;
	if (!stream.readBuffer(&data, size))
	{
		SafeDelete(object);
		MARSHALSTREAM_RETURN_NULL;
	}

	if(!object->set(data, size))
	{
		SafeDelete(object);
		MARSHALSTREAM_RETURN_NULL;
	}

	MARSHALSTREAM_RETURN(object);
}

PyObject* MarshalStream::ReadVarInteger( PyReadStream & stream, BOOL shared )
{
	uint8 len;
	if(!stream.readInt1(&len))
		MARSHALSTREAM_RETURN_NULL;

	uint8 *buffer;
	if (!stream.readBuffer(&buffer, len))
		MARSHALSTREAM_RETURN_NULL;

	MARSHALSTREAM_RETURN(_ByteArray_AsPyLong(buffer, len));
}

PyObject* MarshalStream::ReadClassString( PyReadStream & stream, BOOL shared )
{
	PyObject* objectName = _ReadPyStringFromStringAndSize(stream);
	//PyString_FromStringAndSize
	//MARSHALSTREAM_RETURN_NULL;
	MARSHALSTREAM_RETURN(objectName);
}

PyObject* MarshalStream::_ReadPyStringFromStringAndSize( PyReadStream & stream )
{
	uint32 size;
	if (!stream.readExSize(&size))
	{
		MARSHALSTREAM_RETURN_NULL;
	}

	char* buffer;
	if (!stream.readString(&buffer, size))
	{
		MARSHALSTREAM_RETURN_NULL;
	}

	MARSHALSTREAM_RETURN(PyString_FromStringAndSize(buffer, size));
}

// undef our return null macro.
#undef MARSHALSTREAM_RETURN_NULL