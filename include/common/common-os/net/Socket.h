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

#ifndef __COMMON__NET__SOCKET_H__INCL__
#define __COMMON__NET__SOCKET_H__INCL__

#include "net/Utils.h"

namespace common
{
    /**
     * @brief Collection of utilities to access the network.
     */
    namespace net
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
            /// Convenient typedef for the socket address.
            typedef typename L3::SocketAddress Address;

            /**
             * @brief The default constructor.
             */
            Socket();
            /**
             * @brief The primary constructor.
             *
             * @param[in] type Type of socket to create.
             * @param[in] prot Protocol to use.
             */
            Socket( int type, int prot );
            /**
             * @brief A destructor, closes the socket.
             */
            ~Socket();

            /**
             * @brief Obtains validity of the socket.
             *
             * @retval true  The socket is valid.
             * @retval false The socket is invalid.
             */
            bool isValid() const { return INVALID_SOCKET != mSocket; }
            /**
             * @brief Obtains the socket address.
             *
             * @return The socket address.
             */
            const Address& address() const { return mAddress; }

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
            int Create( int type, int prot );
            /**
             * @brief Closes the current socket.
             *
             * @return An error code.
             */
            int Close();

            /**
             * @brief Binds the socket to an address.
             *
             * @param[in] socketAddress The address to bind to.
             *
             * @return An error code.
             */
            int Bind( const Address& address );

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
            int SetOption( int level, int name, const T& val );

#       ifdef WIN32
            /**
             * @brief Sets advanced options of socket.
             *
             * @param[in] cmd A command to perform.
             * @param[in] arg An argument for command.
             *
             * @return An error code.
             */
            int Ioctl( long cmd, unsigned long arg );
#       else /* !WIN32 */
            /**
             * @brief Sets advanced options of socket.
             *
             * @param[in] cmd The command to perform.
             * @param[in] arg An argument for command.
             *
             * @return An error code.
             */
            int Fcntl( int cmd, long arg );
#       endif /* !WIN32 */

        protected:
            /**
             * @brief Assigns a different socket.
             *
             * Any previous socket is closed.
             *
             * @param[in] socket        The new socket.
             * @param[in] socketAddress The address the <var>socket</var>
             *                          is bound to.
             *
             * @return An error code.
             */
            int Assign( SOCKET socket, const Address& address );

            /// The socket itself.
            SOCKET mSocket;
            /// The socket address this socket is associated with.
            Address mAddress;
        };

#   include "net/Socket.inl"
    }
}

#endif /* !__COMMON__NET__SOCKET_H__INCL__ */
