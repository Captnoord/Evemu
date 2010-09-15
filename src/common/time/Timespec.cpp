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
#include "time/Timespec.h"
#include "time/Timeval.h"
#include "time/WinTime.h"

#ifndef WIN32
/*************************************************************************/
/* Time::Timespec                                                        */
/*************************************************************************/
const Time::Timespec Time::Timespec::NSEC = Time::Timespec( 0, 1 );
const Time::Timespec Time::Timespec::USEC = Time::Timespec::NSEC * Time::NSEC_PER_USEC;
const Time::Timespec Time::Timespec::MSEC = Time::Timespec::USEC * Time::USEC_PER_MSEC;
const Time::Timespec Time::Timespec::SEC = Time::Timespec::MSEC * Time::MSEC_PER_SEC;
const Time::Timespec Time::Timespec::MIN = Time::Timespec::SEC * Time::SEC_PER_MIN;
const Time::Timespec Time::Timespec::HOUR = Time::Timespec::MIN * Time::MIN_PER_HOUR;
const Time::Timespec Time::Timespec::DAY = Time::Timespec::HOUR * Time::HOUR_PER_DAY;
const Time::Timespec Time::Timespec::MONTH = Time::Timespec::DAY * Time::DAY_PER_MONTH;
const Time::Timespec Time::Timespec::YEAR = Time::Timespec::MONTH * Time::MONTH_PER_YEAR;

Time::Timespec::Timespec( time_t sec, long nsec )
{
    mTimespec.tv_sec = sec;
    mTimespec.tv_nsec = nsec;
}

Time::Timespec::Timespec( const timespec& ts )
: mTimespec( ts )
{
}

Time::Timespec::Timespec( const Time::Msec& msec )
{
    mTimespec.tv_sec = msec.count() / MSEC_PER_SEC;
    mTimespec.tv_nsec = NSEC_PER_USEC * USEC_PER_MSEC * ( msec.count() % MSEC_PER_SEC );
}

Time::Timespec::Timespec( const Time::Timespec& oth )
: mTimespec( oth.ts() )
{
}

Time::Timespec::Timespec( const Time::Timeval& tv )
{
    mTimespec.tv_sec = tv.sec();
    mTimespec.tv_nsec = NSEC_PER_USEC * tv.usec();
}

Time::Timespec::Timespec( const Time::WinTime& time )
{
    mTimespec.tv_sec = time.count() / ( WINTIME_PER_USEC * USEC_PER_MSEC * MSEC_PER_SEC );
    mTimespec.tv_nsec = NSEC_PER_WINTIME * ( time.count() % ( WINTIME_PER_USEC * USEC_PER_MSEC * MSEC_PER_SEC ) );
}

bool Time::Timespec::operator==( const Time::Timespec& oth ) const
{
    if( sec() != oth.sec() )
        return false;
    else if ( nsec() != oth.nsec() )
        return false;
    else
        return true;
}

bool Time::Timespec::operator!=( const Time::Timespec& oth ) const
{
    if( sec() != oth.sec() )
        return true;
    else if( nsec() != oth.nsec() )
        return true;
    else
        return false;
}

bool Time::Timespec::operator<( const Time::Timespec& oth ) const
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

bool Time::Timespec::operator<=( const Time::Timespec& oth ) const
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

bool Time::Timespec::operator>( const Time::Timespec& oth ) const
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

bool Time::Timespec::operator>=( const Time::Timespec& oth ) const
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

Time::Timespec Time::Timespec::operator+( const Time::Timespec& oth ) const
{
    return Time::Timespec( ( sec() + oth.sec() )
                           + ( nsec() + oth.nsec() ) / ( NSEC_PER_USEC * USEC_PER_MSEC * MSEC_PER_SEC ),
                           ( nsec() + oth.nsec() ) % ( NSEC_PER_USEC * USEC_PER_MSEC * MSEC_PER_SEC ) );
}

Time::Timespec Time::Timespec::operator-( const Time::Timespec& oth ) const
{
    const size_t k = ( nsec() < oth.nsec()
                       ? 1 + ( oth.nsec() - nsec() - 1 ) / ( NSEC_PER_USEC * USEC_PER_MSEC * MSEC_PER_SEC )
                       : 0 );

    return Time::Timespec( sec() - oth.sec() - k,
                           NSEC_PER_USEC * USEC_PER_MSEC * MSEC_PER_SEC * k + nsec() - oth.nsec() );
}

Time::Timespec Time::Timespec::operator*( size_t ratio ) const
{
    return Time::Timespec( ( sec() * ratio )
                           + ( nsec() * ratio ) / ( NSEC_PER_USEC * USEC_PER_MSEC * MSEC_PER_SEC ),
                           ( nsec() * ratio ) % ( NSEC_PER_USEC * USEC_PER_MSEC * MSEC_PER_SEC ) );
}

Time::Timespec Time::Timespec::operator/( size_t ratio ) const
{
    return Time::Timespec( ( sec() / ratio ),
                           ( NSEC_PER_USEC * USEC_PER_MSEC * MSEC_PER_SEC * ( sec() % ratio )
                             + nsec() ) / ratio );
}

Time::Timespec Time::Timespec::operator%( size_t ratio ) const
{
    const size_t mod = ( NSEC_PER_USEC * USEC_PER_MSEC * MSEC_PER_SEC * ( sec() % ratio )
                         + nsec() ) % ratio;

    return Time::Timespec( mod / ( NSEC_PER_USEC * USEC_PER_MSEC * MSEC_PER_SEC ),
                           mod % ( NSEC_PER_USEC * USEC_PER_MSEC * MSEC_PER_SEC ) );
}

Time::Timespec& Time::Timespec::operator=( const Time::Timespec& oth )
{
    mTimespec = oth.ts();
    return *this;
}

Time::Timespec& Time::Timespec::operator+=( const Time::Timespec& oth )
{
    return ( *this = *this + oth );
}

Time::Timespec& Time::Timespec::operator-=( const Time::Timespec& oth )
{
    return ( *this = *this - oth );
}

Time::Timespec& Time::Timespec::operator*=( size_t ratio )
{
    return ( *this = *this * ratio );
}

Time::Timespec& Time::Timespec::operator/=( size_t ratio )
{
    return ( *this = *this / ratio );
}

Time::Timespec& Time::Timespec::operator%=( size_t ratio )
{
    return ( *this = *this % ratio );
}
#endif /* !WIN32 */
