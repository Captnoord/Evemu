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
    Author:        Bloody.Rabbit
*/

#include "CommonOs.h"

#include "time/TimeMgr.h"

using namespace common;
using namespace common::log;

/*************************************************************************/
/* common::log::Message                                                  */
/*************************************************************************/
const char Message::TYPE_PREFIXES[ TYPE_COUNT ] =
{
    'N', // TYPE_NOTICE
    'E', // TYPE_ERROR
    'W', // TYPE_WARNING
    'S', // TYPE_SUCCESS
    'D', // TYPE_DEBUG
    'H'  // TYPE_DUMP
};

Message::Message( Type type, const char* source,
                       const char* format, ... )
: mType( type ),
  mTime( sTimeMgr.nowTm() ),
  mSource( source )
{
    va_list ap;
    va_start( ap, format );

    int code = vsprintf( mMessage, format, ap );
    assert( 0 <= code );

    va_end( ap );
}

Message::Message( Type type, const char* source,
                       const char* format, va_list ap )
: mType( type ),
  mTime( sTimeMgr.nowTm() ),
  mSource( source )
{
    int code = vsprintf( mMessage, format, ap );
    assert( 0 <= code );
}

char Message::prefix() const
{
    assert( 0 <= type() && type() < TYPE_COUNT );
    return TYPE_PREFIXES[ type() ];
}
