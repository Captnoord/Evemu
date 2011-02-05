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

const Ip4::SocketAddress Ip4::SOCKET_ADDRESS_ANY =
    Ip4::GetSocketAddress( Ip4::ADDRESS_ANY,
                           Ip4::GetPort( 0 ) )
;

const Ip4::Address Ip4::ADDRESS_ANY = Ip4::GetAddress( INADDR_ANY );
const Ip4::Address Ip4::ADDRESS_BROADCAST = Ip4::GetAddress( INADDR_BROADCAST );

Ip4::SocketAddress Ip4::GetSocketAddress( const Ip4::Address& address,
                                          const Ip4::Port& port )
{
    SocketAddress socketAddress;
    socketAddress.sin_family = ADDRESS_FAMILY;
    socketAddress.sin_port = port;
    socketAddress.sin_addr = address;

    return socketAddress;
}

Ip4::Address Ip4::GetAddress( uint32 address )
{
    Address addr;
    addr.s_addr = address;

    return addr;
}

Ip4::Address Ip4::GetAddressByIP( const char* ip )
{
    /* inet_addr may fail, but since the error code is
       always ambiguous, we don't care. */
    return GetAddress( inet_addr( ip ) );
}

Ip4::Address Ip4::GetAddressByHostname( const char* hostname )
{
    hostent* h = gethostbyname( hostname );
    if( NULL == h )
    {
        sLog.error( "network", "Failed to translate '%s' into address: error %d",
                    hostname, H_NET_ERRNO );
        return ADDRESS_ANY;
    }

    return *reinterpret_cast< Ip4::Address* >( h->h_addr_list[0] );
}

Ip4::Address Ip4::GetAddressBySocketAddress( const Ip4::SocketAddress& socketAddress )
{
    return socketAddress.sin_addr;
}

Ip4::Port Ip4::GetPort( uint16 port )
{
    return htons( port );
}

Ip4::Port Ip4::GetPortBySocketAddress( const Ip4::SocketAddress& socketAddress )
{
    return socketAddress.sin_port;
}

std::string Ip4::PrintSocketAddress( const Ip4::SocketAddress& socketAddress )
{
    std::string str;
    sprintf( str, "%s:%s",
             PrintAddress( GetAddressBySocketAddress( socketAddress ) ).c_str(),
             PrintPort( GetPortBySocketAddress( socketAddress ) ).c_str() );

    return str;
}

std::string Ip4::PrintAddress( const Ip4::Address& address )
{
    return std::string( inet_ntoa( address ) );
}

std::string Ip4::PrintPort( const Ip4::Port& port )
{
    return util::String< char >::from< uint16 >( ntohs( port ) );
}
