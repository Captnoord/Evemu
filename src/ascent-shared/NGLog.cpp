#include "Common.h"

#include "NGLog.h"
#include <stdarg.h>

CLog::CLog()
{
	log_level = 3;
#ifdef WIN32
	stderr_handle = GetStdHandle(STD_ERROR_HANDLE);
	stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
}

void CLog::Color(unsigned int color)
{
#ifndef WIN32
	fputs(colorstrings[color], stdout);
#else
	SetConsoleTextAttribute(stdout_handle, (WORD)color);
#endif
}

void CLog::Notice(const char * source, const char * format, ...)
{
#ifdef ENABLE_CONSOLE_LOG
	/* notice is old loglevel 0/string */
	LOCK_LOG;
	va_list ap;
	va_start(ap, format);
	Time();
	fputs("N ", stdout);
	if(*source)
	{
		Color(TWHITE);
		fputs(source, stdout);
		putchar(':');
		putchar(' ');
		Color(TNORMAL);
	}

	vprintf(format, ap);
	putchar('\n');
	va_end(ap);
	Color(TNORMAL);
	UNLOCK_LOG;
#endif//ENABLE_CONSOLE_LOG
}

void CLog::Warning(const char * source, const char * format, ...)
{
#ifdef ENABLE_CONSOLE_LOG
	if(log_level < 2)
		return;

	/* warning is old loglevel 2/detail */
	LOCK_LOG;
	va_list ap;
	va_start(ap, format);
	Time();
	Color(TYELLOW);
	fputs("W ", stdout);
	if(*source)
	{
		Color(TWHITE);
		fputs(source, stdout);
		putchar(':');
		putchar(' ');
		Color(TYELLOW);
	}

	vprintf(format, ap);
	putchar('\n');
	va_end(ap);
	Color(TNORMAL);
	UNLOCK_LOG;
#endif//ENABLE_CONSOLE_LOG
}

void CLog::Success(const char * source, const char * format, ...)
{
#ifdef ENABLE_CONSOLE_LOG
	if(log_level < 2)
		return;

	LOCK_LOG;
	va_list ap;
	va_start(ap, format);
	Time();
	Color(TGREEN);
	fputs("S ", stdout);
	if(*source)
	{
		Color(TWHITE);
		fputs(source, stdout);
		putchar(':');
		putchar(' ');
		Color(TGREEN);
	}

	vprintf(format, ap);
	putchar('\n');
	va_end(ap);
	Color(TNORMAL);
	UNLOCK_LOG;
#endif//ENABLE_CONSOLE_LOG
}

void CLog::Error(const char * source, const char * format, ...)
{
#ifdef ENABLE_CONSOLE_LOG
	if(log_level < 1)
		return;

	LOCK_LOG;
	va_list ap;
	va_start(ap, format);
	Time();
	Color(TRED);
	fputs("E ", stdout);
	if(*source)
	{
		Color(TWHITE);
		fputs(source, stdout);
		putchar(':');
		putchar(' ');
		Color(TRED);
	}

	vprintf(format, ap);
	putchar('\n');
	va_end(ap);
	Color(TNORMAL);
	UNLOCK_LOG;
#endif//ENABLE_CONSOLE_LOG
}

void CLog::Time()
{
	//UNIXTIME = time(NULL);
	tm * t = localtime(&UNIXTIME);
	printf("%02u:%02u ", t->tm_hour, t->tm_min);
	//printf("%02u:%02u ", g_localTime.tm_hour, g_localTime.tm_min);
}

void CLog::Line()
{
	LOCK_LOG;
	putchar('\n');
	UNLOCK_LOG;
}

void CLog::Debug(const char * source, const char * format, ...)
{
#ifdef ENABLE_CONSOLE_LOG
	if(log_level < 3)
		return;

	LOCK_LOG;
	va_list ap;
	va_start(ap, format);
	Time();
	Color(TBLUE);
	fputs("D ", stdout);
	if(*source)
	{
		Color(TWHITE);
		fputs(source, stdout);
		putchar(':');
		putchar(' ');
		Color(TBLUE);
	}

	vprintf(format, ap);
	putchar('\n');
	va_end(ap);
	Color(TNORMAL);
	UNLOCK_LOG;
#endif//ENABLE_CONSOLE_LOG
}

void CLog::LargeErrorMessage(uint32 Colour, ...)
{
	std::vector<char*> lines;
	char * pointer;
	va_list ap;
	va_start(ap, Colour);

	size_t i,j,k;
	pointer = va_arg(ap, char*);
	while( pointer != NULL )
	{
		lines.push_back( pointer );
		pointer = va_arg(ap, char*);
	}

	LOCK_LOG;

	if( Colour == LARGERRORMESSAGE_ERROR )
		Color(TRED);
	else
		Color(TYELLOW);

	printf("*********************************************************************\n");
	printf("*                        MAJOR ERROR/WARNING                        *\n");
	printf("*                        ===================                        *\n");

	for(std::vector<char*>::iterator itr = lines.begin(); itr != lines.end(); ++itr)
	{
		i = strlen(*itr);
		j = (i<=65) ? 65 - i : 0;

		printf("* %s", *itr);
		for( k = 0; k < j; ++k )
		{
			printf(" ");
		}

		printf(" *\n");
	}

	printf("*********************************************************************\n");

#ifdef WIN32
	std::string str = "MAJOR ERROR/WARNING:\n";
	for(std::vector<char*>::iterator itr = lines.begin(); itr != lines.end(); ++itr)
	{
		str += *itr;
		str += "\n";
	}

	MessageBox(0, str.c_str(), "Error", MB_OK);
#else
	printf("Sleeping for 5 seconds.\n");
	Sleep(5000);
#endif

	Color(TNORMAL);
	UNLOCK_LOG;
}