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

#include "Common.h"
#include "Config/ConfigEnv.h"
#include "Log.h"
#include "NGLog.h"
#include <stdarg.h>

string FormatOutputString(const char * Prefix, const char * Description, bool useTimeStamp)
{
	char p[MAX_PATH];
	p[0] = 0;
	time_t t = time(NULL);
	tm * a = gmtime(&t);
	strcat(p, Prefix);
	strcat(p, "/");
	strcat(p, Description);
	if(useTimeStamp)
	{
		char ftime[100];
		snprintf(ftime, 100, "-%-4d-%02d-%02d %02d-%02d-%02d", a->tm_year+1900, a->tm_mon+1, a->tm_mday, a->tm_hour, a->tm_min, a->tm_sec);
		strcat(p, ftime);
	}

	strcat(p, ".log");
	return string(p);
}

createFileSingleton( oLog );
createFileSingleton( CLog );

SERVER_DECL time_t UNIXTIME;
SERVER_DECL tm g_localTime;

oLog::oLog()
{
	Init(-3,3);
}

void oLog::SetColor(unsigned int color)
{
#ifndef WIN32
	fputs(colorstrings[color], stdout);
#else
	SetConsoleTextAttribute(stdout_handle, (WORD)color);
#endif
}

void oLog::String( const char * str, ... )
{
#ifdef ENABLE_CONSOLE_LOG
	if(m_fileLogLevel < 0 && m_screenLogLevel < 0)
		return;

	va_list ap;
	va_start(ap, str);
	
	if(m_screenLogLevel >= 0)
	{
		vprintf(str, ap);
		putc('\n', stdout);
	}

	va_end(ap);
#endif//ENABLE_CONSOLE_LOG
}

void oLog::Error( const char * err, ... )
{
#ifdef ENABLE_CONSOLE_LOG
	if(m_fileLogLevel < 1 && m_screenLogLevel < 1)
		return;

	va_list ap;
	va_start(ap, err);

	if(m_screenLogLevel >= 1)
	{
		SetColor(TRED);
		vfprintf(stderr, err, ap);
		putc('\n', stderr);
		SetColor(TNORMAL);
	}

	va_end(ap);
#endif//ENABLE_CONSOLE_LOG
}

void oLog::Warning( const char * warning, ... )
{
#ifdef ENABLE_CONSOLE_LOG
	if(m_fileLogLevel < 1 && m_screenLogLevel < 1)
		return;

	va_list ap;
	va_start(ap, warning);

	if(m_screenLogLevel >= 1)
	{
		SetColor(TYELLOW);
		vfprintf(stdout, warning, ap);
		putc('\n', stdout);
		SetColor(TNORMAL);
	}

	va_end(ap);
#endif//ENABLE_CONSOLE_LOG
}

void oLog::Basic( const char * str, ... )
{
#ifdef ENABLE_CONSOLE_LOG
	if(m_fileLogLevel < 1 && m_screenLogLevel < 1)
		return;

	va_list ap;
	va_start(ap, str);

	if(m_screenLogLevel >= 1)
	{
		vprintf(str, ap);
		putc('\n', stdout);
	}

	va_end(ap);
#endif//ENABLE_CONSOLE_LOG
}

void oLog::Detail( const char * str, ... )
{
#ifdef ENABLE_CONSOLE_LOG
	if(m_fileLogLevel < 2 && m_screenLogLevel < 2)
		return;

	va_list ap;
	va_start(ap, str);

	if(m_screenLogLevel >= 2)
	{
		vprintf(str, ap);
		putc('\n', stdout);
	}

	va_end(ap);
#endif//ENABLE_CONSOLE_LOG
}

void oLog::Debug( const char * str, ... )
{
#ifdef ENABLE_CONSOLE_LOG
	if(m_fileLogLevel < 3 && m_screenLogLevel < 3)
		return;

	va_list ap;
	va_start(ap, str);

	if(m_screenLogLevel >= 3)
	{
		vprintf(str, ap);
		putc('\n', stdout);
	}

	va_end(ap);
#endif//ENABLE_CONSOLE_LOG
}

void oLog::Menu( const char * str, ... )
{
#ifdef ENABLE_CONSOLE_LOG
	va_list ap;
	va_start(ap, str);
	vprintf( str, ap );
	va_end(ap);
	fflush(stdout);
#endif//ENABLE_CONSOLE_LOG
}

void oLog::Init(int32 fileLogLevel, int32 screenLogLevel)
{
	m_screenLogLevel = screenLogLevel;
	m_fileLogLevel = fileLogLevel;

	// get error handle
#ifdef WIN32
	stderr_handle = GetStdHandle(STD_ERROR_HANDLE);
	stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
}

void oLog::SetScreenLoggingLevel(int32 level)
{
	m_screenLogLevel = level;
}

void oLog::SetFileLoggingLevel(int32 level)
{
	m_fileLogLevel = level;
}

void oLog::Color(uint32 colorcode, const char * str, ...)
{
#ifdef ENABLE_CONSOLE_LOG
	if( !str ) return;
	va_list ap;
	va_start(ap, str);
#ifdef WIN32
	SetConsoleTextAttribute(stdout_handle, WORD(colorcode));
#else
	printf(colorstrings[colorcode]);
#endif
	vprintf( str, ap );
	fflush(stdout);
	va_end(ap);
#endif//#ENABLE_CONSOLE_LOG
}

void SessionLogWriter::write(const char* format, ...)
{
	if(!m_file)
		return;

	va_list ap;
	va_start(ap, format);
	char out[32768];

	time_t t = time(NULL);
	tm* aTm = localtime(&t);
	sprintf(out, "[%-4d-%02d-%02d %02d:%02d:%02d] ",aTm->tm_year+1900,aTm->tm_mon+1,aTm->tm_mday,aTm->tm_hour,aTm->tm_min,aTm->tm_sec);
	size_t l = strlen(out);
	vsnprintf(&out[l], 32768 - l, format, ap);

	fprintf(m_file, "%s\n", out);
	va_end(ap);
}


void SessionLogWriter::Open()
{
	m_file = fopen(m_filename, "a");
}

void SessionLogWriter::Close()
{
	if(!m_file) return;
	fflush(m_file);
	fclose(m_file);
	m_file=NULL;
}

SessionLogWriter::SessionLogWriter(const char * filename, bool open)
{
	m_filename = _strdup(filename);
	m_file = NULL;
	if( open == true )
	{
		Open();
	}
}

SessionLogWriter::~SessionLogWriter()
{
	if( m_file != NULL)
	{
		Close();
	}

	free(m_filename);
}
