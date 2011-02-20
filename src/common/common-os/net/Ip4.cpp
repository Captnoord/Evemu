/**
 * @file net/Ip4.cpp
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

#include "net/Ip4.h"
#include "net/Utils.h"
#include "log/LogMgr.h"

using namespace common;
using namespace common::net;

/*************************************************************************/
/* common::net::Ip4                                                      */
/*************************************************************************/
const int Ip4::ADDRESS_FAMILY = AF_INET;

const in_addr Ip4::ADDRESS_ANY = { INADDR_ANY };
const in_addr Ip4::ADDRESS_BROADCAST = { INADDR_BROADCAST };

/*************************************************************************/
/* common::net::Ip4::SocketAddress                                       */
/*************************************************************************/
Ip4::SocketAddress::SocketAddress( const sockaddr_in& sockAddr )
: mSocketAddress( sockAddr )
{
    assert( Ip4::ADDRESS_FAMILY == mSocketAddress.sin_family );
}

Ip4::SocketAddress::SocketAddress( const in_addr& addr, uint16 port )
{
    mSocketAddress.sin_family = Ip4::ADDRESS_FAMILY;

    setAddress( addr );
    setPort( port );
}

const in_addr& Ip4::SocketAddress::address() const
{
    return mSocketAddress.sin_addr;
}

uint16 Ip4::SocketAddress::port() const
{
    return ::ntohs( mSocketAddress.sin_port );
}

std::string Ip4::SocketAddress::str() const
{
    std::string str;

    int code = ::sprintf( str, "%s:%u", ::inet_ntoa( address() ), port() );
    assert( 0 == code );

    return str;
}

void Ip4::SocketAddress::setAddress( const in_addr& addr )
{
    mSocketAddress.sin_addr = addr;
}

void Ip4::SocketAddress::setAddress( const char* ip )
{
    /* inet_addr may fail, but since the error code is
       always ambiguous, we don't care. */
    in_addr addr = { ::inet_addr( ip ) };

    setAddress( addr );
}

bool Ip4::SocketAddress::setHostname( const char* hostname )
{
    hostent* h = ::gethostbyname( hostname );
    if( NULL != h )
    {
        setAddress( *reinterpret_cast< in_addr* >( h->h_addr_list[0] ) );
        return true;
    }
    else
    {
        sLog.error( "network", "Failed to translate '%s' into address: error %d",
                    hostname, H_NET_ERRNO );
        return false;
    }
}

void Ip4::SocketAddress::setPort( uint16 port )
{
    mSocketAddress.sin_port = ::htons( port );
}
