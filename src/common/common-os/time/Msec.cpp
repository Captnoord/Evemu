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
/* common::time::Msec                                                    */
/*************************************************************************/
const Msec Msec::MSEC = 1;
const Msec Msec::SEC = Msec::MSEC * MSEC_PER_SEC;
const Msec Msec::MIN = Msec::SEC * SEC_PER_MIN;
const Msec Msec::HOUR = Msec::MIN * MIN_PER_HOUR;
const Msec Msec::DAY = Msec::HOUR * HOUR_PER_DAY;
const Msec Msec::MONTH = Msec::DAY * DAY_PER_MONTH;
const Msec Msec::YEAR = Msec::MONTH * MONTH_PER_YEAR;

Msec::Msec( size_t msec )
: mMsec( msec )
{
}

Msec::Msec( const Msec& oth )
: mMsec( oth.count() )
{
}

#ifndef WIN32
Msec::Msec( const Timespec& ts )
: mMsec( MSEC_PER_SEC * ts.sec() + ts.nsec() / ( NSEC_PER_USEC * USEC_PER_MSEC ) )
{
}
#endif /* !WIN32 */

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

Msec Msec::operator*( size_t ratio ) const
{
    return count() * ratio;
}

Msec Msec::operator/( size_t ratio ) const
{
    return count() / ratio;
}

Msec Msec::operator%( size_t ratio ) const
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

Msec& Msec::operator*=( size_t ratio )
{
    mMsec *= ratio;
    return *this;
}

Msec& Msec::operator/=( size_t ratio )
{
    mMsec /= ratio;
    return *this;
}

Msec& Msec::operator%=( size_t ratio )
{
    mMsec %= ratio;
    return *this;
}
