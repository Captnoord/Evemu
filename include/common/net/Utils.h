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
    Author:     Aim, Captnoord, Zhur, Bloody.Rabbit
*/

#ifndef __NET__UTILS_H__INCL__
#define __NET__UTILS_H__INCL__

/*
 * Define some common interface.
 */
#ifdef WIN32
#   define MSG_NOSIGNAL 0

#   define close closesocket

#   define SHUT_RD   SD_RECEIVE
#   define SHUT_WR   SD_SEND
#   define SHUT_RDWR SD_BOTH
#else /* !WIN32 */
typedef int SOCKET;

#   define INVALID_SOCKET ( (SOCKET)-1 )
#   define SOCKET_ERROR   ( (SOCKET)-1 )

#   if defined( FREE_BSD )
#       define MSG_NOSIGNAL 0
#   elif defined( APPLE )
#       define MSG_NOSIGNAL SO_NOSIGPIPE
#   endif
#endif /* !WIN32 */

namespace Net
{
    /*
     * Define NET_ERRNO and H_NET_ERRNO.
     */
#   ifdef WIN32
#       define   NET_ERRNO ( (int)WSAGetLastError() )
#       define H_NET_ERRNO NET_ERRNO
#   else /* !WIN32 */
#       define   NET_ERRNO ( (int)errno )
#       define H_NET_ERRNO ( (int)h_errno )
#   endif /* !WIN32 */

#   ifdef WIN32
    /**
     * @brief Winsock initialization class.
     *
     * Utility class to initialize Winsock. Winsock is
     * initialized as long as there is at least one
     * instance of this class.
     *
     * @author Zhur
     */
    class InitWinsock
    {
    public:
        /// A primary constructor.
        InitWinsock();
        /// A destructor.
        ~InitWinsock();
    };
#   endif
}

#endif /* !__NET__UTILS_H__INCL__ */
