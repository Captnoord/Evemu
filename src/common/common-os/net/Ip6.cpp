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

#ifdef WITH_IPV6
/*************************************************************************/
/* common::net::Ip6                                                      */
/*************************************************************************/
const int Ip6::ADDRESS_FAMILY = AF_INET6;

const in6_addr Ip6::ADDRESS_ANY = ::in6addr_any;
const in6_addr Ip6::ADDRESS_LOOPBACK = ::in6addr_loopback;

/*************************************************************************/
/* common::net::Ip6::SocketAddress                                       */
/*************************************************************************/
Ip6::SocketAddress::SocketAddress( const sockaddr_in6& sockAddr )
: mSocketAddress( sockAddr )
{
    assert( Ip6::ADDRESS_FAMILY == mSocketAddress.sin6_family );
}

Ip6::SocketAddress::SocketAddress( const in6_addr& addr, uint16 port,
                                   uint32 flowInfo, uint32 scopeId )
{
    mSocketAddress.sin6_family = Ip6::ADDRESS_FAMILY;

    setAddress( addr );
    setPort( port );
    setFlowInfo( flowInfo );
    setScopeId( scopeId );
}

const in6_addr& Ip6::SocketAddress::address() const
{
    return mSocketAddress.sin6_addr;
}

uint16 Ip6::SocketAddress::port() const
{
    return ::ntohs( mSocketAddress.sin6_port );
}

uint32 Ip6::SocketAddress::flowInfo() const
{
    return mSocketAddress.sin6_flowinfo;
}

uint32 Ip6::SocketAddress::scopeId() const
{
    return mSocketAddress.sin6_scope_id;
}

void Ip6::SocketAddress::setAddress( const in6_addr& addr )
{
    mSocketAddress.sin6_addr = addr;
}

void Ip6::SocketAddress::setPort( uint16 port )
{
    mSocketAddress.sin6_port = ::htons( port );
}

void Ip6::SocketAddress::setFlowInfo( uint32 flowInfo )
{
    mSocketAddress.sin6_flowinfo = flowInfo;
}

void Ip6::SocketAddress::setScopeId( uint32 scopeId )
{
    mSocketAddress.sin6_scope_id = scopeId;
}
#endif /* WITH_IPV6 */
