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

#include "CommonOs.h"

#include "time/Timeval.h"

/*************************************************************************/
/* Time::Timeval                                                         */
/*************************************************************************/
const Time::Timeval Time::Timeval::USEC = Time::Timeval( 0, 1 );
const Time::Timeval Time::Timeval::MSEC = Time::Timeval::USEC * Time::USEC_PER_MSEC;
const Time::Timeval Time::Timeval::SEC = Time::Timeval::MSEC * Time::MSEC_PER_SEC;
const Time::Timeval Time::Timeval::MIN = Time::Timeval::SEC * Time::SEC_PER_MIN;
const Time::Timeval Time::Timeval::HOUR = Time::Timeval::MIN * Time::MIN_PER_HOUR;
const Time::Timeval Time::Timeval::DAY = Time::Timeval::HOUR * Time::HOUR_PER_DAY;
const Time::Timeval Time::Timeval::MONTH = Time::Timeval::DAY * Time::DAY_PER_MONTH;
const Time::Timeval Time::Timeval::YEAR = Time::Timeval::MONTH * Time::MONTH_PER_YEAR;

Time::Timeval::Timeval( time_t sec, useconds_t usec )
{
    mTimeval.tv_sec = sec;
    mTimeval.tv_usec = usec;
}

Time::Timeval::Timeval( const timeval& tv )
: mTimeval( tv )
{
}

Time::Timeval::Timeval( const Time::Msec& msec )
{
    mTimeval.tv_sec = msec.count() / MSEC_PER_SEC;
    mTimeval.tv_usec = USEC_PER_MSEC * ( msec.count() % MSEC_PER_SEC );
}

Time::Timeval::Timeval( const Time::Timeval& oth )
: mTimeval( oth.tv() )
{
}

Time::Timeval::Timeval( const Time::WinTime& time )
{
    mTimeval.tv_sec = time.count() / ( WINTIME_PER_USEC * USEC_PER_MSEC * MSEC_PER_SEC );
    mTimeval.tv_usec = ( time.count() % ( WINTIME_PER_USEC * USEC_PER_MSEC * MSEC_PER_SEC ) ) / WINTIME_PER_USEC;
}

bool Time::Timeval::operator==( const Time::Timeval& oth ) const
{
    if( sec() != oth.sec() )
        return false;
    else if( usec() != oth.usec() )
        return false;
    else
        return true;
}

bool Time::Timeval::operator!=( const Time::Timeval& oth ) const
{
    if( sec() != oth.sec() )
        return true;
    else if( usec() != oth.usec() )
        return true;
    else
        return false;
}

bool Time::Timeval::operator<( const Time::Timeval& oth ) const
{
    if( sec() < oth.sec() )
        return true;
    else if( sec() > oth.sec() )
        return false;
    else if( usec() < oth.usec() )
        return true;
    else
        return false;
}

bool Time::Timeval::operator<=( const Time::Timeval& oth ) const
{
    if( sec() < oth.sec() )
        return true;
    else if( sec() > oth.sec() )
        return false;
    else if( usec() <= oth.usec() )
        return true;
    else
        return false;
}

bool Time::Timeval::operator>( const Time::Timeval& oth ) const
{
    if( sec() > oth.sec() )
        return true;
    else if( sec() < oth.sec() )
        return false;
    else if( usec() > oth.usec() )
        return true;
    else
        return false;
}

bool Time::Timeval::operator>=( const Time::Timeval& oth ) const
{
    if( sec() > oth.sec() )
        return true;
    else if( sec() < oth.sec() )
        return false;
    else if( usec() >= oth.usec() )
        return true;
    else
        return false;
}

Time::Timeval Time::Timeval::operator+( const Time::Timeval& oth ) const
{
    return Time::Timeval( ( sec() + oth.sec() )
                          + ( usec() + oth.usec() ) / ( USEC_PER_MSEC * MSEC_PER_SEC ),
                          ( usec() + oth.usec() ) % ( USEC_PER_MSEC * MSEC_PER_SEC ) );
}

Time::Timeval Time::Timeval::operator-( const Time::Timeval& oth ) const
{
    const size_t k = ( usec() < oth.usec()
                       ? 1 + ( oth.usec() - usec() - 1 ) / ( USEC_PER_MSEC * MSEC_PER_SEC )
                       : 0 );

    return Time::Timeval( sec() - oth.sec() - k,
                          USEC_PER_MSEC * MSEC_PER_SEC * k + usec() - oth.usec() );
}

Time::Timeval Time::Timeval::operator*( size_t ratio ) const
{
    return Time::Timeval( ( sec() * ratio )
                          + ( usec() * ratio ) / ( USEC_PER_MSEC * MSEC_PER_SEC ),
                          ( usec() * ratio ) % ( USEC_PER_MSEC * MSEC_PER_SEC ) );
}

Time::Timeval Time::Timeval::operator/( size_t ratio ) const
{
    return Time::Timeval( ( sec() / ratio ),
                          ( USEC_PER_MSEC * MSEC_PER_SEC * ( sec() % ratio )
                            + usec() ) / ratio );
}

Time::Timeval Time::Timeval::operator%( size_t ratio ) const
{
    const size_t mod = ( USEC_PER_MSEC * MSEC_PER_SEC * ( sec() % ratio )
                         + usec() ) % ratio;

    return Time::Timeval( mod / ( USEC_PER_MSEC * MSEC_PER_SEC ),
                          mod % ( USEC_PER_MSEC * MSEC_PER_SEC ) );
}

Time::Timeval& Time::Timeval::operator=( const Time::Timeval& oth )
{
    mTimeval = oth.tv();
    return *this;
}

Time::Timeval& Time::Timeval::operator+=( const Time::Timeval& oth )
{
    return ( *this = *this + oth );
}

Time::Timeval& Time::Timeval::operator-=( const Time::Timeval& oth )
{
    return ( *this = *this - oth );
}

Time::Timeval& Time::Timeval::operator*=( size_t ratio )
{
    return ( *this = *this * ratio );
}

Time::Timeval& Time::Timeval::operator/=( size_t ratio )
{
    return ( *this = *this / ratio );
}

Time::Timeval& Time::Timeval::operator%=( size_t ratio )
{
    return ( *this = *this % ratio );
}
