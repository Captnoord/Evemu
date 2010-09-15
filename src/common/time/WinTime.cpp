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

#include "time/Const.h"
#include "time/Msec.h"
#include "time/Timeval.h"
#include "time/WinTime.h"

#ifndef WIN32
#   include "time/Timespec.h"
#endif /* !WIN32 */

/*************************************************************************/
/* Time::WinTime                                                         */
/*************************************************************************/
const Time::WinTime Time::WinTime::USEC = 1 * WINTIME_PER_USEC;
const Time::WinTime Time::WinTime::MSEC = Time::WinTime::USEC * Time::USEC_PER_MSEC;
const Time::WinTime Time::WinTime::SEC = Time::WinTime::MSEC * Time::MSEC_PER_SEC;
const Time::WinTime Time::WinTime::MIN = Time::WinTime::SEC * Time::SEC_PER_MIN;
const Time::WinTime Time::WinTime::HOUR = Time::WinTime::MIN * Time::MIN_PER_HOUR;
const Time::WinTime Time::WinTime::DAY = Time::WinTime::HOUR * Time::HOUR_PER_DAY;
const Time::WinTime Time::WinTime::MONTH = Time::WinTime::DAY * Time::DAY_PER_MONTH;
const Time::WinTime Time::WinTime::YEAR = Time::WinTime::MONTH * Time::MONTH_PER_YEAR;

#ifdef WIN32
Time::WinTime Time::WinTime::now()
{
    FILETIME ft;
    ::GetSystemTimeAsFileTime( &ft );

    return WinTime( ( static_cast< uint64 >( ft.dwHighDateTime ) << 32 )
                    | static_cast< uint64 >( ft.dwLowDateTime ) );
}
#endif /* WIN32 */

Time::WinTime::WinTime( uint64 time )
: mTime( time )
{
}

Time::WinTime::WinTime( const Time::Msec& msec )
: mTime( WINTIME_PER_USEC * USEC_PER_MSEC * msec.count() )
{
}

#ifndef WIN32
Time::WinTime::WinTime( const Time::Timespec& ts )
: mTime( WINTIME_PER_USEC * USEC_PER_MSEC * MSEC_PER_SEC * ts.sec()
         + ts.nsec() / NSEC_PER_WINTIME )
{
}
#endif /* !WIN32 */

Time::WinTime::WinTime( const Time::Timeval& tv )
: mTime( WINTIME_PER_USEC * ( USEC_PER_MSEC * MSEC_PER_SEC * tv.sec()
                              + tv.usec() ) )
{
}

Time::WinTime::WinTime( const Time::WinTime& oth )
: mTime( oth.count() )
{
}

bool Time::WinTime::operator==( const Time::WinTime& oth ) const
{
    return count() == oth.count();
}

bool Time::WinTime::operator!=( const Time::WinTime& oth ) const
{
    return count() != oth.count();
}

bool Time::WinTime::operator<( const Time::WinTime& oth ) const
{
    return count() < oth.count();
}

bool Time::WinTime::operator<=( const Time::WinTime& oth ) const
{
    return count() <= oth.count();
}

bool Time::WinTime::operator>( const Time::WinTime& oth ) const
{
    return count() > oth.count();
}

bool Time::WinTime::operator>=( const Time::WinTime& oth ) const
{
    return count() >= oth.count();
}

Time::WinTime Time::WinTime::operator+( const Time::WinTime& oth ) const
{
    return count() + oth.count();
}

Time::WinTime Time::WinTime::operator-( const Time::WinTime& oth ) const
{
    return count() - oth.count();
}

Time::WinTime Time::WinTime::operator*( size_t ratio ) const
{
    return count() * ratio;
}

Time::WinTime Time::WinTime::operator/( size_t ratio ) const
{
    return count() / ratio;
}

Time::WinTime Time::WinTime::operator%( size_t ratio ) const
{
    return count() % ratio;
}

Time::WinTime& Time::WinTime::operator=( const Time::WinTime& oth )
{
    mTime = oth.count();
    return *this;
}

Time::WinTime& Time::WinTime::operator+=( const Time::WinTime& oth )
{
    mTime += oth.count();
    return *this;
}

Time::WinTime& Time::WinTime::operator-=( const Time::WinTime& oth )
{
    mTime -= oth.count();
    return *this;
}

Time::WinTime& Time::WinTime::operator*=( size_t ratio )
{
    mTime *= ratio;
    return *this;
}

Time::WinTime& Time::WinTime::operator/=( size_t ratio )
{
    mTime /= ratio;
    return *this;
}

Time::WinTime& Time::WinTime::operator%=( size_t ratio )
{
    mTime %= ratio;
    return *this;
}
