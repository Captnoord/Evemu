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

#ifndef __NET__NET_UTILS_H__INCL__
#define __NET__NET_UTILS_H__INCL__

#ifdef WIN32
#   define MSG_NOSIGNAL 0
#else /* !WIN32 */
#   define INVALID_SOCKET ( -1 )
#   define SOCKET_ERROR   ( -1 )

#   if defined( FREE_BSD )
#       define MSG_NOSIGNAL 0
#   elif defined( APPLE )
#       define MSG_NOSIGNAL SO_NOSIGPIPE
#   endif

typedef int SOCKET;

#endif /* !WIN32 */

#ifndef ERRBUF_SIZE
#   define ERRBUF_SIZE 1024
#endif

uint32 ResolveIP( const char* hostname, char* errbuf = NULL );
//bool ParseAddress( const char* iAddress, int32* oIP, int16* oPort, char* errbuf = NULL );

#ifdef WIN32
class InitWinsock
{
public:
    InitWinsock();
    ~InitWinsock();
};
#endif

#endif /* !__NET__NET_UTILS_H__INCL__ */
