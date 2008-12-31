#include "PyStringTable.h"

createFileSingleton(PyMarshalStringTable);

PyMarshalStringTable::PyMarshalStringTable()
{
	for (size_t i = 0; i < StringTableSize; i++)
	{
		uint32 hashValue = hash(StringTableData[i]);
		mStringTable[hashValue] = static_cast<uint8>(i);
	}
}

PyMarshalStringTable::~PyMarshalStringTable(){}

/* lookup a index using a string */
size_t PyMarshalStringTable::LookupIndex(std::string &str)
{
	mLock.Acquire();
	uint32 hashValue = hash(str);
	StringMapConstItr Itr = mStringTable.find(hashValue);
	if (Itr != mStringTable.end())
	{
		mLock.Release();
		return Itr->second;
	}
	mLock.Release();
	return -1;
}

/* lookup a index using a string */
size_t PyMarshalStringTable::LookupIndex(const char* str)
{
	// I am lazy... so I do it this way
	std::string _str(str);
	return LookupIndex(_str);
}

bool PyMarshalStringTable::LookupString(uint8 index, std::string &str)
{
	mLock.Acquire();
	if (index > StringTableSize)
	{
		str = "";
		mLock.Release();
		return false;
	}

	str = StringTableData[index];
	mLock.Release();
	return true;
}
