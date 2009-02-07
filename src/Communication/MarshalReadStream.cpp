#include "Common.h"
#include "DebugTools.h"
#include "Log.h"
#include "NGLog.h"
#include "MarshalReadStream.h"

MarshalReadStream::MarshalReadStream() : mBuffer(NULL), mSize(0), mReadIndex(0) {}
MarshalReadStream::MarshalReadStream( const char* buff, size_t size ) : mBuffer(NULL), mSize(size), mReadIndex(0)
{
	resize(size);

	ASCENT_MEMCPY(mBuffer, buff, size);
}

MarshalReadStream::~MarshalReadStream()
{
	SafeFree(mBuffer);
}

bool MarshalReadStream::readBuffer( uint8 ** buffer, size_t len )
{
	if (mSize < mReadIndex + len)
		return false;

	*buffer = &mBuffer[mReadIndex];
	mReadIndex+=len;
	return true;
}

bool MarshalReadStream::readString( char ** buffer, size_t len )
{
	if (mSize < mReadIndex + len)
		return false;

	*buffer = (char*)&mBuffer[mReadIndex];
	mReadIndex+=len;
	return true;
}

bool MarshalReadStream::readWstring( wchar_t ** buffer, size_t len )
{
	if (mSize < mReadIndex + len)
		return false;

	*buffer = (wchar_t*)&mBuffer[mReadIndex];
	mReadIndex+=len;
	return true;
}

size_t MarshalReadStream::size()
{
	return mSize;
}

bool MarshalReadStream::resize( size_t newsize )
{
	// check if we have enough memory allocated.
	if( mAllocatedMem > newsize && mBuffer != NULL )
	{
		mSize = newsize;
		return true;
	}

	// we seem to need to allocate or reallocate memory...
	if (mBuffer != NULL)
	{
		mBuffer = (uint8*)ASCENT_REALLOC(mBuffer, newsize);
		mAllocatedMem = newsize;
	}
	else
	{
		mBuffer = (uint8*)ASCENT_MALLOC(newsize);
		mAllocatedMem = newsize;
	}

	// check if the memory is allocated
	if (mBuffer == NULL)
		return false;

	mSize = newsize;
	return true;
}

void MarshalReadStream::reset()
{
	mReadIndex = 0;
}

bool MarshalReadStream::set( uint8* buff, size_t size )
{
	if (buff == NULL || size <= 0)
		return false;
	resize(size);
	ASCENT_MEMCPY(mBuffer, buff, size);
	reset();
	return true;
}

bool MarshalReadStream::seek( int32 offset, int origin )
{
	switch (origin)
	{
	case SEEK_SET:
		{
			// (signed) maybe break the stuff...
			if (offset < (signed)mSize)
				mReadIndex = offset;
			else
				return false;
		} return true;
	case SEEK_CUR:
		{
			if (mReadIndex + offset < mSize)
				mReadIndex+=offset;
			else
				return false;
		} return true;
	case SEEK_END:
		{
			if (mSize - offset >= 0)
				mReadIndex = mSize - offset;
			else
				return false;
		} return true;
	default:
		{
			assert(false);
		} return false;
	}
	return false;
}

bool MarshalReadStream::readExSize( size_t & size )
{
	size_t _size = 0;
	if (!readInt1(_size))
		return false;

	if (_size == 0xFF)
	{
		if(!readInt4(_size))
			return false;
	}
	size = _size;
	return true;
}
