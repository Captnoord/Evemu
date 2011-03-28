#ifndef _RLEMODULE_H
#define _RLEMODULE_H

/* RLE String module, contains both compress and uncompress routines */

class RleModule
{
public:

	void PackZeroCompressed(const uint8 *src, uint32 len, std::vector<uint8> &out_buf)
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
	}

    static bool encode(const uint8 *src, uint32 len, std::vector<uint8> &out_buf);

	/**
	 * \brief decode decodes the Zero 'Rle' data.
	 *
	 * 
	 *
	 * @param[in] src the source buffer.
 	 * @param[in] src_len the length of the source buffer.
	 * @param[out] dst the destination buffer, needs to be large enough.
	 * @param[out] dst_len the length of the destination buffer.
	 * @return true if decoded correctly and false if a error has happened.
	 * @todo clean this function up, so it doesn't look as ugly as it does now.
	 * @note as my teacher always screamed at me "DON'T USE GOTO'S AND LABELS".
	 */
	static bool decode(unsigned char* src, const size_t src_len, unsigned char* dst, size_t* dst_len);

	// ZeroCompress OpCode
	struct ZCOpCode {
		// first part
		uint8 f_len : 3;
		bool f_isZero : 1;

		// second part
		uint8 s_len : 3;
		bool s_isZero : 1;
	};

#define _log(x,...) //

	typedef std::vector<uint8> zeropackbuff;
	// for verify purposes
	static void UnpackZeroCompressed(const uint8 *in_buf, uint32 in_length, zeropackbuff &buffer) {
		buffer.clear();
		if(in_buf == NULL || in_length == 0)
			return;

		_log(NET__ZEROINFL, "Zero-inflating buffer of length %d", in_length );

		ZCOpCode opcode;
		const uint8 *end = in_buf + in_length;

		while(in_buf < end) {
			opcode = *(ZCOpCode *)(in_buf++);

			if(opcode.f_isZero) {
				uint8 count = opcode.f_len+1;
				_log(NET__ZEROINFL, "    Opcode 0x%x (first part) yields %d zero bytes at %d", opcode, count, buffer.size());
				for(; count > 0; count--) {
					buffer.push_back(0);
				}
			} else {
				uint8 count = 8-opcode.f_len;
				_log(NET__ZEROINFL, "    Opcode 0x%x (first part) yields %d data bytes at %d", opcode, count, buffer.size());
				//caution: if we are at the end of the buffer, its possible to be processing a '0' opcode, which should mean "8 bytes", but really means "end"
				for(; count > 0 && in_buf < end; count--) {
					buffer.push_back(*in_buf++);
				}
			}

			if(opcode.s_isZero) {
				uint8 count = opcode.s_len+1;
				_log(NET__ZEROINFL, "    Opcode 0x%x (second part) yields %d zero bytes at %d", opcode, count, buffer.size());
				for(; count > 0; count--) {
					buffer.push_back(0);
				}
			} else {
				uint8 count = 8-opcode.s_len;
				_log(NET__ZEROINFL, "    Opcode 0x%x (second part) yields %d data bytes at %d", opcode, count, buffer.size());
				//caution: if we are at the end of the buffer, its possible to be processing a '0' opcode, which should mean "8 bytes", but really means "end"
				for(; count > 0 && in_buf < end; count--) {
					buffer.push_back(*in_buf++);
				}
			}
		}

		_log(NET__ZEROINFL, "  Zero-inflating buffer yields %d inflated bytes (ratio %.02f)", buffer.size(), double(in_length) / double(buffer.size()));
	}
};
#endif // _RLEMODULE_H

