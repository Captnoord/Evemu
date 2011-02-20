/**
 * @file time/Msec.cpp
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
#include "time/Timeval.h"
#include "time/WinTime.h"

#ifdef HAVE_TIMESPEC
#   include "time/Timespec.h"
#endif /* HAVE_TIMESPEC */

using namespace common;
using namespace common::time;

/*************************************************************************/
/* common::time::Msec                                                    */
/*************************************************************************/
const Msec Msec::MSEC = 1;
const Msec Msec::SEC = Msec::MSEC * MSEC_PER_SEC;
const Msec Msec::MIN = Msec::SEC * SEC_PER_MIN;
const Msec Msec::HOUR = Msec::MIN * MIN_PER_HOUR;
const Msec Msec::DAY = Msec::HOUR * HOUR_PER_DAY;
const Msec Msec::MONTH = Msec::DAY * DAY_PER_MONTH;
const Msec Msec::YEAR = Msec::MONTH * MONTH_PER_YEAR;

Msec::Msec( unsigned long long msec )
: mMsec( msec )
{
}

Msec::Msec( const Msec& oth )
: mMsec( oth.count() )
{
}

#ifdef HAVE_TIMESPEC
Msec::Msec( const Timespec& ts )
: mMsec( MSEC_PER_SEC * ts.sec() + ts.nsec() / ( NSEC_PER_USEC * USEC_PER_MSEC ) )
{
}
#endif /* HAVE_TIMESPEC */

Msec::Msec( const Timeval& tv )
: mMsec( MSEC_PER_SEC * tv.sec() + tv.usec() / USEC_PER_MSEC )
{
}

Msec::Msec( const WinTime& time )
: mMsec( time.count() / ( WINTIME_PER_USEC * USEC_PER_MSEC ) )
{
}

bool Msec::operator==( const Msec& oth ) const
{
    return count() == oth.count();
}

bool Msec::operator!=( const Msec& oth ) const
{
    return count() != oth.count();
}

bool Msec::operator<( const Msec& oth ) const
{
    return count() < oth.count();
}

bool Msec::operator<=( const Msec& oth ) const
{
    return count() <= oth.count();
}

bool Msec::operator>( const Msec& oth ) const
{
    return count() > oth.count();
}

bool Msec::operator>=( const Msec& oth ) const
{
    return count() >= oth.count();
}

Msec Msec::operator+( const Msec& oth ) const
{
    return count() + oth.count();
}

Msec Msec::operator-( const Msec& oth ) const
{
    return count() - oth.count();
}

Msec Msec::operator*( unsigned long long ratio ) const
{
    return count() * ratio;
}

Msec Msec::operator/( unsigned long long ratio ) const
{
    return count() / ratio;
}

Msec Msec::operator%( unsigned long long ratio ) const
{
    return count() % ratio;
}

Msec& Msec::operator=( const Msec& oth )
{
    mMsec = oth.count();
    return *this;
}

Msec& Msec::operator+=( const Msec& oth )
{
    mMsec += oth.count();
    return *this;
}

Msec& Msec::operator-=( const Msec& oth )
{
    mMsec -= oth.count();
    return *this;
}

Msec& Msec::operator*=( unsigned long long ratio )
{
    mMsec *= ratio;
    return *this;
}

Msec& Msec::operator/=( unsigned long long ratio )
{
    mMsec /= ratio;
    return *this;
}

Msec& Msec::operator%=( unsigned long long ratio )
{
    mMsec %= ratio;
    return *this;
}
