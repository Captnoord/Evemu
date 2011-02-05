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
/* common::time::Timeval                                                 */
/*************************************************************************/
const Timeval Timeval::USEC = Timeval( 0, 1 );
const Timeval Timeval::MSEC = Timeval::USEC * USEC_PER_MSEC;
const Timeval Timeval::SEC = Timeval::MSEC * MSEC_PER_SEC;
const Timeval Timeval::MIN = Timeval::SEC * SEC_PER_MIN;
const Timeval Timeval::HOUR = Timeval::MIN * MIN_PER_HOUR;
const Timeval Timeval::DAY = Timeval::HOUR * HOUR_PER_DAY;
const Timeval Timeval::MONTH = Timeval::DAY * DAY_PER_MONTH;
const Timeval Timeval::YEAR = Timeval::MONTH * MONTH_PER_YEAR;

Timeval::Timeval( time_t sec, useconds_t usec )
{
    mTimeval.tv_sec = sec;
    mTimeval.tv_usec = usec;
}

Timeval::Timeval( const timeval& tv )
: mTimeval( tv )
{
}

Timeval::Timeval( const Msec& msec )
{
    mTimeval.tv_sec = msec.count() / MSEC_PER_SEC;
    mTimeval.tv_usec = USEC_PER_MSEC * ( msec.count() % MSEC_PER_SEC );
}

#ifndef WIN32
Timeval::Timeval( const Timespec& ts )
{
    mTimeval.tv_sec = ts.sec();
    mTimeval.tv_usec = ts.nsec() / NSEC_PER_USEC;
}
#endif /* !WIN32 */

Timeval::Timeval( const Timeval& oth )
: mTimeval( oth.tv() )
{
}

Timeval::Timeval( const WinTime& time )
{
    mTimeval.tv_sec = time.count() / ( WINTIME_PER_USEC * USEC_PER_MSEC * MSEC_PER_SEC );
    mTimeval.tv_usec = ( time.count() % ( WINTIME_PER_USEC * USEC_PER_MSEC * MSEC_PER_SEC ) ) / WINTIME_PER_USEC;
}

bool Timeval::operator==( const Timeval& oth ) const
{
    if( sec() != oth.sec() )
        return false;
    else if( usec() != oth.usec() )
        return false;
    else
        return true;
}

bool Timeval::operator!=( const Timeval& oth ) const
{
    if( sec() != oth.sec() )
        return true;
    else if( usec() != oth.usec() )
        return true;
    else
        return false;
}

bool Timeval::operator<( const Timeval& oth ) const
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

bool Timeval::operator<=( const Timeval& oth ) const
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

bool Timeval::operator>( const Timeval& oth ) const
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

bool Timeval::operator>=( const Timeval& oth ) const
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

Timeval Timeval::operator+( const Timeval& oth ) const
{
    return Timeval( ( sec() + oth.sec() )
                          + ( usec() + oth.usec() ) / ( USEC_PER_MSEC * MSEC_PER_SEC ),
                          ( usec() + oth.usec() ) % ( USEC_PER_MSEC * MSEC_PER_SEC ) );
}

Timeval Timeval::operator-( const Timeval& oth ) const
{
    const size_t k = ( usec() < oth.usec()
                       ? 1 + ( oth.usec() - usec() - 1 ) / ( USEC_PER_MSEC * MSEC_PER_SEC )
                       : 0 );

    return Timeval( sec() - oth.sec() - k,
                          USEC_PER_MSEC * MSEC_PER_SEC * k + usec() - oth.usec() );
}

Timeval Timeval::operator*( size_t ratio ) const
{
    return Timeval( ( sec() * ratio )
                          + ( usec() * ratio ) / ( USEC_PER_MSEC * MSEC_PER_SEC ),
                          ( usec() * ratio ) % ( USEC_PER_MSEC * MSEC_PER_SEC ) );
}

Timeval Timeval::operator/( size_t ratio ) const
{
    return Timeval( ( sec() / ratio ),
                          ( USEC_PER_MSEC * MSEC_PER_SEC * ( sec() % ratio )
                            + usec() ) / ratio );
}

Timeval Timeval::operator%( size_t ratio ) const
{
    const size_t mod = ( USEC_PER_MSEC * MSEC_PER_SEC * ( sec() % ratio )
                         + usec() ) % ratio;

    return Timeval( mod / ( USEC_PER_MSEC * MSEC_PER_SEC ),
                          mod % ( USEC_PER_MSEC * MSEC_PER_SEC ) );
}

Timeval& Timeval::operator=( const Timeval& oth )
{
    mTimeval = oth.tv();
    return *this;
}

Timeval& Timeval::operator+=( const Timeval& oth )
{
    return ( *this = *this + oth );
}

Timeval& Timeval::operator-=( const Timeval& oth )
{
    return ( *this = *this - oth );
}

Timeval& Timeval::operator*=( size_t ratio )
{
    return ( *this = *this * ratio );
}

Timeval& Timeval::operator/=( size_t ratio )
{
    return ( *this = *this / ratio );
}

Timeval& Timeval::operator%=( size_t ratio )
{
    return ( *this = *this % ratio );
}
