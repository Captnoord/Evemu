#ifndef _MEMSTREAM_H
#define _MEMSTREAM_H

#include "Common.h"
#include "DebugTools.h"

typedef enum _memStreamOrigin
{
	STREAM_SET,
	STREAM_CUR,
	STREAM_END,
} STREAM_ORIGIN;

#define DEFAULT_MEM_STREAM_SIZE 0x1000

#define SafeFree(x) {ASCENT_FREE(x); x = NULL;}

/**
 * \class memStream
 *
 * @brief a class for writing and reading from memory
 *
 * 
 *
 * @author Captnoord
 * @date January 2009
 */
class memStream
{
public:

	/**
	 * @brief Initial class constructor.
	 *
	 * 
	 *
	 */
	memStream()
	{
		mOffset = 0;
		mAllocatedSize = DEFAULT_MEM_STREAM_SIZE;
		mContent = (uint8*)ASCENT_MALLOC(DEFAULT_MEM_STREAM_SIZE);
	}

	/**
	 * @brief class constructor with initial size parameter.
	 *
	 * 
	 *
	 * @param[in] size is the amount of preallocated bytes.
	 */
	memStream(size_t size)
	{
		mOffset = 0;
		mAllocatedSize = size;
		mContent = (uint8*)ASCENT_MALLOC(size);
	}

	/**
	 * @brief class destructor and it doesn't do anything special.
	 *
	 * 
	 *
	 */
	~memStream()
	{
		SafeFree(mContent);
		mContent = NULL;
	}

	/**
	 * @brief a function to set the read/write offset
	 *
	 * 
	 *
	 * @param[in] offset is the location relevant to the origin rule
	 * @param[in] origin is the offset origin
	 */
	void seek(uint32 offset, STREAM_ORIGIN origin);

	/**
	 * @brief function to get the current read/write index.
	 *
	 * 
	 *
	 * @return current read/write index.
	 */
	uint32 tell();
	
	/**
	 * @brief get the buffer of the stream
	 *
	 * 
	 *
	 * @return the buffer pointer
	 */
	uint8* content() {return mContent;}

	/**
	 * @brief function to get the size of the buffer
	 *
	 * 
	 *
	 * @return size_t the size of the buffer
	 */
	size_t size() {return mSize;}

	/**
	 * @brief reserve a amount of memory
	 *
	 * 
	 *
	 * @param[in] the amount of memory required to be reserved.
	 * @return the reserved amount of memory
	 * @todo make the return thing work.
	 */
	size_t reserve(size_t size)
	{
		if (mOffset + size > mSize)
		{
			mContent = (uint8*)ASCENT_REALLOC(mContent, mOffset + size);
			return size;
		}
		else
		{
			return size;
		}
	}

	/**
	 * @brief writes a single byte to the stream regardless of the type
	 *
	 * 
	 *
	 * @param[in] value is the variable that needs a single byte added to the stream.
	 */
	template <typename T>
	void write1(T & value)
	{
		reserve(1);
		mContent[mOffset++] = *((uint8*)&value); mOffset+=1;
	}

	/**
	 * @brief writes 2 byte's to the stream regardless of the type
	 *
	 * 
	 *
	 * @param[in] value is the variable that needs 2 byte's added to the stream.
	 */
	template <typename T>
	void write2(T & value)
	{
		ASCENT_ASSERT(sizeof(value) >= 2);
		reserve(2);
		*((uint16*)&mContent[mOffset]) = *((uint16*)&value); mOffset+=2;
	}

	/**
	 * @brief writes 4 byte's to the stream regardless of the type
	 *
	 * 
	 *
	 * @param[in] value is the variable that needs 4 byte's added to the stream.
	 */
	template <typename T>
	void write4(T & value)
	{
		ASCENT_ASSERT(sizeof(value) >= 4);
		reserve(4);
		*((uint32*)&mContent[mOffset]) = *((uint32*)&value); mOffset+=4;
	}

	/**
	 * @brief writes 8 byte's to the stream regardless of the type
	 *
	 * 
	 *
	 * @param[in] value is the variable that needs 8 byte's added to the stream.
	 */
	template <typename T>
	void write8(T & value)
	{
		ASCENT_ASSERT(sizeof(value) >= 8);
		reserve(8);
		*((uint64*)&mContent[mOffset]) = *((uint64*)&value); mOffset+=8;
	}

	size_t write(const uint8 * data, size_t len)
	{
		if (data == NULL || len == 0)
			return 0;

		reserve(len);
		ASCENT_MEMCPY(&mContent[mOffset], &data, len); mOffset+=(uint32)len;

		return len;
	}
	
	template <typename T>
	T read1()
	{

	}

	template <typename T>
	T read2()
	{

	}

	template <typename T>
	T read4()
	{

	}

	template <typename T>
	T read8()
	{

	}

	/**
	 * @brief read a [n] amount of bytes from the stream.
	 *
	 * 
	 *
	 * @param[in] len is the amount of bytes
	 * @param[in] dest is the buffer that is going to contain the readed data.
	 * @param[out] dest contains the data that is read.
	 * @return the amount of bytes readed from the stream.
	 */
	size_t read(uint8 *dest, size_t len);

protected:

	uint8 * mContent;
	uint32	mOffset;
	size_t	mSize;
	size_t	mAllocatedSize;
};

#endif //_MEMSTREAM_H