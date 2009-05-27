#include "Common.h"
#include "ascent.h"
#include "RleModule.h"

bool RleModule::decode( unsigned char* src, const size_t src_len, unsigned char* dst, size_t* dst_len )
{
	bool unkBool1;
	unsigned int index;
	unsigned int write_index;
	unsigned char *tSrc;
	signed int unkInt1;
	int chunkSize;
	size_t unkInt2;
	int unkInt3;
	bool unkInt4;
	signed int unkInt5;

	size_t dst_size = *dst_len;

	unkBool1 = false;
	write_index = 0;
	index = 0;

	if ( src_len )
	{
		while ( write_index < dst_size )
		{
			tSrc = src;
			if ( unkBool1 )
			{
				unkInt1 = unkInt5 >> 4;
			}
			else
			{
				unkInt1 = (char)src[index];
				unkInt5 = src[index++];
			}
			chunkSize = (unkInt1 & 0xF) - 8;
			unkInt4 = unkBool1 == false;
			if ( chunkSize < 0 )
			{
				if ( write_index - chunkSize > dst_size )
				{
					Log.Error("RleModule", "Invalid RLE string");

					// this needs some love...
					ASCENT_HARDWARE_BREAKPOINT;

					return false;
				}

				unkInt2 = src_len;
				while ( 1 )
				{
					++chunkSize;
					if ( index >= (unsigned int)unkInt2 )
						goto LABEL_2;
					dst[write_index++] = tSrc[index++];
					if ( !chunkSize )
						goto LABEL_17;
				}
			}

			if ( chunkSize + write_index + 1 > dst_size )
			{
				Log.Error("RleModule", "Invalid RLE string");

				// this needs some love...
				ASCENT_HARDWARE_BREAKPOINT;

				return false;
			}

			unkInt3 = chunkSize + 1;
			memset(&dst[write_index], 0, chunkSize + 1);
			unkInt2 = src_len;
			write_index += unkInt3;
LABEL_17:
			if ( index >= (unsigned int)unkInt2 )
				goto LABEL_2;
			unkBool1 = unkInt4;
		}
	}
	else
	{
LABEL_2:
		if ( write_index < dst_size )
			memset(&dst[write_index], 0, dst_size - write_index);
	}

	/* I think this is correct.... but I actually don't know hehe */
	*dst_len = write_index;

	return true;
}

