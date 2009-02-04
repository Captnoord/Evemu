#include "Common.h"
#include "ascent.h"
#include "PyObjects.h"
#include "MarshalStream.h"
#include "EveMarshalOpcodes.h"
#include "PyStream.h"
#include "PyStringTable.h"
#include "MarshalReferenceMap.h"
#include <zlib/zlib.h>

MarshalStream::MarshalStream() : _deleted(false), PyIntZero(0), PyIntOne(1), PyIntMinOne(-1), PyFloatZero(0.0),PyIntInfinite(-1, true),
	Py_TrueStruct(true), Py_ZeroStruct(false), sharedObjectsMapInternal(NULL) {}

MarshalStream::~MarshalStream()
{
	_deleted = true;
}

void MarshalStream::load(PyReadStream & stream)
{
	if (!ReadMarshalHeader(stream))
	{
		Log.Error("MarshalStream", "[load] Unable to read and initialize the packet header");
		return;
	}

	PyObject* unmarshaledData = unmarshal(stream);
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
		return false;
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
		stream >> opcode;
		switch ( opcode & 0x3F )
		{
			
			case Op_PyNone:
			{
				return return_fixer(&PyNone);
			}

			case Op_PyByteString:
			{
				//return return_fixer(sub_100976B0(a2, (opcode >> 6) & 1);
				return NULL;
			}

			case Op_PyLongLong:
			{
				int64 number;
				if (!stream.readInt8(&number))
					return NULL;
				return return_fixer(PyLong_FromLongLong(number));
			}

			case Op_PyLong:
			{
				int32 number;
				if (!stream.readInt4(&number))
					return NULL;
				return return_fixer(PyInt_FromLong(number));
			}

			case Op_PySignedShort:
			{
				int16 number;
				if (!stream.readInt2(&number))
					return NULL;
				return return_fixer(PyInt_FromLong(number));
			}

			case Op_PyByte:
			{
				int8 number;
				if (!stream.readInt1(&number))
					return NULL;
				return return_fixer(PyInt_FromLong(number));
			}

			// original code also has a reference counter of this object in the following cases.
			case Op_PyInfinite:
			{
				return return_fixer(&PyIntInfinite);
			}
			
			case Op_PyZeroInteger:
			{
				return return_fixer(&PyIntZero);
			}
			
			case Op_PyOneInteger:
			{
				return return_fixer(&PyIntOne);
			}
			
			case Op_PyFloat:
			{
				double number;
				if (!stream.readDouble8(&number))
					return NULL;
				return return_fixer(PyFloat_FromDouble(number));
			}

			case Op_PyZeroFloat:
			{
				return return_fixer(&PyFloatZero);
			}
			
			case Op_PyTrue:
			{
				return return_fixer(&Py_TrueStruct);
			}
			
			case Op_PyFalse:
			{
				return return_fixer(&Py_ZeroStruct);
			}
			
			case Op_PyBuffer:
			{
				return return_fixer(ReadBuffer(stream));
			}
			
			case Op_PyEmptyString:
			{
				return return_fixer(&PyStringEmpty);
			}
			
			case Op_PyCharString:
			{
				char CharString;
				if (!stream.readChar(&CharString))
					return NULL;
				return return_fixer(PyString_FromStringAndSize(&CharString, 1));
			}

			case Op_PyShortString:
			{
				uint8 strlen;
				char* strptr;
				if (!stream.readInt1(&strlen))
					return NULL;
				if (!stream.readString(&strptr, strlen))
					return NULL;
				return return_fixer(PyString_FromStringAndSize(strptr, strlen));
			}

			case Op_PyStringTableItem:
			{
				uint8 index;
				if (!stream.readInt1(&index))
					return NULL;
	
				PyString * ret = NULL;
				if (!sPyStringTable.LookupPyString(index, ret) && ret == NULL)
				{
					Log.Error("MarshalStream", "Invalid string table index %d", index);
					return NULL;
				}
				return return_fixer(ret);
			}

			/* I'm very unsure about this one...!!!!!!!!!!! */
			case Op_PyUnicodeByteString:
			{
				uint8 strlen;
				wchar_t* strptr;
				if (!stream.readInt1(&strlen))
					return NULL;
	
				if (!stream.readWstring(&strptr, strlen))
					return NULL;
	
				return return_fixer(PyUnicodeUCS2_FromWideChar(strptr, strlen));
			}

			case Op_PyEmptyUnicodeString:
			{
				return return_fixer(PyUnicodeUCS2_FromWideChar(0, 0));
			}

			case Op_PyUnicodeCharString:
			{
				wchar_t singleWChar;
				if (!stream.readWchar(&singleWChar))
					return NULL;
				return return_fixer(PyUnicodeUCS2_FromWideChar(&singleWChar, 1));
			}

			case Op_PyUnicodeString:
			{
				uint32 strlen;
				char* strptr;
				if (!stream.readExSize(&strlen))
					return NULL;
	
				if (!stream.readString(&strptr, strlen))
					return NULL;
	
				return return_fixer(PyUnicodeUCS2_DecodeUTF8(strptr, strlen));
			}

			// unknown.. but somehow related to the cPickle implementation
			case Op_Pyunk1:
			{
				// this function reads a string with extended size from the stream....
				ASCENT_HARDWARE_BREAKPOINT;
				return NULL;
			}

			case Op_PyEmptyTuple:
			{
				return return_fixer(PyTuple_New(0));
			}
			
			case Op_PyOneTuple:
			{
				ASCENT_HARDWARE_BREAKPOINT;
				PyTuple& tuple = *PyTuple_New(1);
				if ((opcode & 0x40) != 0)
				{
					if(!sharedObjectsMap.StoreReferencedObject(&tuple))
						return NULL;
				}
	
				// warning recursive function...
				tuple[0] = unmarshal(stream);
				return return_fixer(&tuple);
			}

			case Op_PyTwoTuple:
			{
				ASCENT_HARDWARE_BREAKPOINT;
				PyTuple& tuple = *PyTuple_New(2);
				if ((opcode & 0x40) != 0)
				{
					if(!sharedObjectsMap.StoreReferencedObject(&tuple))
						return NULL;
				}
	
				// warning double recursive function...
				for (int i = 0; i < 2; i++)
				{
					tuple[i] = unmarshal(stream);
				}
				return return_fixer(&tuple);
			}
			
			case Op_PyTuple:
			{
				ASCENT_HARDWARE_BREAKPOINT;
				uint32 elementCount;
				if (!stream.readExSize(&elementCount))
					return NULL;
	
				PyTuple& tuple = *PyTuple_New(elementCount);
				if ((opcode & 0x40) != 0)
				{
					if(!sharedObjectsMap.StoreReferencedObject(&tuple))
						return NULL;
				}
	
				// warning double recursive function...
				for (uint32 i = 0; i < elementCount; i++)
				{
					tuple[i] = unmarshal(stream);
				}
				return return_fixer(&tuple);
			}

			case Op_PyEmptyList:
			{
				return return_fixer(PyList_New(0));
			}

			case Op_PyOneList:
			{
				PyList &list = *PyList_New(1);
				if (&list == NULL)
					return NULL;
				
				if ((opcode & 0x40) != 0)
				{
					if(!sharedObjectsMap.StoreReferencedObject(&list))
						return NULL;
				}
				// recursive function...
				list[0] = unmarshal(stream);
	
				return return_fixer(&list);
			}

			case Op_PyList:
			{
				uint32 elementCount;
				if (!stream.readExSize(&elementCount))
					return NULL;
	
				PyList &list = *PyList_New(elementCount);
				if (&list == NULL)
					return NULL;
	
				if ((opcode & 0x40) != 0)
				{
					if(!sharedObjectsMap.StoreReferencedObject(&list))
						return NULL;
				}
	
				// n recursive function call
				for (uint32 i = 0; i < elementCount; i++)
				{
					list[i] = unmarshal(stream);
				}
				return return_fixer(&list);
			}
			
			/* this handler is a lot more complicated than I have implemented here... will do for now... */
			case Op_PyDict:
			{
				uint32 elementCount;
				if (!stream.readExSize(&elementCount))
					return NULL;
	
				PyDict & dict = *PyDict_New();
				if ((opcode & 0x40) != 0)
				{
					if(!sharedObjectsMap.StoreReferencedObject(&dict))
						return NULL;
				}
	
				// whoo tricky stuff....
				for (uint32 i = 0; i < elementCount; i++)
				{
					PyObject* keyNameType = (PyObject*)unmarshal(stream); // this should read the keyname from the stream
					if (keyNameType->gettype() != PyTypeString)
					{
						Log.Error("marshal::load","bigtime error in the dict unmarshalling we expecting a string..");
						assert(false);
					}
	
					PyString* keyName = (PyString*)keyNameType;
	
					// EVIL HACKY SOLLUTION WARNING!!!!!!!!!!!!!!!!!!!!!!!
					dict[keyName->content()] = unmarshal(stream);
				}
				return return_fixer(&dict);
			}

			/* read a object, it also has a check for reference mapping */
			case Op_PyObject:
			{
				return NULL;
			}

			/* unknown isn't handled but what we know of them is that its related to the cPickle system */
			case Op_Pyunk2:
			{
				return NULL;
			}
			
			/* need to implement custom callbacks and reading stuff... but for the server this doesn't seem to usefull.. */
			case Op_PySubStruct:
			{
				return NULL;
			}
			
			case Op_PyLoadcPickledObject:
			{
				/* |extended size|stream| */
				//return return_fixer(PyObject_CallMethod(unpickledObject, "load", 0);
				return NULL;
			}
		
			case Op_LoadCachedUnpickledObjectFunction:
			{
				return NULL;
			}

			case Op_PySavedStreamElement:
			{
				uint32 index;
				if (!stream.readExSize(&index))
					return NULL;
				void* obj;
				if (!sharedObjectsMap.GetStoredObject(index,&obj))
				{
					Log.Error("MarshalStream", "(Op_PySavedStreamElement)there seems to be a Invalid TY_REFECENCE in the stream");
					return NULL;
				}
				return return_fixer(obj);
			}

			/* this is fun as check the clients packet hash.... */
			case Op_PyChecksumedStream:
			{
				uint32 clientHash;
				if (!stream.readInt4(&clientHash))
					return NULL;
				
				Bytef* data = &stream.content()[stream.tell()];
				uInt len = (uInt)stream.size() - stream.tell();
				uint32 serverHash = adler32(0, data, len);
				if (clientHash != serverHash)
				{
					Log.Error("MarshalStream", "(Op_PyChecksumedStream) the checksum fails... client send us a 'oepsie' or he's just plain stupid");
					return NULL;
				}
				else
				{
					return return_fixer(unmarshal(stream));
				}
			}


			case Op_NewObject1:
			{
				//return return_fixer(sub_10097BB0(v4, a2, (opcode >> 6) & 0xFFFFFF01);
				return NULL;
			}

			case Op_NewObject2:
			{
				//return return_fixer(sub_10097E50(a2, (opcode >> 6) & 0xFFFFFF01);
				return NULL;
			}

			case Op_PyPackedRow:
			{
				//return return_fixer(sub_100706A0(v4);
				return NULL;
			}

			case Op_PySubStream:
			{
				//return return_fixer(sub_10095D30(a2);
				return NULL;
			}

			case Op_PyVarInteger:
			{
				return return_fixer(readVarInteger(stream, (opcode >> 6) & 1));
			}

			default:
			{
				Log.Error("MarshalStream", "Invalid type tag %d in stream.", opcode);
				return NULL;
			}
		}
		return NULL;
	}
	Log.Error("MarshalStream", "not anough data in the stream to read a aditional character");
	return NULL;
}

void* MarshalStream::readNewObject1(PyReadStream & stream, bool shared)
{
	return NULL;
}

void* MarshalStream::readNewObject2(PyReadStream & stream, bool shared)
{
	return NULL;
}

PyObject* MarshalStream::readVarInteger(PyReadStream & stream, bool shared)
{
	uint8 len;
	if(!stream.readInt1(&len))
		return NULL;

	return return_fixer(_ByteArray_AsPyLong(&stream.content()[stream.tell()], len));
}

PyString* MarshalStream::ReadBuffer(PyReadStream & stream)
{
	if (stream.size() - stream.tell() < 1)
	{
		//do error handling
		return NULL;
	}

	uint32 size;
	if (!stream.readInt1(&size))
		return NULL;
	if (size == 0xFF)
	{
		if (!stream.readInt4(&size))
			return NULL;
	}

	// doing the creating of the object in 1 evil try
	PyString* ret = PyString_FromStringAndSize((const char*)stream.content()[stream.tell()], size);
	stream.seek(size, STREAM_SEEK_CUR);
	return ret;
}
