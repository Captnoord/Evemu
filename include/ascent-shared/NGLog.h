/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2008 Ascent Team <http://www.ascentemu.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef ASCENT_FRAMEWORKNGLOG_H
#define ASCENT_FRAMEWORKNGLOG_H

#include "Common.h"
#include "Singleton.h"
#include "Threading/Threading.h"


#ifdef WIN32

#define TRED FOREGROUND_RED | FOREGROUND_INTENSITY
#define TGREEN FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define TYELLOW FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY
#define TNORMAL FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE
#define TWHITE TNORMAL | FOREGROUND_INTENSITY
#define TBLUE FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY

#else

#define TRED 1
#define TGREEN 2
#define TYELLOW 3
#define TNORMAL 4
#define TWHITE 5
#define TBLUE 6

#endif

extern SERVER_DECL time_t UNIXTIME;		/* update this every loop to avoid the time() syscall! */
extern SERVER_DECL tm g_localTime;

// unix color strings
#ifndef WIN32
static const char* colorstrings[TBLUE+1] = {
	"",
	"\033[22;31m",
	"\033[22;32m",
	"\033[01;33m",
	//"\033[22;37m",
	"\033[0m",
	"\033[01;37m",
	"\033[1;34m",
};
#endif//WIN32

class SERVER_DECL CLog : public Singleton< CLog >
{
	#define LARGERRORMESSAGE_ERROR 1
	#define LARGERRORMESSAGE_WARNING 2

	#ifdef LOG_USE_MUTEX
	#	define LOCK_LOG mutex.Acquire()
	#	define UNLOCK_LOG mutex.Release();
	#else
	#	define LOCK_LOG 
	#	define UNLOCK_LOG 
	#endif//LOG_USE_MUTEX

public:

	CLog();

	void Line();
	void Color(unsigned int color);
	void Notice(const char * source, const char * format, ...);
	void Warning(const char * source, const char * format, ...);
	void Success(const char * source, const char * format, ...);
	void Error(const char * source, const char * format, ...);
	void Debug(const char * source, const char * format, ...);

	void LargeErrorMessage(uint32 Colour, ...);

protected:
	ASCENT_INLINE void Time();

#ifdef WIN32
	HANDLE stdout_handle, stderr_handle;
#endif//WIN32

#ifdef LOG_USE_MUTEX
	Mutex mutex;
#endif//LOG_USE_MUTEX

	int32 log_level;
};

#define Log CLog::getSingleton()

#endif

