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

#include "CommonPosix.h"

/*************************************************************************/
/* Log::Console                                                          */
/*************************************************************************/
/// A default color to print with.
const char* const COLOR_DEFAULT = "\033[" "00"    "m";
/// A color to print sources with.
const char* const COLOR_SOURCE  = "\033[" "37;01" "m";

/// An array of colors to print messages with.
const char* const TYPE_COLORS[ Log::Message::TYPE_COUNT ] =
{
    "\033[" "00"    "m", // TYPE_NOTICE
    "\033[" "31;22" "m", // TYPE_ERROR
    "\033[" "33;01" "m", // TYPE_WARNING
    "\033[" "32;22" "m", // TYPE_SUCCESS
    "\033[" "36;01" "m", // TYPE_DEBUG
    "\033[" "35;22" "m"  // TYPE_DUMP
};

Stream::Error Log::Console::Write( const Message& m )
{
    assert( 0 <= m.type() && m.type() < Message::TYPE_COUNT );

    const Std::Tm& tm = m.time();

    int code = ::printf( "%04d-%02d-%02d %02d:%02d:%02d %s%c %s%s:%s %s%s\n",
                         1900 + tm.year(), 1 + tm.mon(), tm.mday(),
                         tm.hour(), tm.min(), tm.sec(),
                         TYPE_COLORS[ m.type() ], m.prefix(),
                         COLOR_SOURCE, m.source().c_str(), TYPE_COLORS[ m.type() ],
                         m.message().c_str(), COLOR_DEFAULT );

    return 0 <= code ? Stream::ERROR_OK : Stream::ERROR_WRITE;
}

Stream::Error Log::Console::Write( const Message* mp, size_t count, size_t* countWritten )
{
    for( size_t i = 0; i < count; ++i )
    {
        const Stream::Error err = Write( mp[ i ] );
        if( Stream::ERROR_OK != err )
        {
            if( NULL != countWritten )
                *countWritten = i;

            return err;
        }
    }

    if( NULL != countWritten )
        *countWritten = count;

    return Stream::ERROR_OK;
}
