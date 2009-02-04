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

#ifndef FILE_LOGGER_H
#define FILE_LOGGER_H

/**
 * \class FileLogger
 *
 * @brief a logger class so we can (us developers) dump what we are actualy are sending.
 *
 * 
 * @todo rename this class into something like PacketLogger ( hell i'm lazy )
 * @author Captnoord
 * @date December 2008
 */

class FileLogger : public Singleton<FileLogger>
{
public:
	FileLogger(const char * path) : mEnableFileWrite(false)
	{
		fouttxt = fopen(path,"wb");
	}

	~FileLogger()
	{
		fflush(fouttxt);
		fclose(fouttxt);
	}

	void enable() {mEnableFileWrite = true;}
	void disable() {mEnableFileWrite = false;}

	/**
	 * @brief logPacket is a function that dumps a PyTupleStream to a file.
	 *
	 * 
	 *
	 * @param[in] packet is the stream that needs to be dumped.
	 */
	void logPacket(PyTupleStream* packet, int direction)
	{
		if (mEnableFileWrite == false)
			return;

		if (packet == NULL)
			return;

		logPacket(packet->content(), packet->size(), direction);
	}

	void logSimplePacket(PyTupleStream* packet, int direction)
	{
		if (mEnableFileWrite == false)
			return;

		if (packet == NULL)
			return;

		logSimplePacket(packet->content(), packet->size(), direction);
	}

#define ENDING_WINDOWS "\r\n"
#define ENDING_UNIX "\n"
// use unix line endings..
#define LINE_END ENDING_UNIX
	
	/**
	 * @brief logPacket is a function that dumps a buffer
	 *
	 * "Part where I credit burlex for using his hex dump function"
	 *
	 * @param[in] data is the buffer that contains the data that needs to be written.
	 * @param[in] size is the length of the data buffer.
	 * @param[in] direction is the direction of the buffer... if its client -> server or server -> client.
	 */
	void logPacket(const unsigned char* data, size_t size, unsigned int direction)
	{
		unsigned int line = 1;
		unsigned int countpos = 0;
		size_t lenght = size;

		fprintf(fouttxt, "{%s} PacketSize = %u"LINE_END, (direction ? "SERVER" : "CLIENT"), lenght);
		fprintf(fouttxt, "|------------------------------------------------|----------------|"LINE_END);
		fprintf(fouttxt, "|00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F |0123456789ABCDEF|"LINE_END);
		fprintf(fouttxt, "|------------------------------------------------|----------------|"LINE_END);

		if(lenght > 0)
		{
			fprintf(fouttxt, "|");
			for (unsigned int count = 0 ; count < lenght ; count++)
			{
				if (countpos == 16)
				{
					countpos = 0;

					fprintf(fouttxt,"|");

					for (unsigned int a = count-16; a < count;a++)
					{
						if ((data[a] < 32) || (data[a] > 126))
							fprintf(fouttxt,".");
						else
							fprintf(fouttxt,"%c",data[a]);
					}

					fprintf(fouttxt,"|"LINE_END);

					line++;
					fprintf(fouttxt,"|");
				}

				fprintf(fouttxt,"%02X ",data[count]);

				//FIX TO PARSE PACKETS WITH LENGHT < OR = TO 16 BYTES.
				if (count+1 == lenght && lenght <= 16)
				{
					for (unsigned int b = countpos+1; b < 16;b++)
						fprintf(fouttxt,"   ");

					fprintf(fouttxt,"|");

					for (unsigned int a = 0; a < lenght;a++)
					{
						if ((data[a] < 32) || (data[a] > 126))
							fprintf(fouttxt,".");
						else
							fprintf(fouttxt,"%c",data[a]);
					}

					for (unsigned int c = count; c < 15;c++)
						fprintf(fouttxt," ");

					fprintf(fouttxt,"|"LINE_END);
				}

				//FIX TO PARSE THE LAST LINE OF THE PACKETS WHEN THE LENGHT IS > 16 AND ITS IN THE LAST LINE.
				if (count+1 == lenght && lenght > 16)
				{
					for (unsigned int b = countpos+1; b < 16;b++)
						fprintf(fouttxt,"   ");

					fprintf(fouttxt,"|");

					unsigned short print = 0;

					for (unsigned int a = line * 16 - 16; a < lenght;a++)
					{
						if ((data[a] < 32) || (data[a] > 126))
							fprintf(fouttxt,".");
						else
							fprintf(fouttxt,"%c",data[a]);

						print++;
					}

					for (unsigned int c = print; c < 16;c++)
						fprintf(fouttxt," ");

					fprintf(fouttxt,"|"LINE_END);
				}

				countpos++;
			}
		}
		fprintf(fouttxt, "-------------------------------------------------------------------"LINE_END""LINE_END);
		fflush(fouttxt);
	}

