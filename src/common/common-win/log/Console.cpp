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
    Author:     Bloody.Rabbit
*/

#include "CommonWin.h"

#include "win/ConsoleScreenBuffer.h"

using namespace common;
using namespace common::log;

/*************************************************************************/
/* common::log::Console                                                  */
/*************************************************************************/
/// A default color to print with.
const WORD COLOR_DEFAULT = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
/// A color to print sources with.
const WORD COLOR_SOURCE  = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;

/// An array of colors to print messages with.
const WORD TYPE_COLORS[ Message::TYPE_COUNT ] =
{
    ( FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE                        ), // TYPE_NOTICE
    ( FOREGROUND_RED                                      | FOREGROUND_INTENSITY ), // TYPE_ERROR
    ( FOREGROUND_RED | FOREGROUND_GREEN                   | FOREGROUND_INTENSITY ), // TYPE_WARNING
    (                  FOREGROUND_GREEN                   | FOREGROUND_INTENSITY ), // TYPE_SUCCESS
    (                  FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY ), // TYPE_DEBUG
    ( FOREGROUND_RED                    | FOREGROUND_BLUE | FOREGROUND_INTENSITY )  // TYPE_DUMP
};

stream::Error Console::Write( const Message& m )
{
    assert( 0 <= m.type() && m.type() < Message::TYPE_COUNT );

    static win::ConsoleScreenBuffer outputScreen = win::ConsoleScreenBuffer::DEFAULT_OUTPUT_SCREEN;
    const stdx::Tm& tm = m.time();

    // Time
    int code = ::printf( "%04d-%02d-%02d %02d:%02d:%02d ",
                         1900 + tm.year(), 1 + tm.mon(), tm.mday(),
                         tm.hour(), tm.min(), tm.sec() );
    if( 0 > code )
        return stream::ERROR_WRITE;

    // Prefix
    DWORD codeWin = outputScreen.SetTextAttributes( TYPE_COLORS[ m.type() ] );
    if( ERROR_SUCCESS != codeWin )
        return stream::ERROR_WRITE;

    code = ::printf( "%c ", m.prefix() );
    if( 0 > code )
        return stream::ERROR_WRITE;

    // Source
    codeWin = outputScreen.SetTextAttributes( COLOR_SOURCE );
    if( ERROR_SUCCESS != codeWin )
        return stream::ERROR_WRITE;

    code = ::printf( "%s:", m.source().c_str() );
    if( 0 > code )
        return stream::ERROR_WRITE;

    // Message
    codeWin = outputScreen.SetTextAttributes( TYPE_COLORS[ m.type() ] );
    if( ERROR_SUCCESS != codeWin )
        return stream::ERROR_WRITE;

    code = ::printf( " %s", m.message().c_str() );
    if( 0 > code )
        return stream::ERROR_WRITE;

    // Reset color
    codeWin = outputScreen.SetTextAttributes( COLOR_DEFAULT );
    if( ERROR_SUCCESS != codeWin )
        return stream::ERROR_WRITE;

    return stream::ERROR_OK;
}

stream::Error Console::Write( const Message* mp, size_t count, size_t* countWritten )
{
    for( size_t i = 0; i < count; ++i )
    {
        const stream::Error err = Write( mp[ i ] );
        if( stream::ERROR_OK != err )
        {
            if( NULL != countWritten )
                *countWritten = i;

            return err;
        }
    }

    if( NULL != countWritten )
        *countWritten = count;

    return stream::ERROR_OK;
}
