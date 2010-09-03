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

#include "CommonPCH.h"

#include "net/Ip4.h"
#include "net/Utils.h"
#include "utils/Log.h"
#include "utils/StrConv.h"

/*************************************************************************/
/* Net::Ip4                                                              */
/*************************************************************************/
const int Net::Ip4::ADDRESS_FAMILY = AF_INET;

const Net::Ip4::SocketAddress Net::Ip4::SOCKET_ADDRESS_ANY =
    Net::Ip4::GetSocketAddress( Net::Ip4::ADDRESS_ANY,
                                Net::Ip4::GetPort( 0 ) )
;

const Net::Ip4::Address Net::Ip4::ADDRESS_ANY = Net::Ip4::GetAddress( INADDR_ANY );
const Net::Ip4::Address Net::Ip4::ADDRESS_BROADCAST = Net::Ip4::GetAddress( INADDR_BROADCAST );

Net::Ip4::SocketAddress Net::Ip4::GetSocketAddress( const Net::Ip4::Address& address,
                                                    const Net::Ip4::Port& port )
{
    SocketAddress socketAddress;
    socketAddress.sin_family = ADDRESS_FAMILY;
    socketAddress.sin_port = port;
    socketAddress.sin_addr = address;

    return socketAddress;
}

Net::Ip4::Address Net::Ip4::GetAddress( uint32 address )
{
    Address addr;
    addr.s_addr = address;

    return addr;
}

Net::Ip4::Address Net::Ip4::GetAddressByIP( const char* ip )
{
    /* inet_addr may fail, but since the error code is
       always ambiguous, we don't care. */
    return GetAddress( inet_addr( ip ) );
}

Net::Ip4::Address Net::Ip4::GetAddressByHostname( const char* hostname )
{
    hostent* h = gethostbyname( hostname );
    if( NULL == h )
    {
        sLog.Error( "Network", "Failed to translate '%s' into address: error %d",
                    hostname, H_NET_ERRNO );
        return ADDRESS_ANY;
    }

    return *reinterpret_cast< Net::Ip4::Address* >( h->h_addr_list[0] );
}

Net::Ip4::Address Net::Ip4::GetAddressBySocketAddress( const Net::Ip4::SocketAddress& socketAddress )
{
    return socketAddress.sin_addr;
}

Net::Ip4::Port Net::Ip4::GetPort( uint16 port )
{
    return htons( port );
}

Net::Ip4::Port Net::Ip4::GetPortBySocketAddress( const Net::Ip4::SocketAddress& socketAddress )
{
    return socketAddress.sin_port;
}

std::string Net::Ip4::PrintSocketAddress( const Net::Ip4::SocketAddress& socketAddress )
{
    std::string str;
    sprintf( str, "%s:%s",
             PrintAddress( GetAddressBySocketAddress( socketAddress ) ).c_str(),
             PrintPort( GetPortBySocketAddress( socketAddress ) ).c_str() );

    return str;
}

std::string Net::Ip4::PrintAddress( const Net::Ip4::Address& address )
{
    return std::string( inet_ntoa( address ) );
}

std::string Net::Ip4::PrintPort( const Net::Ip4::Port& port )
{
    return strFrom< uint16 >( ntohs( port ) );
}
