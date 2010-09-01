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

#ifndef __NET__SOCKET_H__INCL__
#define __NET__SOCKET_H__INCL__

#include "net/Utils.h"

namespace Net
{
    /**
     * @brief A network socket.
     *
     * @author Bloody.Rabbit
     */
    template< typename L3 >
    class Socket
    {
    public:
        /**
         * @brief The default constructor.
         */
        Socket()
        : mSocket( INVALID_SOCKET ),
          mSocketAddress( L3::SOCKET_ADDRESS_ANY )
        {
        }
        /**
         * @brief The primary constructor.
         *
         * @param[in] type Type of socket to create.
         * @param[in] prot Protocol to use.
         */
        Socket( int type, int prot )
        : mSocket( INVALID_SOCKET ),
          mSocketAddress( L3::SOCKET_ADDRESS_ANY )
        {
            int code = Create( type, prot );
            assert( 0 == code );
        }
        /**
         * @brief A destructor, closes the socket.
         */
        ~Socket()
        {
            int code = Assign( INVALID_SOCKET, L3::SOCKET_ADDRESS_ANY );
            assert( 0 == code );
        }

        /// Obtains validity of the socket.
        bool isValid() const { return INVALID_SOCKET != mSocket; }

        /// Obtains the socket address.
        const typename L3::SocketAddress& socketAddress() const { return mSocketAddress; }

        /**
         * @brief Creates a new socket.
         *
         * Any old socket is closed before creating a new one.
         *
         * @param[in] type Type of socket to create.
         * @param[in] prot Protocol to use.
         *
         * @return An error code.
         */
        int Create( int type, int prot )
        {
            SOCKET socket = ::socket( L3::ADDRESS_FAMILY, type, prot );
            if( INVALID_SOCKET == socket )
                return NET_ERRNO;

            int code = Assign( socket, L3::SOCKET_ADDRESS_ANY );
            if( 0 != code )
                return code;

            return 0;
        }
        /**
         * @brief Closes the current socket.
         *
         * @return An error code.
         */
        int Close()
        {
            /* There is a problem that we may be shutting down
               a listening socket, for which the shutdown will
               fail. Thus we ignore ENOTCONN error. */
            if( 0 != ::shutdown( mSocket, SHUT_WR ) )
                if( ENOTCONN != NET_ERRNO )
                    return NET_ERRNO;

            if( 0 != ::shutdown( mSocket, SHUT_RD ) )
                if( ENOTCONN != NET_ERRNO )
                    return NET_ERRNO;

            if( 0 != ::close( mSocket ) )
                return NET_ERRNO;

            mSocket = INVALID_SOCKET;
            mSocketAddress = L3::SOCKET_ADDRESS_ANY;
            return 0;
        }

        /**
         * @brief Binds the socket to an address.
         *
         * @param[in] socketAddress The address to bind to.
         *
         * @return An error code.
         */
        int Bind( const typename L3::SocketAddress& socketAddress )
        {
            if( 0 != ::bind( mSocket,
                             reinterpret_cast< const sockaddr* >( &socketAddress ),
                             sizeof( socketAddress ) ) )
                return NET_ERRNO;

            mSocketAddress = socketAddress;
            return 0;
        }

        /**
         * @brief Sets a socket option.
         *
         * @param[in] level Level of option.
         * @param[in] name  Name of the option.
         * @param[in] val   Value of the option.
         *
         * @return An error code.
         */
        template< typename T >
        int SetOption( int level, int name, const T& val )
        {
            if( 0 != ::setsockopt( mSocket, level, name, &val, sizeof( val ) ) )
                return NET_ERRNO;

            return 0;
        }

#   ifdef WIN32
        /**
         * @brief Sets advances options of socket.
         *
         * @param[in] cmd A command to perform.
         * @param[in] arg An argument for command.
         *
         * @return An error code.
         */
        int Ioctl( long cmd, unsigned long arg )
        {
            if( 0 != ::ioctlsocket( mSocket, cmd, &arg ) )
                return NET_ERRNO;

            return 0;
        }
#   else /* !WIN32 */
        /**
         * @brief Sets advanced options of socket.
         *
         * @param[in] cmd The command to perform.
         * @param[in] arg An argument for command.
         *
         * @return An error code.
         */
        int Fcntl( int cmd, long arg )
        {
            if( 0 != ::fcntl( mSocket, cmd, arg ) )
                return NET_ERRNO;

            return 0;
        }
#   endif /* !WIN32 */

    protected:
        /**
         * @brief Assigns a different socket.
         *
         * Any previous socket is closed.
         *
         * @param[in] socket        The new socket.
         * @param[in] socketAddress The address the @a socket is bound to.
         *
         * @return An error code.
         */
        int Assign( SOCKET socket, const typename L3::SocketAddress& socketAddress )
        {
            if( isValid() )
            {
                int code = Close();
                if( 0 != code )
                    return code;
            }

            mSocket = socket;
            mSocketAddress = socketAddress;
            return 0;
        }

        /// The socket itself.
        SOCKET mSocket;
        /// The socket address this socket is associated with.
        typename L3::SocketAddress mSocketAddress;
    };
}

#endif /* !__NET__SOCKET_H__INCL__ */
