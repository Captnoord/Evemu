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

#include "CommonPCH.h"

#include "time/TimeConst.h"
#include "time/TimeTimeval.h"
#include "time/TimeWin.h"

void SetTimevalByNow( timeval& tv )
{
#ifdef WIN32
    Win32Time t;
    SetWin32TimeByNow( t );

    SetTimevalByWin32Time( tv, t );
#else /* !WIN32 */
    int code = ::gettimeofday( &tv, NULL );
    assert( 0 == code );
#endif /* !WIN32 */
}

void SetTimevalByWin32Time( timeval& tv, const Win32Time& t )
{
    tv.tv_sec = t / ( WIN32TIME_PER_USEC * USEC_PER_MSEC * MSEC_PER_SEC );
    tv.tv_usec = ( t % ( WIN32TIME_PER_USEC * USEC_PER_MSEC * MSEC_PER_SEC ) ) / WIN32TIME_PER_USEC;
}
