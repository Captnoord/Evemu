/**
 * @file net/Utils.h
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
 * @author Aim, Captnoord, Zhur, Bloody.Rabbit
 */

#ifndef __COMMON__NET__UTILS_H__INCL__
#define __COMMON__NET__UTILS_H__INCL__

#ifndef HAVE_MSG_NOSIGNAL
#   ifdef HAVE_SO_NOSIGPIPE
#       define MSG_NOSIGNAL SO_NOSIGPIPE
#   else /* !HAVE_SO_NOSIGPIPE */
#       define MSG_NOSIGNAL 0
#   endif /* !HAVE_SO_NOSIGPIPE */
#endif /* !HAVE_MSG_NOSIGNAL */

/*
 * Define some common interface.
 */
#ifdef WIN32
#   define close closesocket

#   define SHUT_RD   SD_RECEIVE
#   define SHUT_WR   SD_SEND
#   define SHUT_RDWR SD_BOTH

#   define ENOTCONN WSAENOTCONN
#else /* !WIN32 */
typedef int SOCKET;

#   define INVALID_SOCKET static_cast< SOCKET >( -1 )
#   define SOCKET_ERROR   static_cast< SOCKET >( -1 )
#endif /* !WIN32 */

/*
 * Define NET_ERRNO and H_NET_ERRNO.
 */
#ifdef WIN32
#   define   NET_ERRNO static_cast< int >( WSAGetLastError() )
#   define H_NET_ERRNO NET_ERRNO
#else /* !WIN32 */
#   define   NET_ERRNO static_cast< int >(   errno )
#   define H_NET_ERRNO static_cast< int >( h_errno )
#endif /* !WIN32 */

#endif /* !__COMMON__NET__UTILS_H__INCL__ */
