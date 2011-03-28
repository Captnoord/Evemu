/**
 * unit tests for evemu framework..
 */

//#include "Common.h"
#include "EvemuPCH.h"

/* rle encoding decoding test with random data */
bool unit_test_rle_module()
{
    /* salt it with 0 to make sure we can repeat the test with the same random value's */
    srand(0);

#define RLE_IN_BUFF_SIZE 0x1000
    uint8* inbuff = (uint8*)ASCENT_MALLOC(RLE_IN_BUFF_SIZE);
    assert(inbuff);

    /* fill the buffer with random data... */
    for (int i = 0; i < RLE_IN_BUFF_SIZE; i++)
    {
        inbuff[i] = rand() % 0xFF;
    }

    std::vector<uint8> out_buf;
    if (!RleModule::encode(inbuff, RLE_IN_BUFF_SIZE, out_buf))
    {
        sLog.Error("rle unit test", "encoding random data went wrong");
        return false;
    }

    /* its normal to expect the same amount of data decoded as we have encoded */
    uint8* outbuff = (uint8*)ASCENT_MALLOC(RLE_IN_BUFF_SIZE);
    assert(outbuff);

    size_t outsize;
    if (!RleModule::decode(&out_buf[0], out_buf.size(), outbuff, &outsize))
    {
        sLog.Error("rle unit test", "decoding random data went wrong");
        return false;
    }

    if (outsize != RLE_IN_BUFF_SIZE)
    {
        sLog.Error("rle unit test", "decoded data doesn't have the same length as the source something is wrong..");
        return false;
    }

    if (!memcmp(inbuff, outbuff, RLE_IN_BUFF_SIZE))
    {
        sLog.Error("rle unit test", "decoded data isn't the same as the data we encoded... so something is wrong.");
        return false;
    }

    return true;
}

int main(int argc, char* argv[])
{
    /* set the global time variable */
    UNIXTIME = time(NULL);
    g_localTime = *localtime(&UNIXTIME);

    if (!unit_test_rle_module())
    {
        sLog.Error("unit test", "unit_test_rle_module failed");
    }
	return 0;
}

