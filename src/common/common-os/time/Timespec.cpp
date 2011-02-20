/**
 * @file time/Timespec.cpp
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

#include "CommonOs.h"

#include "time/Const.h"
#include "time/Msec.h"
#include "time/Timespec.h"
#include "time/Timeval.h"
#include "time/WinTime.h"

using namespace common;
using namespace common::time;

#ifdef HAVE_TIMESPEC
/*************************************************************************/
/* common::time::Timespec                                                */
/*************************************************************************/
const Timespec Timespec::NSEC = Timespec( 0, 1 );
const Timespec Timespec::USEC = Timespec::NSEC * NSEC_PER_USEC;
const Timespec Timespec::MSEC = Timespec::USEC * USEC_PER_MSEC;
const Timespec Timespec::SEC = Timespec::MSEC * MSEC_PER_SEC;
const Timespec Timespec::MIN = Timespec::SEC * SEC_PER_MIN;
const Timespec Timespec::HOUR = Timespec::MIN * MIN_PER_HOUR;
const Timespec Timespec::DAY = Timespec::HOUR * HOUR_PER_DAY;
const Timespec Timespec::MONTH = Timespec::DAY * DAY_PER_MONTH;
const Timespec Timespec::YEAR = Timespec::MONTH * MONTH_PER_YEAR;

Timespec::Timespec( time_t sec, long nsec )
{
    mTimespec.tv_sec = sec;
    mTimespec.tv_nsec = nsec;
}

Timespec::Timespec( const timespec& ts )
: mTimespec( ts )
{
}

Timespec::Timespec( const Msec& msec )
{
    mTimespec.tv_sec = msec.count() / MSEC_PER_SEC;
    mTimespec.tv_nsec = NSEC_PER_USEC * USEC_PER_MSEC * ( msec.count() % MSEC_PER_SEC );
}

Timespec::Timespec( const Timespec& oth )
: mTimespec( oth.ts() )
{
}

Timespec::Timespec( const Timeval& tv )
{
    mTimespec.tv_sec = tv.sec();
    mTimespec.tv_nsec = NSEC_PER_USEC * tv.usec();
}

Timespec::Timespec( const WinTime& time )
{
    mTimespec.tv_sec = time.count() / ( WINTIME_PER_USEC * USEC_PER_MSEC * MSEC_PER_SEC );
    mTimespec.tv_nsec = NSEC_PER_WINTIME * ( time.count() % ( WINTIME_PER_USEC * USEC_PER_MSEC * MSEC_PER_SEC ) );
}

bool Timespec::operator==( const Timespec& oth ) const
{
    if( sec() != oth.sec() )
        return false;
    else if ( nsec() != oth.nsec() )
        return false;
    else
        return true;
}

bool Timespec::operator!=( const Timespec& oth ) const
{
    if( sec() != oth.sec() )
        return true;
    else if( nsec() != oth.nsec() )
        return true;
    else
        return false;
}

bool Timespec::operator<( const Timespec& oth ) const
{
    if( sec() < oth.sec() )
        return true;
    else if( sec() > oth.sec() )
        return false;
    else if( nsec() < oth.nsec() )
        return true;
    else
        return false;
}

bool Timespec::operator<=( const Timespec& oth ) const
{
    if( sec() < oth.sec() )
        return true;
    else if( sec() > oth.sec() )
        return false;
    else if( nsec() <= oth.nsec() )
        return true;
    else
        return false;
}

bool Timespec::operator>( const Timespec& oth ) const
{
    if( sec() > oth.sec() )
        return true;
    else if( sec() < oth.sec() )
        return false;
    else if( nsec() > oth.nsec() )
        return true;
    else
        return false;
}

bool Timespec::operator>=( const Timespec& oth ) const
{
    if( sec() > oth.sec() )
        return true;
    else if( sec() < oth.sec() )
        return false;
    else if( nsec() >= oth.nsec() )
        return true;
    else
        return false;
}

Timespec Timespec::operator+( const Timespec& oth ) const
{
    return Timespec( ( sec() + oth.sec() )
                     + ( nsec() + oth.nsec() ) / ( NSEC_PER_USEC * USEC_PER_MSEC * MSEC_PER_SEC ),
                     ( nsec() + oth.nsec() ) % ( NSEC_PER_USEC * USEC_PER_MSEC * MSEC_PER_SEC ) );
}

Timespec Timespec::operator-( const Timespec& oth ) const
{
    const unsigned long long k = ( nsec() < oth.nsec()
                                   ? 1 + ( oth.nsec() - nsec() - 1 ) / ( NSEC_PER_USEC * USEC_PER_MSEC * MSEC_PER_SEC )
                                   : 0 );

    return Timespec( sec() - oth.sec() - k,
                     NSEC_PER_USEC * USEC_PER_MSEC * MSEC_PER_SEC * k + nsec() - oth.nsec() );
}

Timespec Timespec::operator*( unsigned long long ratio ) const
{
    return Timespec( ( sec() * ratio )
                     + ( nsec() * ratio ) / ( NSEC_PER_USEC * USEC_PER_MSEC * MSEC_PER_SEC ),
                     ( nsec() * ratio ) % ( NSEC_PER_USEC * USEC_PER_MSEC * MSEC_PER_SEC ) );
}

Timespec Timespec::operator/( unsigned long long ratio ) const
{
    return Timespec( ( sec() / ratio ),
                     ( NSEC_PER_USEC * USEC_PER_MSEC * MSEC_PER_SEC * ( sec() % ratio )
                       + nsec() ) / ratio );
}

Timespec Timespec::operator%( unsigned long long ratio ) const
{
    const unsigned long long mod = ( NSEC_PER_USEC * USEC_PER_MSEC * MSEC_PER_SEC * ( sec() % ratio )
                                     + nsec() ) % ratio;

    return Timespec( mod / ( NSEC_PER_USEC * USEC_PER_MSEC * MSEC_PER_SEC ),
                     mod % ( NSEC_PER_USEC * USEC_PER_MSEC * MSEC_PER_SEC ) );
}

Timespec& Timespec::operator=( const Timespec& oth )
{
    mTimespec = oth.ts();
    return *this;
}

Timespec& Timespec::operator+=( const Timespec& oth )
{
    return ( *this = *this + oth );
}

Timespec& Timespec::operator-=( const Timespec& oth )
{
    return ( *this = *this - oth );
}

Timespec& Timespec::operator*=( unsigned long long ratio )
{
    return ( *this = *this * ratio );
}

Timespec& Timespec::operator/=( unsigned long long ratio )
{
    return ( *this = *this / ratio );
}

Timespec& Timespec::operator%=( unsigned long long ratio )
{
    return ( *this = *this % ratio );
}
#endif /* HAVE_TIMESPEC */
