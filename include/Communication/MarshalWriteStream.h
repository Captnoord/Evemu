#ifndef _MARSHALWRITESTREAM_H
#define _MARSHALWRITESTREAM_H

#define MWS_DEFAULT_BUFFER_SIZE 0x10000

/**
 * \class MarshalWriteStream
 *
 * @brief This class wraps us all the write stuff related to the marshal stream.
 *
 * 
 *
 * @author Captnoord
 * @date February 2009
 */
class MarshalWriteStream
{
public:
	MarshalWriteStream();
	~MarshalWriteStream();

	size_t size() {return mWriteIndex;}
	size_t allocatedsize() {return mAllocatedBufferSize;}
private:
	size_t mWriteIndex;
	size_t mAllocatedBufferSize;
	void* mBuffer;
};

#endif //_MARSHALWRITESTREAM_H