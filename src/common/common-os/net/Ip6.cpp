/**
 * @file net/Ip6.cpp
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

#include "net/Ip6.h"

using namespace common;
using namespace common::net;

/*************************************************************************/
/* common::net::Ip6                                                      */
/*************************************************************************/
const int Ip6::ADDRESS_FAMILY = AF_INET6;

const Ip6::SocketAddress Ip6::SOCKET_ADDRESS_ANY =
    Ip6::GetSocketAddress( Ip6::ADDRESS_ANY,
                           Ip6::GetPort( 0 ) )
;

const Ip6::Address Ip6::ADDRESS_ANY = ::in6addr_any;
const Ip6::Address Ip6::ADDRESS_LOOPBACK = ::in6addr_loopback;

Ip6::SocketAddress Ip6::GetSocketAddress( const Ip6::Address& address,
                                          const Ip6::Port& port,
                                          const Ip6::FlowInfo& fi,
                                          const Ip6::ScopeID& sid )
{
    Ip6::SocketAddress socketAddress;
    socketAddress.sin6_family = Ip6::ADDRESS_FAMILY;
    socketAddress.sin6_addr = address;
    socketAddress.sin6_port = port;
    socketAddress.sin6_flowinfo = fi;
    socketAddress.sin6_scope_id = sid;

    return socketAddress;
}

Ip6::Address Ip6::GetAddressBySocketAddress( const SocketAddress& socketAddress )
{
    return socketAddress.sin6_addr;
}

Ip6::Port Ip6::GetPort( uint16 port )
{
    return htons( port );
}

Ip6::Port Ip6::GetPortBySocketAddress( const SocketAddress& socketAddress )
{
    return socketAddress.sin6_port;
}
