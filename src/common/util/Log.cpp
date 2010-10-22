/*
    ------------------------------------------------------------------------------------
    LICENSE:
    ------------------------------------------------------------------------------------
    This file is part of EVEmu: EVE Online Server Emulator
    Copyright 2006 - 2008 The EVEmu Team
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
    Author:     Captnoord
*/

#include "CommonPCH.h"

#include "time/TimeMgr.h"
#include "util/Log.h"
#include "util/String.h"

/*************************************************************************/
/* Util::Log                                                             */
/*************************************************************************/
#ifdef WIN32
const WORD Util::Log::COLOR_TABLE[ COLOR_COUNT ] =
{
    ( FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE                        ), // COLOR_DEFAULT
    ( 0                                                                          ), // COLOR_BLACK
    ( FOREGROUND_RED                                      | FOREGROUND_INTENSITY ), // COLOR_RED
    (                  FOREGROUND_GREEN                   | FOREGROUND_INTENSITY ), // COLOR_GREEN
    ( FOREGROUND_RED | FOREGROUND_GREEN                   | FOREGROUND_INTENSITY ), // COLOR_YELLOW
    (                                     FOREGROUND_BLUE | FOREGROUND_INTENSITY ), // COLOR_BLUE
    ( FOREGROUND_RED                    | FOREGROUND_BLUE | FOREGROUND_INTENSITY ), // COLOR_MAGENTA
    (                  FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY ), // COLOR_CYAN
    ( FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY )  // COLOR_WHITE
};
#else /* !WIN32 */
const char* const Util::Log::COLOR_TABLE[ COLOR_COUNT ] =
{
    "\033[" "00"    "m", // COLOR_DEFAULT
    "\033[" "30;22" "m", // COLOR_BLACK
    "\033[" "31;22" "m", // COLOR_RED
    "\033[" "32;22" "m", // COLOR_GREEN
    "\033[" "33;01" "m", // COLOR_YELLOW
    "\033[" "34;01" "m", // COLOR_BLUE
    "\033[" "35;22" "m", // COLOR_MAGENTA
    "\033[" "36;01" "m", // COLOR_CYAN
    "\033[" "37;01" "m"  // COLOR_WHITE
};
#endif /* !WIN32 */

Util::Log::Log()
#ifdef WIN32
: mLogfile( NULL ),
  mOutputScreen( Win::ConsoleScreenBuffer::DEFAULT_OUTPUT_SCREEN )
#else /* !WIN32 */
: mLogfile( NULL )
#endif /* !WIN32 */
{
    // open a default logfile
    if( OpenLogfile( EVEMU_ROOT_DIR"/log/evemu.log" ) )
    {
        Success( "Util::Log", "Opened default logfile '%s'",
                 EVEMU_ROOT_DIR"/log/evemu.log" );
    }
    else
    {
        Warning( "Util::Log", "Could not open default logfile '%s': %s",
                 EVEMU_ROOT_DIR"/log/evemu.log", strerror( errno ) );
    }
}

Util::Log::~Log()
{
    SetLogfile( NULL );
}

void Util::Log::Message( const char* source, const char* format, ... )
{
    va_list ap;
    va_start( ap, format );

    PrintMsgVa( COLOR_DEFAULT, 'L', source, format, ap );

    va_end( ap );
}

void Util::Log::Error( const char* source, const char* format, ... )
{
    va_list ap;
    va_start( ap, format );

    PrintMsgVa( COLOR_RED, 'E', source, format, ap );

    va_end( ap );
}

void Util::Log::Warning( const char* source, const char* format, ... )
{
    va_list ap;
    va_start( ap, format );

    PrintMsgVa( COLOR_YELLOW, 'W', source, format, ap );

    va_end( ap );
}

void Util::Log::Success( const char* source, const char* format, ... )
{
    va_list ap;
    va_start( ap, format );

    PrintMsgVa( COLOR_GREEN, 'S', source, format, ap );

    va_end( ap );
}

void Util::Log::Debug( const char* source, const char* format, ... )
{
#ifndef NDEBUG
    va_list ap;
    va_start( ap, format );

    PrintMsgVa( COLOR_CYAN, 'D', source, format, ap );

    va_end( ap );
#endif /* !NDEBUG */
}

