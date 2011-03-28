#include "Common.h"
#include "ascent.h"
#include "RleModule.h"

bool RleModule::decode( unsigned char* src, const size_t src_len, unsigned char* dst, size_t* dst_len )
{
    bool unkBool1;
    unsigned int index;
    unsigned int write_index;
    unsigned char *tSrc;
    int unkInt1;
    int chunkSize;
    size_t unkInt2;
    int unkInt3;
    bool unkInt4;
    int unkInt5 = 0;

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

/* todo we need to add more error checking */
bool RleModule::encode( const uint8 *src, uint32 len, std::vector<uint8> &out_buf )
{
    /* get the end pointer */
    const uint8 *end = &src[len];

    while(src < end)
    {
        // we need to have enough room without moving (otherwise
        // it would invalidate our pointer obtained below); size
        // is 1 byte of opcode + at most 2x 8 bytes
        out_buf.reserve(out_buf.size() + 1 + 16);

        // insert opcode
        out_buf.push_back(0);	// insert opcode placeholder
        ZCOpCode *opcode = (ZCOpCode *)&out_buf.back();	// obtain pointer to it

        // encode first part
        if(*src == 0x00)
        {
            //we are starting with zero, hunting for non-zero
            opcode->f_isZero = true;
            opcode->f_len = 0;

            src++;
            for(; src < end && *src == 0x00 && opcode->f_len < 7; opcode->f_len++)
                src++;
        }
        else
        {
            //we are starting with data, hunting for zero
            opcode->f_isZero = false;
            opcode->f_len = 7;

            out_buf.push_back(*src++);
            for(; src < end && *src != 0x00 && opcode->f_len > 0; opcode->f_len--)
                out_buf.push_back(*src++);
        }

        if(src >= end)
            break;

        // encode second part
        if(*src == 0x00) {
            //we are starting with zero, hunting for non-zero
            opcode->s_isZero = true;
            opcode->s_len = 0;

            src++;
            for(; src < end && *src == 0x00 && opcode->s_len < 7; opcode->s_len++)
                src++;
        }
        else
        {
            //we are starting with data, hunting for zero
            opcode->s_isZero = false;
            opcode->s_len = 7;

            out_buf.push_back(*src++);
            for(; src < end && *src != 0x00 && opcode->s_len > 0; opcode->s_len--)
                out_buf.push_back(*src++);
        }
    }

    //_log(NET__ZEROCOMP,
    //	"  Zero-compressing buffer resulted in %lu bytes (ratio %.02f)",
    //	out_buf.size(), double(out_buf.size()) / double(len)
    //	);

    return true;
}
