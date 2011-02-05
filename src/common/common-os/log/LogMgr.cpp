/**
 * @file log/LogMgr.cpp
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

#include "CommonStd.h"

#include "log/LogMgr.h"
#include "util/String.h"

using namespace common;
using namespace common::log;

/*************************************************************************/
/* common::log::LogMgr                                                   */
/*************************************************************************/
void LogMgr::notice( const char* source, const char* format, ... )
{
    va_list ap;
    va_start( ap, format );

    Write( Message( Message::TYPE_NOTICE, source, format, ap ) );

    va_end( ap );
}

void LogMgr::error( const char* source, const char* format, ... )
{
    va_list ap;
    va_start( ap, format );

    Write( Message( Message::TYPE_ERROR, source, format, ap ) );

    va_end( ap );
}

void LogMgr::warning( const char* source, const char* format, ... )
{
    va_list ap;
    va_start( ap, format );

    Write( Message( Message::TYPE_WARNING, source, format, ap ) );

    va_end( ap );
}

void LogMgr::success( const char* source, const char* format, ... )
{
    va_list ap;
    va_start( ap, format );

    Write( Message( Message::TYPE_SUCCESS, source, format, ap ) );

    va_end( ap );
}

void LogMgr::debug( const char* source, const char* format, ... )
{
#ifndef NDEBUG
    va_list ap;
    va_start( ap, format );

    Write( Message( Message::TYPE_DEBUG, source, format, ap ) );

    va_end( ap );
#endif /* !NDEBUG */
}

void LogMgr::dump( const char* source, const void* data, size_t length,
                        const char* format, ... )
{
#ifndef NDEBUG
    va_list ap;
    va_start( ap, format );

    Write( Message( Message::TYPE_DUMP, source, format, ap ) );

    va_end( ap );

    for( size_t i = 0; i < length; i += 0x10 )
        dumpLine( source, data, length, i );
#endif /* !NDEBUG */
}

void LogMgr::dumpLine( const char* source, const void* data, size_t length,
                            size_t offset )
{
#ifndef NDEBUG
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
            const uint8 b = *( static_cast< const uint8* >( data )
                               + offset + i );

            lineLen += snprintf( &line[ lineLen ],
                                 sizeof( line ) - lineLen,
                                 " %02X", b );
            printable[ i ] = ( util::String< char >::isPrintable( b )
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

    Write( Message( Message::TYPE_DUMP, source, "%04X:%.*s | %.*s",
                    offset, lineLen, line, sizeof( printable ), printable ) );
#endif /* !NDEBUG */
}
