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
            /// Typedef for a stream element due to reference ambiguity.
            typedef void Element;

            /**
             * @brief A default constructor.
             */
            StreamSocket()
            {
            }
            /**
             * @brief A primary constructor.
             *
             * @param[in] prot A protocol to use.
             */
            StreamSocket( int prot )
            : Base( SOCK_STREAM, prot )
            {
            }

            /**
             * @brief Creates a new socket.
             *
             * @param[in] prot A protocol to use.
             *
             * @return An error code.
             */
            int Create( int prot )
            {
                return Base::Create( SOCK_STREAM, prot );
            }

            /**
             * @brief Connects to specified address.
             *
             * @param[in] socketAddress The address to connect.
             *
             * @return An error code.
             */
            int Connect( const typename L3::SocketAddress& socketAddress )
            {
                if( 0 != ::connect( Base::mSocket,
                                    reinterpret_cast< const sockaddr* >( &socketAddress ),
                                    sizeof( socketAddress ) ) )
                    return NET_ERRNO;

                Base::mSocketAddress = socketAddress;
                return 0;
            }

            /**
             * @brief Receives data from socket.
             *
             * @param[out] data      Where to store received data.
             * @param[out] bytesRead Where to store the number
             *                       of received bytes.
             *
             * @return An error code.
             */
            stream::Error Read( util::Data& data, size_t* bytesRead = NULL )
            {
                int code = ::recv( Base::mSocket, &data[0], data.size(), 0 );
                if( 0 < code )
                {
                    if( NULL != bytesRead )
                        *bytesRead = code;

                    return stream::ERROR_OK;
                }
                else if( 0 == code )
                    return stream::ERROR_EOS;
                else if( EWOULDBLOCK == NET_ERRNO )
                    return stream::ERROR_TRYLATER;
                else
                    return stream::ERROR_READ;
            }
            /**
             * @brief Sends data to socket.
             *
             * @param[in]  data         The data to send.
             * @param[out] bytesWritten Where to store the number
             *                          of bytes written.
             *
             * @return An error code.
             */
            stream::Error Write( const util::Data& data, size_t* bytesWritten = NULL )
            {
                int code = ::send( Base::mSocket, &data[0], data.size(), 0 );
                if( 0 <= code )
                {
                    if( NULL != bytesWritten )
                        *bytesWritten = code;

                    return stream::ERROR_OK;
                }
                else if( EWOULDBLOCK == NET_ERRNO )
                    return stream::ERROR_TRYLATER;
                else
                    return stream::ERROR_WRITE;
            }

            /**
             * @brief Starts listening on socket.
             *
             * @param[in] backlog Limit for number of connections.
             *
             * @return An error code.
             */
            int Listen( int backlog = SOMAXCONN )
            {
                if( 0 != ::listen( Base::mSocket, backlog ) )
                    return NET_ERRNO;

                return 0;
            }
            /**
             * @brief Starts listening on socket, binding to an address.
             *
             * @param[in] socketAddress The address to bind to.
             * @param[in] backlog       Limit for number of connections.
             *
             * @return An error code.
             */
            int Listen( const typename L3::SocketAddress& socketAddress, int backlog = SOMAXCONN )
            {
                int code = Bind( socketAddress );
                if( 0 != code )
                    return code;

                code = Listen( backlog );
                if( 0 != code )
                    return code;

                return 0;
            }
            /**
             * @brief Accepts a connection.
             *
             * @param[out] into Socket to keep the new connection.
             *
             * @return An error code.
             */
            int Accept( StreamSocket< L3 >& into )
            {
                // Close the socket first
                int code = into.Close();
                if( 0 != code )
                    return code;

                // Obtain the new socket
                typename L3::SocketAddress socketAddress;
                socklen_t len = sizeof( socketAddress );

                SOCKET socket = ::accept( Base::mSocket,
                                          reinterpret_cast< sockaddr* >( &socketAddress ),
                                          &len );
                if( INVALID_SOCKET == socket )
                    return NET_ERRNO;

                // Assign it
                code = into.Assign( socket, socketAddress );
                if( 0 != code )
                    return code;

                return 0;
            }
        };
    }
}

#endif /* !__COMMON__NET__STREAM_SOCKET_H__INCL__ */
