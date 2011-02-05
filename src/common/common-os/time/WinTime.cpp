/**
 * @file time/WinTime.cpp
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
 * @author Bloody.Rabbit
 */

#include "CommonStd.h"

#include "time/Const.h"
#include "time/Msec.h"
#include "time/Timeval.h"
#include "time/WinTime.h"

#ifndef WIN32
#   include "time/Timespec.h"
#endif /* !WIN32 */

using namespace common;
using namespace common::time;

/*************************************************************************/
/* common::time::WinTime                                                 */
/*************************************************************************/
const WinTime WinTime::USEC = 1 * WINTIME_PER_USEC;
const WinTime WinTime::MSEC = WinTime::USEC * USEC_PER_MSEC;
const WinTime WinTime::SEC = WinTime::MSEC * MSEC_PER_SEC;
const WinTime WinTime::MIN = WinTime::SEC * SEC_PER_MIN;
const WinTime WinTime::HOUR = WinTime::MIN * MIN_PER_HOUR;
const WinTime WinTime::DAY = WinTime::HOUR * HOUR_PER_DAY;
const WinTime WinTime::MONTH = WinTime::DAY * DAY_PER_MONTH;
const WinTime WinTime::YEAR = WinTime::MONTH * MONTH_PER_YEAR;

WinTime::WinTime( uint64 time )
: mTime( time )
{
}

WinTime::WinTime( const Msec& msec )
: mTime( WINTIME_PER_USEC * USEC_PER_MSEC * msec.count() )
{
}

#ifndef WIN32
WinTime::WinTime( const Timespec& ts )
: mTime( WINTIME_PER_USEC * USEC_PER_MSEC * MSEC_PER_SEC * ts.sec() + ts.nsec() / NSEC_PER_WINTIME )
{
}
#endif /* !WIN32 */

WinTime::WinTime( const Timeval& tv )
: mTime( WINTIME_PER_USEC * ( USEC_PER_MSEC * MSEC_PER_SEC * tv.sec() + tv.usec() ) )
{
}

WinTime::WinTime( const WinTime& oth )
: mTime( oth.count() )
{
}

bool WinTime::operator==( const WinTime& oth ) const
{
    return count() == oth.count();
}

bool WinTime::operator!=( const WinTime& oth ) const
{
    return count() != oth.count();
}

bool WinTime::operator<( const WinTime& oth ) const
{
    return count() < oth.count();
}

bool WinTime::operator<=( const WinTime& oth ) const
{
    return count() <= oth.count();
}

bool WinTime::operator>( const WinTime& oth ) const
{
    return count() > oth.count();
}

bool WinTime::operator>=( const WinTime& oth ) const
{
    return count() >= oth.count();
}

WinTime WinTime::operator+( const WinTime& oth ) const
{
    return count() + oth.count();
}

WinTime WinTime::operator-( const WinTime& oth ) const
{
    return count() - oth.count();
}

WinTime WinTime::operator*( size_t ratio ) const
{
    return count() * ratio;
}

WinTime WinTime::operator/( size_t ratio ) const
{
    return count() / ratio;
}

WinTime WinTime::operator%( size_t ratio ) const
{
    return count() % ratio;
}

WinTime& WinTime::operator=( const WinTime& oth )
{
    mTime = oth.count();
    return *this;
}

WinTime& WinTime::operator+=( const WinTime& oth )
{
    mTime += oth.count();
    return *this;
}

WinTime& WinTime::operator-=( const WinTime& oth )
{
    mTime -= oth.count();
    return *this;
}

WinTime& WinTime::operator*=( size_t ratio )
{
    mTime *= ratio;
    return *this;
}

WinTime& WinTime::operator/=( size_t ratio )
{
    mTime /= ratio;
    return *this;
}

WinTime& WinTime::operator%=( size_t ratio )
{
    mTime %= ratio;
    return *this;
}
