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
    Author:        Zhur
*/

#ifndef __COMMON_OS_H__INCL__
#define __COMMON_OS_H__INCL__

/*************************************************************************/
/* common-std                                                            */
/*************************************************************************/
#include "CommonStd.h"

#include "log/Message.h"
#include "mt/Mutex.h"
#include "mt/Target.h"
#include "mt/ThreadMgr.h"
#include "std/Tm.h"
#include "time/Const.h"
#include "time/Msec.h"
#include "time/WinTime.h"
#include "util/Buffer.h"
#include "util/Singleton.h"
#include "util/String.h"

/*************************************************************************/
/* common-os                                                             */
/*************************************************************************/
#ifdef WIN32
// Define basic Windows configuration
#   define WIN32_LEAN_AND_MEAN
#   define _WIN32_WINNT 0x0500 // building for Win2k
#   define NOMINMAX

#   include <process.h>
#   include <windows.h>
#   include <winsock2.h>
#   include <ws2tcpip.h>
#else /* !WIN32 */
// Network
#   include <arpa/inet.h>
#   include <netdb.h>
#   include <netinet/in.h>
#   include <sys/socket.h>

// Files
#   include <dirent.h>
#   include <fcntl.h>
#   include <sys/stat.h>

// Threads
#   include <pthread.h>

// Time
#   include <sys/time.h>

// Miscellaneous
#   include <unistd.h>
#endif /* !WIN32 */

#endif /* !__NET__COMMON_H__INCL__ */
