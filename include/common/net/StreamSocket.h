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

#ifndef __NET__STREAM_SOCKET_H__INCL__
#define __NET__STREAM_SOCKET_H__INCL__

#include "net/Socket.h"
#include "utils/Buffer.h"

namespace Net
{
    /**
     * @brief A stream socket.
     *
     * @author Bloody.Rabbit
     */
    template< typename L3 >
    class StreamSocket
    : public Socket< L3 >
    {
        /// Typedef for our base due to readability.
        typedef Socket< L3 > _Base;

    public:
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
        : _Base( SOCK_STREAM, prot )
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
            return _Base::Create( SOCK_STREAM, prot );
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
            if( 0 != ::connect( _Base::mSocket,
                                reinterpret_cast< const sockaddr* >( &socketAddress ),
                                sizeof( socketAddress ) ) )
                return NET_ERRNO;

            _Base::mSocketAddress = socketAddress;
            return 0;
        }

        /**
         * @brief Receives data from socket.
         *
         * @param[out] into  Where to store received data.
         * @param[in]  flags Receive options.
         *
         * @return An error code (< 0) or number of received bytes (>= 0).
         */
        int Receive( Buffer& into, int flags = 0 )
        {
            int res = ::recv( _Base::mSocket, &into[0], into.size(), flags );
            if( 0 > res )
                return NET_ERRNO;

            return res;
        }
        /**
         * @brief Sends data to socket.
         *
         * @param[in] data  The data to send.
         * @param[in] flags Send options.
         *
         * @return An error code (< 0) or number of sent bytes (>= 0).
         */
        int Send( const Buffer& data, int flags = 0 )
        {
            int res = ::send( _Base::mSocket, &data[0], data.size(), flags );
            if( 0 > res )
                return NET_ERRNO;

            return res;
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
            if( 0 != ::listen( _Base::mSocket, backlog ) )
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
            int code;

            code = Bind( socketAddress );
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
            typename L3::SocketAddress socketAddress;
            socklen_t len = sizeof( socketAddress );

            SOCKET socket = ::accept( _Base::mSocket,
                                      reinterpret_cast< sockaddr* >( &socketAddress ),
                                      &len );
            if( INVALID_SOCKET == socket )
                return NET_ERRNO;

            int code = into.Assign( socket, socketAddress );
            if( 0 != code )
                return code;

            return 0;
        }
    };
}

#endif /* !__NET__STREAM_SOCKET_H__INCL__ */
