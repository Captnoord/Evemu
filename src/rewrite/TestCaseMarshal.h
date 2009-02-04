#ifndef _TEST_CASE_MARSHAL_H
#define _TEST_CASE_MARSHAL_H

/************************************************************************/
/* Marshal packet testing                                               */
/************************************************************************/

static uint32 readTestIndex = 0;
static uint8* readTestBuffer = NULL;
static size_t readTestBufferSize = 0;

template <typename T>
static T readChunk()
{
	uint8 typeSize = sizeof(T);

	// read overFlow
	if (readTestIndex + typeSize > readTestBufferSize)
	{
		Log.Error("TestCase::Marshal", "Read overflow in Marshal packet test");
		T errorReturn;
		memset(&errorReturn, 0, typeSize);
		return errorReturn;
	}
	else
	{
		T * chunkEntry = (T*)&readTestBuffer[readTestIndex];
		readTestIndex += typeSize;
		return *chunkEntry;
	}
}

static uint32 readExSize()
{
	uint32 size = readChunk<uint8>();
	if (size == 0xFF)
	{
		size = readChunk<uint32>();
	}
	return size;
}

static uint8 readSize()
{
	uint8 size = readChunk<uint8>();
	return size;
}

static std::string readString(uint32 size)
{
	std::string ret;
	ret.resize(size);

	memcpy(&ret[0], &readTestBuffer[readTestIndex], size);
	readTestIndex+=size;
	return ret;
}

static void testmarshal(PyStream * packet, uint32 depth = -1)
{
#define TestLog(x) {for (uint32 i = 0; i < depth; i++) putc('\t', stdout); sLog.String(x);}
#define TestLogf(x, j) {for (uint32 i = 0; i < depth; i++) putc('\t',stdout); sLog.String(x,j);}

	// keep track of the function call depth
	depth++;

	if (depth == 0)
	{
		readTestIndex = 0;
		readTestBuffer = packet->content();
		readTestBufferSize = packet->size();
	}


	uint8 opcode = readChunk<uint8>() & PyRepOpcodeMask;
	switch (opcode)
	{
	case Op_PyNone:
		{
			TestLog("PyNone");
		} break;

	case Op_PyByteString:
		{
			uint32 strlen = readExSize();
			std::string str = readString(strlen);
			TestLogf("PyString: %s (I think this is raw data... packet.... lol....)", str.c_str());
		} break;

	case Op_PyLongLong:
		{
			uint64 longlong = readChunk<uint64>();
			TestLogf("PyNumber: "I64FMTD, longlong);			
		} break;

	case Op_PyLong:
		{
			uint32 pylong = readChunk<uint32>();
			TestLogf("PyNumber: %u", pylong);

		} break;

	case Op_PySignedShort:
		{
			int16 pyshort = readChunk<int16>();
			TestLogf("PyNumber: %u", pyshort);
		}	break;

	case Op_PyByte:
		{
			int8 pyshort = readChunk<int8>();
			TestLogf("PyNumber: %u", pyshort);
		} break;

	case Op_PyInfinite:
		{ 
			TestLog("PyNumber: infinite");
		} break;

	case Op_PyZeroInteger:
		{
			TestLog("PyNumber: 0");
		} break;

	case Op_PyOneInteger:
		{
			TestLog("PyNumber: 1");
		} break;

	case Op_PyFloat:
		{
			double value = readChunk<double>();
			TestLogf("PyReal: %f", value);
		} break;

	case Op_PyZeroFloat:
		{
			TestLog("PyReal 0.0");
		} break;

	case Op_PyBuffer:
		{
			uint32 strlen = readExSize();
			std::string str = readString(strlen);
			TestLogf("PyString: %s", str.c_str());		
		} break;

	case Op_PyEmptyString: {
		TestLog("PyString: \"\"");
		} break;

	case Op_PyCharString:
		{
			std::string str = readString(1);
			TestLogf("PyString: %s", str.c_str());
		} break;

	case Op_PyShortString:
		{
			uint8 len = readSize();
			std::string str = readString(len);
			TestLogf("PyString: %s", str.c_str());
		} break;

	case Op_PyStringTableItem:
		{
			uint8 index = readChunk<uint8>();
			std::string str;

			sPyStringTable.LookupString(index, str);
			TestLogf("PyString: %s", str.c_str());
		} break;

	case Op_PyUnicodeByteString:
		{

			TestLog("PyStringTableItem not supported atm");
		} break;

	case Op_PyTuple:
		{
			uint8 len = readSize();
			TestLogf("PyTuple with %u items", len);
			for (uint32 i = 0; i < len; i++)
			{
				testmarshal(NULL, depth);
			}
		} break;

	case Op_PyList:
		{

		} break;

	case Op_PyDict:
		{

		} break;

	case Op_PyObject:
		{

		} break;


		//stream custom data with callback method
	case Op_PySubStruct:
		{
			// This is really a remote object specification


		} break;

	case Op_PySavedStreamElement:
		{

		} break;

	case Op_PyChecksumedStream:
		{

		} break;

	case Op_PyTrue:
		{	
		} break;

	case Op_PyFalse:
		{	
		} break;

	case Op_NewObject1:
	case Op_NewObject2:
		{

		} break;

	case Op_PyEmptyTuple:
		{

		} break;

	case Op_PyOneTuple:
		{
			TestLog("PyTuple with 1 item");
			testmarshal(NULL, depth);
		} break;

	case Op_PyEmptyList:
		{

		} break;

	case Op_PyOneList:
		{

		} break;

	case Op_PyEmptyUnicodeString:
		{
		} break;

		// TODO: check this one...
	case Op_PyUnicodeCharString:
		{

		} break;

	case Op_PyPackedRow:
		{

		} break;

	case Op_PySubStream:
		{

		} break;

	case Op_PyTwoTuple:
		{
			uint8 len = 2;
			TestLogf("PyTuple with %u items", len);
			for (uint32 i = 0; i < len; i++)
			{
				testmarshal(NULL, depth);
			}
		} break;

	case Op_PackedTerminator:
		{

		} break;

	case Op_PyUnicodeString:
		{

		} break;

	case Op_PyVarInteger:
		{

		} break;

		/************************************************************************/
		/* Unhandled Marshalled objects                                         */
		/************************************************************************/
	case 0x13: //19 // something related to PyBuffers and 
		{
			assert(false);
			printf("Unhandled opcode: %u\n", opcode & PyRepOpcodeMask);
		} break;

	case 0x18: //24 // I think something like InvalidateCachedObjects
		{
			assert(false);
			printf("Unhandled opcode: %u\n", opcode & PyRepOpcodeMask);
		} break;

	case 0x1A: //26		// FACT: python cPickled object with extended size 
		{ 
			assert(false);
			printf("Unhandled opcode: %u\n", opcode & PyRepOpcodeMask);
		} break;

	case 0x21: //33		//FACT: python cPickle object : dump....
		{
			assert(false);
			printf("Unhandled opcode: %u\n", opcode & PyRepOpcodeMask);
		} break;

	default:
		{
			printf("Default: Unhandled opcode: %u\n", opcode & PyRepOpcodeMask);
			assert(false);
		} break;
	}
}
/************************************************************************/
/* End marshal packet testing                                           */
/************************************************************************/

#endif//_TEST_CASE_MARSHAL_H