void Util::Log::Dump( const char* source, const void* data, size_t length,
                      const char* format, ... )
{
#ifndef NDEBUG
    Mt::MutexLock l( mMutex );

    va_list ap;
    va_start( ap, format );

    PrintMsgVa( COLOR_MAGENTA, 'H', source, format, ap );

    va_end( ap );

    PrintDump( COLOR_MAGENTA, 'H', source, data, length );
#endif /* !NDEBUG */
}

bool Util::Log::OpenLogfile( const char* filename )
{
    FILE* file = fopen( filename, "a" );
    if( NULL == file )
        return false;

    return SetLogfile( file );
}

bool Util::Log::SetLogfile( FILE* file )
{
    Mt::MutexLock l( mMutex );

    if( hasLogfile() )
    {
        if( 0 != fclose( mLogfile ) )
            return false;
    }

    mLogfile = file;
    return true;
}

void Util::Log::PrintDump( Color color, char prefix, const char* source,
                           const void* data, size_t length )
{
    for( size_t i = 0; i < length; i += 0x10 )
        PrintDumpLine( color, prefix, source, data, length, i );
}

void Util::Log::PrintDumpLine( Color color, char prefix, const char* source,
                               const void* data, size_t length, size_t offset )
{
    char line[ 80 ];
    size_t lineLen = 0;

    char printable[ 0x10 ];

    for( size_t i = 0; i < 0x10; ++i )
    {
        if( 0x08 == i )
        {
            lineLen += snprintf( &line[ lineLen ],
                                 sizeof( line ) - lineLen,
                                 " -" );
        }

        if( ( offset + i ) < length )
        {
            const uint8 b = *( static_cast< const uint8* >( data ) + offset + i );

            lineLen += snprintf( &line[ lineLen ],
                                 sizeof( line ) - lineLen,
                                 " %02X", b );
            printable[ i ] = ( Util::String< char >::isPrintable( b )
                               ? static_cast< const char >( b )
                               : '.' );
        }
        else
        {
            lineLen += snprintf( &line[ lineLen ],
                                 sizeof( line ) - lineLen,
                                 "   " );
            printable[ i ] = ' ';
        }
    }

    PrintMsg( color, prefix, source, "%04X:%.*s | %.*s",
              offset, lineLen, line, sizeof( printable ), printable );
}

void Util::Log::PrintMsg( Color color, char prefix, const char* source,
                          const char* format, ... )
{
    va_list ap;
    va_start( ap, format );

    PrintMsgVa( color, prefix, source, format, ap );

    va_end( ap );
}

void Util::Log::PrintMsgVa( Color color, char prefix, const char* source,
                            const char* format, va_list ap )
{
    Mt::MutexLock l( mMutex );

    PrintTime();

    SetColor( color );
    Print( " %c ", prefix );

    if( source && *source )
    {
        SetColor( COLOR_WHITE );
        Print( "%s: ", source );

        SetColor( color );
    }

    PrintVa( format, ap );
    Print( "\n" );

    SetColor( COLOR_DEFAULT );
}

void Util::Log::PrintTime()
{
    Std::Tm t = sTimeMgr.nowTm();
    Time::Timeval tv = sTimeMgr.nowUnix();

    Print( "%04d-%02d-%02d %02d:%02d:%02d.%06d",
           1900 + t.year(), 1 + t.mon(), t.mday(),
           t.hour(), t.min(), t.sec(),
           tv.usec() );
}

void Util::Log::Print( const char* format, ... )
{
    va_list ap;
    va_start( ap, format );

    PrintVa( format, ap );

    va_end( ap );
}

void Util::Log::PrintVa( const char* format, va_list ap )
{
    Mt::MutexLock l( mMutex );

    if( hasLogfile() )
    {
        // this is a design flaw ( UNIX related )
        va_list ap2;
        va_copy( ap2, ap );

        vfprintf( mLogfile, format, ap2 );

#ifndef NDEBUG
        // flush immediately so the logfile is accurate if we crash
        fflush( mLogfile );
#endif /* !NDEBUG */

        va_end( ap2 );
    }

    vprintf( format, ap );
}

void Util::Log::SetColor( Color color )
{
    assert( 0 <= color && color < COLOR_COUNT );

    Mt::MutexLock l( mMutex );

#ifdef WIN32
    BOOL success = mOutputScreen.SetTextAttributes( COLOR_TABLE[ color ] );
    assert( TRUE == success );
#else /* !WIN32 */
    fputs( COLOR_TABLE[ color ], stdout );
#endif /* !WIN32 */
}