	// simple packets are streams we supply the header for its very slow... hehe
	void logSimplePacket(const unsigned char* xdata, size_t size, unsigned int direction)
	{
		unsigned char* data = new unsigned char[size + 5];
		
		memcpy(data+5, xdata, size);
		data[0] = 0x7E;
		data[1] = 0;
		data[2] = 0;
		data[3] = 0;
		data[4] = 0;

		unsigned int line = 1;
		unsigned int countpos = 0;
		size_t lenght = size + 5;

		fprintf(fouttxt, "{%s} PacketSize = %u"LINE_END, (direction ? "SERVER" : "CLIENT"), lenght);
		fprintf(fouttxt, "|------------------------------------------------|----------------|"LINE_END);
		fprintf(fouttxt, "|00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F |0123456789ABCDEF|"LINE_END);
		fprintf(fouttxt, "|------------------------------------------------|----------------|"LINE_END);

		if(lenght > 0)
		{
			fprintf(fouttxt, "|");
			for (unsigned int count = 0 ; count < lenght ; count++)
			{
				if (countpos == 16)
				{
					countpos = 0;

					fprintf(fouttxt,"|");

					for (unsigned int a = count-16; a < count;a++)
					{
						if ((data[a] < 32) || (data[a] > 126))
							fprintf(fouttxt,".");
						else
							fprintf(fouttxt,"%c",data[a]);
					}

					fprintf(fouttxt,"|"LINE_END);

					line++;
					fprintf(fouttxt,"|");
				}

				fprintf(fouttxt,"%02X ",data[count]);

				//FIX TO PARSE PACKETS WITH LENGHT < OR = TO 16 BYTES.
				if (count+1 == lenght && lenght <= 16)
				{
					for (unsigned int b = countpos+1; b < 16;b++)
						fprintf(fouttxt,"   ");

					fprintf(fouttxt,"|");

					for (unsigned int a = 0; a < lenght;a++)
					{
						if ((data[a] < 32) || (data[a] > 126))
							fprintf(fouttxt,".");
						else
							fprintf(fouttxt,"%c",data[a]);
					}

					for (unsigned int c = count; c < 15;c++)
						fprintf(fouttxt," ");

					fprintf(fouttxt,"|"LINE_END);
				}

				//FIX TO PARSE THE LAST LINE OF THE PACKETS WHEN THE LENGHT IS > 16 AND ITS IN THE LAST LINE.
				if (count+1 == lenght && lenght > 16)
				{
					for (unsigned int b = countpos+1; b < 16;b++)
						fprintf(fouttxt,"   ");

					fprintf(fouttxt,"|");

					unsigned short print = 0;

					for (unsigned int a = line * 16 - 16; a < lenght;a++)
					{
						if ((data[a] < 32) || (data[a] > 126))
							fprintf(fouttxt,".");
						else
							fprintf(fouttxt,"%c",data[a]);

						print++;
					}

					for (unsigned int c = print; c < 16;c++)
						fprintf(fouttxt," ");

					fprintf(fouttxt,"|"LINE_END);
				}

				countpos++;
			}
		}
		fprintf(fouttxt, "-------------------------------------------------------------------"LINE_END""LINE_END);
		fflush(fouttxt);
	}

protected:

	FILE * fouttxt;
	bool mEnableFileWrite;

#ifdef WIN32
	HANDLE mStdoutHandle, mStderrHandle;
#endif
};

#define sFileLogger FileLogger::getSingleton()

#endif//FILE_LOGGER_H
