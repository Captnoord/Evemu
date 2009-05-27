/*
	------------------------------------------------------------------------------------
	LICENSE:
	------------------------------------------------------------------------------------
	This file is part of EVEmu: EVE Online Server Emulator
	Copyright 2006 - 2009 The EVEmu Team
	For the latest information visit http://evemu.mmoforge.org
	------------------------------------------------------------------------------------
	This program is free software; you can redistribute it and/or modify it under
	the terms of the GNU Lesser General Public License as published by the Free Software
	Foundation; either version 2 of the License, or (at your option) any later
	version.

	This program is distributed in the hope that it will be useful, but WITHOUT
	ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
	FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License along with
	this program; if not, write to the Free Software Foundation, Inc., 59 Temple
	Place - Suite 330, Boston, MA 02111-1307, USA, or go to
	http://www.gnu.org/copyleft/lesser.txt.
	------------------------------------------------------------------------------------
	Author:		Captnoord
*/

#ifndef _HEXASCIIMODULE_H
#define _HEXASCIIMODULE_H

/**
 * \class HexAsciiModule
 *
 * @brief 
 *
 * no long description for the moment.
 *
 * @author Captnoord.
 * @date February 2009
 */
class HexAsciiModule
{
public:
	/**
	 * function to write a buffer to a file handle formatted into a hex edit view.
	 * @note function originating from 'Burlex' his utility framework so credits for this function goes to him.
	 */
	static void print_hexview(FILE *dstFile, const void *pSource, size_t sourceLength)
	{
		fputs("--------|------------------------------------------------|----------------|\n", dstFile);
		fputs(" offset |00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F |0123456789ABCDEF|\n", dstFile);
		fputs("--------|------------------------------------------------|----------------|\n", dstFile);

		size_t i = 0;
		size_t c = 0;
		size_t start;
		size_t written;
		uint8 byte;
		const unsigned char *pData = (const unsigned char *)pSource;

		for( ; i < sourceLength; )
		{
			start = i;
			fprintf(dstFile,"%08X|", i);
			for( c = 0; c < 16 && i < sourceLength; )		// write 16 bytes per line
			{
				fprintf(dstFile, "%02X ", (int)pData[i]);
				++i; ++c;
			}

			written = c;
			for( ; c < 16; ++c )							// finish off any incomplete bytes
				fputs("   ", dstFile);

			// write the text part
			fputc('|', dstFile);
			for( c = 0; c < written; ++c )
			{
				byte = pData[start + c];
				if( isprint((int)byte) )
					fputc((char)byte, dstFile);
				else
					fputc('.', dstFile);
			}

			for( ; c < 16; ++c )
				fputc(' ', dstFile);

			fputs("|\n", dstFile);
		}

		fputs("---------------------------------------------------------------------------\n", dstFile);
	}

	/**
	 * writes a buffer as hexadecimal string to a file handle.
	 */
	static void hexlify(FILE * fd, const char* src, const size_t srclen, bool upper = false)
	{
		size_t i, j;
		char ch = 'a';
		if (upper == true)
			ch = 'A';

		/* make hex version of string, taken from shamodule.c */
		for (i=j=0; i < srclen; i++) {
			char c;
			c = (src[i] >> 4) & 0xf;
			c = (c>9) ? c+ch-10 : c + '0';
			fputc(c, fd);
			c = src[i] & 0xf;
			c = (c>9) ? c+ch-10 : c + '0';
			fputc(c, fd);
		}
	}
};
#endif // _HEXASCIIMODULE_H

