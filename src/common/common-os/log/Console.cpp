/**
 * @file log/Console.cpp
 *
 * This file is part of EVEmu: EVE Online Server Emulator.<br>
 * Copyright (C) 2006-2011 The EVEmu Team<br>
 * For the latest information visit <i>http://evemu.org</i>.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA, or go to
 * <i>http://www.gnu.org/copyleft/lesser.txt</i>.
 *
 * @author Bloody.Rabbit
 */

#include "CommonOs.h"

#include "log/Console.h"

using namespace common;
using namespace common::log;

/*************************************************************************/
/* common::log::Console                                                  */
/*************************************************************************/
#ifdef WIN32
const WORD Console::COLOR_DEFAULT = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
const WORD Console::COLOR_SOURCE  = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;

const WORD Console::TYPE_COLORS[ Message::TYPE_COUNT ] =
{
    ( FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE                        ), // TYPE_NOTICE
    ( FOREGROUND_RED                                      | FOREGROUND_INTENSITY ), // TYPE_ERROR
    ( FOREGROUND_RED | FOREGROUND_GREEN                   | FOREGROUND_INTENSITY ), // TYPE_WARNING
    (                  FOREGROUND_GREEN                   | FOREGROUND_INTENSITY ), // TYPE_SUCCESS
    (                  FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY ), // TYPE_DEBUG
    ( FOREGROUND_RED                    | FOREGROUND_BLUE | FOREGROUND_INTENSITY )  // TYPE_DUMP
};
#else /* !WIN32 */
const char* const Console::COLOR_DEFAULT = "\033[" "00"    "m";
const char* const Console::COLOR_SOURCE  = "\033[" "37;01" "m";

const char* const Console::TYPE_COLORS[ Message::TYPE_COUNT ] =
{
    "\033[" "00"    "m", // TYPE_NOTICE
    "\033[" "31;22" "m", // TYPE_ERROR
    "\033[" "33;01" "m", // TYPE_WARNING
    "\033[" "32;22" "m", // TYPE_SUCCESS
    "\033[" "36;01" "m", // TYPE_DEBUG
    "\033[" "35;22" "m"  // TYPE_DUMP
};
#endif /* !WIN32 */

Console::Console()
#ifdef WIN32
: mOutputScreen( win::ConsoleScreenBuffer::DEFAULT_OUTPUT_SCREEN )
#endif /* WIN32 */
{
}

stream::Error Console::Write( const Message& m )
{
    assert( 0 <= m.type() && m.type() < Message::TYPE_COUNT );

    const time::Tm& tm = m.time();

#ifdef WIN32
    // Time
    int code = ::printf( "%04d-%02d-%02d %02d:%02d:%02d ",
                         1900 + tm.year(), 1 + tm.mon(), tm.mday(),
                         tm.hour(), tm.min(), tm.sec() );
    if( 0 > code )
        return stream::ERROR_WRITE;

    // Prefix
    DWORD codeWin = mOutputScreen.SetTextAttributes( TYPE_COLORS[ m.type() ] );
    if( ERROR_SUCCESS != codeWin )
        return stream::ERROR_WRITE;

    code = ::printf( "%c ", m.prefix() );
    if( 0 > code )
        return stream::ERROR_WRITE;

    // Source
    codeWin = mOutputScreen.SetTextAttributes( COLOR_SOURCE );
    if( ERROR_SUCCESS != codeWin )
        return stream::ERROR_WRITE;

    code = ::printf( "%s:", m.source().c_str() );
    if( 0 > code )
        return stream::ERROR_WRITE;

    // Message
    codeWin = mOutputScreen.SetTextAttributes( TYPE_COLORS[ m.type() ] );
    if( ERROR_SUCCESS != codeWin )
        return stream::ERROR_WRITE;

    code = ::printf( " %s", m.message().c_str() );
    if( 0 > code )
        return stream::ERROR_WRITE;

    // Reset color
    codeWin = mOutputScreen.SetTextAttributes( COLOR_DEFAULT );
    if( ERROR_SUCCESS != codeWin )
        return stream::ERROR_WRITE;

    return stream::ERROR_OK;
#else /* !WIN32 */
    int code = ::printf( "%04d-%02d-%02d %02d:%02d:%02d %s%c %s%s:%s %s%s\n",
                         1900 + tm.year(), 1 + tm.mon(), tm.mday(),
                         tm.hour(), tm.min(), tm.sec(),
                         TYPE_COLORS[ m.type() ], m.prefix(),
                         COLOR_SOURCE, m.source().c_str(), TYPE_COLORS[ m.type() ],
                         m.message().c_str(), COLOR_DEFAULT );

    return 0 <= code ? stream::ERROR_OK : stream::ERROR_WRITE;
#endif /* !WIN32 */
}
