/**
 * @file net/Ip6.h
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

#ifndef __COMMON__NET__IP6_H__INCL__
#define __COMMON__NET__IP6_H__INCL__

namespace common
{
    namespace net
    {
#   ifdef WITH_IPV6
        /**
         * @brief Contains information about IPv6.
         *
         * @author Bloody.Rabbit
         */
        struct Ip6
        {
            /// Typedef for socket address.
            typedef sockaddr_in6 SocketAddress;
            /// Typedef for address.
            typedef in6_addr Address;
            /// Typedef for port.
            typedef uint16 Port;
            /// Typedef for flow info
            typedef uint32 FlowInfo;
            /// Typedef for scope ID
            typedef uint32 ScopeID;

            /// Address family.
            static const int ADDRESS_FAMILY;

            /// Socket pseudo-address "any".
            static const SocketAddress SOCKET_ADDRESS_ANY;

            /// Pseudo-address "any".
            static const Address ADDRESS_ANY;
            /// Pseudo-address "loopback".
            static const Address ADDRESS_LOOPBACK;

            /**
             * @brief Obtains a socket address.
             *
             * @param[in] address An address.
             * @param[in] port    A port.
             * @param[in] fi      Flow info.
             * @param[in] sid     A scope ID.
             *
             * @return The socket address.
             */
            static SocketAddress GetSocketAddress( const Address& address, const Port& port,
                                                   const FlowInfo& fi = 0, const ScopeID& sid = 0 );

            /**
             * @brief Obtains an address from a socket address.
             *
             * @param[in] socketAddress A socket address.
             *
             * @return The address.
             */
            static Address GetAddressBySocketAddress( const SocketAddress& socketAddress );

            /**
             * @brief Obtains a port.
             *
             * @param[in] port A port number.
             *
             * @return The port.
             */
            static Port GetPort( uint16 port );
            /**
             * @brief Obtains a port from a socket address.
             *
             * @param[in] socketAddress A socket address.
             *
             * @return The port.
             */
            static Port GetPortBySocketAddress( const SocketAddress& socketAddress );
        };
#   endif /* WITH_IPV6 */
    }
}

#endif /* !__COMMON__NET__IP6_H__INCL__ */
