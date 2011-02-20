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
        template< typename L3 >
        class Socket;

        template< typename L3 >
        class StreamSocket;

#   ifdef WITH_IPV6
        /**
         * @brief Contains information about IPv6.
         *
         * @author Bloody.Rabbit
         */
        struct Ip6
        {
            /// Address family.
            static const int ADDRESS_FAMILY;

            /// Pseudo-address "any".
            static const in6_addr ADDRESS_ANY;
            /// Pseudo-address "loopback".
            static const in6_addr ADDRESS_LOOPBACK;

            /**
             * @brief A socket address.
             *
             * @author Bloody.Rabbit
             */
            class SocketAddress
            {
                friend class Socket< Ip6 >;
                friend class StreamSocket< Ip6 >;

            public:
                /**
                 * @brief Constructor with socket address.
                 *
                 * @param[in] sockAddr Socket address.
                 */
                SocketAddress( const sockaddr_in6& sockAddr );
                /**
                 * @brief Primary constructor.
                 *
                 * @param[in] addr Network address.
                 * @param[in] port Port.
                 * @param[in] fi   Flow info.
                 * @param[in] sid  Scope ID.
                 */
                SocketAddress( const in6_addr& addr = ADDRESS_ANY, uint16 port = 0,
                               uint32 fi = 0, uint32 sid = 0 );

                /**
                 * @brief Obtains network address.
                 *
                 * @return The network address.
                 */
                const in6_addr& address() const;
                /**
                 * @brief Obtains port.
                 *
                 * @return The port.
                 */
                uint16 port() const;
                /**
                 * @brief Obtains flow info.
                 *
                 * @return The flow info.
                 */
                uint32 flowInfo() const;
                /**
                 * @brief Obtains scope ID.
                 *
                 * @return The scope ID.
                 */
                uint32 scopeId() const;

                /**
                 * @brief Sets network address.
                 *
                 * @param[in] addr Network address.
                 */
                void setAddress( const in6_addr& addr );
                /**
                 * @brief Sets port.
                 *
                 * @param[in] port Port.
                 */
                void setPort( uint16 port );
                /**
                 * @brief Sets flow info.
                 *
                 * @param[in] flowInfo Flow info.
                 */
                void setFlowInfo( uint32 flowInfo );
                /**
                 * @brief Sets scope ID.
                 *
                 * @param[in] scopeId Scope ID.
                 */
                void setScopeId( uint32 scopeId );

            protected:
                /// The socket address.
                sockaddr_in6 mSocketAddress;
            };
        };
#   endif /* WITH_IPV6 */
    }
}

#endif /* !__COMMON__NET__IP6_H__INCL__ */
