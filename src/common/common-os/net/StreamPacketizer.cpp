/**
 * @file net/StreamPacketizer.cpp
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
 * @author Zhur
 */

#include "CommonOs.h"

#include "net/StreamPacketizer.h"

using namespace common;
using namespace common::net;

/*************************************************************************/
/* common::net::StreamPacketizer                                         */
/*************************************************************************/
StreamPacketizer::~StreamPacketizer()
{
    ClearBuffers();
}

void StreamPacketizer::InputData( const util::Data& data )
{
    mBuffer.Append( data.begin< uint8 >(), data.end< uint8 >() );
}

void StreamPacketizer::Process()
{
    util::Buffer::ConstIterator< uint8 > cur, end;
    cur = mBuffer.begin< uint8 >();
    end = mBuffer.end< uint8 >();
    while( true )
    {
        if( sizeof( uint32 ) > ( end - cur ) )
            break;

        const util::Buffer::ConstIterator< uint32 > len = cur.as< uint32 >();
        const util::Buffer::ConstIterator< uint8 > start = ( len + 1 ).as< uint8 >();

        if( *len > (uint32)( end - start ) )
            break;

        mPackets.push( new util::Buffer( start, start + *len ) );
        cur = ( start + *len );
    }

    if( cur != mBuffer.begin< uint8 >() )
        mBuffer.Assign( cur, end );
}

util::Buffer* StreamPacketizer::PopPacket()
{
    util::Buffer* ret = NULL;
    if( !mPackets.empty() )
    {
        ret = mPackets.front();
        mPackets.pop();
    }

    return ret;
}

void StreamPacketizer::ClearBuffers()
{
    util::Buffer* buf;
    while( ( buf = PopPacket() ) )
        util::safeDelete( buf );
}
