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

#ifndef __THREAD__THREAD_UTILS_H__INCL__
#define __THREAD__THREAD_UTILS_H__INCL__

// Return thread macro's
// URL: http://msdn.microsoft.com/en-us/library/hw264s73(VS.80).aspx
// Important Quote: "_endthread and _endthreadex cause C++ destructors pending in the thread not to be called."
// Result: mem leaks under windows
#ifdef WIN32
#   define THREAD_RETURN( x ) ( x )
typedef void thread_return_t;
#else /* !WIN32 */
#   define THREAD_RETURN( x ) return ( x )
typedef void* thread_return_t;
#endif /* !WIN32 */

#endif /* !__THREAD__THREAD_UTILS_H__INCL__ */
