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

#include "net/Ip6.h"

/*************************************************************************/
/* Net::Ip6                                                              */
/*************************************************************************/
const int Net::Ip6::ADDRESS_FAMILY = AF_INET6;

const Net::Ip6::SocketAddress Net::Ip6::SOCKET_ADDRESS_ANY =
    Net::Ip6::GetSocketAddress( Net::Ip6::ADDRESS_ANY,
                                Net::Ip6::GetPort( 0 ) )
;

const Net::Ip6::Address Net::Ip6::ADDRESS_ANY = ::in6addr_any;
const Net::Ip6::Address Net::Ip6::ADDRESS_LOOPBACK = ::in6addr_loopback;

Net::Ip6::SocketAddress Net::Ip6::GetSocketAddress( const Net::Ip6::Address& address,
                                                    const Net::Ip6::Port& port,
                                                    const Net::Ip6::FlowInfo& fi,
                                                    const Net::Ip6::ScopeID& sid )
{
    Net::Ip6::SocketAddress socketAddress;
    socketAddress.sin6_family = Net::Ip6::ADDRESS_FAMILY;
    socketAddress.sin6_addr = address;
    socketAddress.sin6_port = port;
    socketAddress.sin6_flowinfo = fi;
    socketAddress.sin6_scope_id = sid;

    return socketAddress;
}

Net::Ip6::Address Net::Ip6::GetAddressBySocketAddress( const SocketAddress& socketAddress )
{
    return socketAddress.sin6_addr;
}

Net::Ip6::Port Net::Ip6::GetPort( uint16 port )
{
    return htons( port );
}

Net::Ip6::Port Net::Ip6::GetPortBySocketAddress( const SocketAddress& socketAddress )
{
    return socketAddress.sin6_port;
}
