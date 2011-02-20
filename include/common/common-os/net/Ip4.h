/**
 * @file net/Ip4.h
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

#ifndef __COMMON__NET__IP4_H__INCL__
#define __COMMON__NET__IP4_H__INCL__

namespace common
{
    namespace net
    {
        template< typename L3 >
        class Socket;

        template< typename L3 >
        class StreamSocket;

        /**
         * @brief Contains information about IPv4.
         *
         * @author Bloody.Rabbit
         */
        struct Ip4
        {
            /// Address family.
            static const int ADDRESS_FAMILY;

            /// Pseudo-address "any".
            static const in_addr ADDRESS_ANY;
            /// Pseudo-address "broadcast".
            static const in_addr ADDRESS_BROADCAST;

            /**
             * @brief A socket address.
             *
             * @author Bloody.Rabbit
             */
            class SocketAddress
            {
                friend class Socket< Ip4 >;
                friend class StreamSocket< Ip4 >;

            public:
                /**
                 * @brief Constructor with socket address.
                 *
                 * @param[in] addr Socket address.
                 */
                SocketAddress( const sockaddr_in& sockAddr );
                /**
                 * @brief Primary constructor.
                 *
                 * @param[in] addr Network address.
                 * @param[in] port Port.
                 */
                SocketAddress( const in_addr& addr = ADDRESS_ANY, uint16 port = 0 );

                /**
                 * @brief Obtains network address from the socket address.
                 *
                 * @return The network address.
                 */
                const in_addr& address() const;
                /**
                 * @brief Obtains port from the socket address.
                 *
                 * @return The port.
                 */
                uint16 port() const;

                /**
                 * @brief Creates string containing info about address and port.
                 *
                 * The string has form of "a.b.c.d:x".
                 *
                 * @return The string.
                 */
                std::string str() const;

                /**
                 * @brief Sets network address.
                 *
                 * @param[in] addr The network address.
                 */
                void setAddress( const in_addr& addr );
                /**
                 * @brief Sets network address.
                 *
                 * @param[in] ip The network address in form "a.b.c.d"
                 */
                void setAddress( const char* ip );
                /**
                 * @brief Sets network address by hostname.
                 *
                 * @param[in] hostname The hostname.
                 *
                 * @return true  Hostname resolving successfull.
                 * @return false Hostname resolving failed.
                 */
                bool setHostname( const char* hostname );
                /**
                 * @brief Sets port.
                 *
                 * @param[in] port The port.
                 */
                void setPort( uint16 port );

            protected:
                /// The socket address.
                sockaddr_in mSocketAddress;
            };
        };
    }
}

#endif /* !__COMMON__NET__IP4_H__INCL__ */
