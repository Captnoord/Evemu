/**
 * @file net/StreamSocket.h
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

#ifndef __COMMON__NET__STREAM_SOCKET_H__INCL__
#define __COMMON__NET__STREAM_SOCKET_H__INCL__

#include "net/Socket.h"

namespace common
{
    namespace net
    {
        /**
         * @brief A stream socket.
         *
         * @author Bloody.Rabbit
         */
        template< typename L3 >
        class StreamSocket
        : public Socket< L3 >,
          public stream::Input< void >,
          public stream::Output< void >
        {
            /// Typedef for our base due to readability.
            typedef Socket< L3 > Base;

        public:
            /// Conventient typedef for the socket address.
            typedef typename Base::Address Address;

            /// Typedef for a stream element due to reference ambiguity.
            typedef void Element;

            /**
             * @brief A default constructor.
             */
            StreamSocket();
            /**
             * @brief A primary constructor.
             *
             * @param[in] prot A protocol to use.
             */
            StreamSocket( int prot );

            /**
             * @brief Creates a new socket.
             *
             * @param[in] prot A protocol to use.
             *
             * @return An error code.
             */
            int Create( int prot );

            /**
             * @brief Connects to specified address.
             *
             * @param[in] address The address to connect.
             *
             * @return An error code.
             */
            int Connect( const Address& address );

            /**
             * @brief Receives data from socket.
             *
             * @param[out] data      Where to store received data.
             * @param[out] bytesRead Where to store the number
             *                       of received bytes.
             *
             * @return An error code.
             */
            stream::Error Read( util::Data& data, size_t* bytesRead = NULL );
            /**
             * @brief Sends data to socket.
             *
             * @param[in]  data         The data to send.
             * @param[out] bytesWritten Where to store the number
             *                          of bytes written.
             *
             * @return An error code.
             */
            stream::Error Write( const util::Data& data, size_t* bytesWritten = NULL );

            /**
             * @brief Starts listening on socket.
             *
             * @param[in] backlog Limit for number of connections.
             *
             * @return An error code.
             */
            int Listen( int backlog = SOMAXCONN );
            /**
             * @brief Starts listening on socket, binding to an address.
             *
             * @param[in] address The address to bind to.
             * @param[in] backlog Limit for number of connections.
             *
             * @return An error code.
             */
            int Listen( const Address& address, int backlog = SOMAXCONN );
            /**
             * @brief Accepts a connection.
             *
             * @param[out] into Socket to keep the new connection.
             *
             * @return An error code.
             */
            int Accept( StreamSocket& into );
        };

#   include "net/StreamSocket.inl"
    }
}

#endif /* !__COMMON__NET__STREAM_SOCKET_H__INCL__ */
