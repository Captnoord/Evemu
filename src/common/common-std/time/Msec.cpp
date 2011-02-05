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

#include "CommonStd.h"

#include "time/Const.h"
#include "time/Msec.h"
#include "time/WinTime.h"

/*************************************************************************/
/* Time::Msec                                                            */
/*************************************************************************/
const Time::Msec Time::Msec::MSEC = 1;
const Time::Msec Time::Msec::SEC = Time::Msec::MSEC * Time::MSEC_PER_SEC;
const Time::Msec Time::Msec::MIN = Time::Msec::SEC * Time::SEC_PER_MIN;
const Time::Msec Time::Msec::HOUR = Time::Msec::MIN * Time::MIN_PER_HOUR;
const Time::Msec Time::Msec::DAY = Time::Msec::HOUR * Time::HOUR_PER_DAY;
const Time::Msec Time::Msec::MONTH = Time::Msec::DAY * Time::DAY_PER_MONTH;
const Time::Msec Time::Msec::YEAR = Time::Msec::MONTH * Time::MONTH_PER_YEAR;

Time::Msec::Msec( size_t msec )
: mMsec( msec )
{
}

Time::Msec::Msec( const Time::Msec& oth )
: mMsec( oth.count() )
{
}

Time::Msec::Msec( const Time::WinTime& time )
: mMsec( time.count() / ( WINTIME_PER_USEC * USEC_PER_MSEC ) )
{
}

bool Time::Msec::operator==( const Time::Msec& oth ) const
{
    return count() == oth.count();
}

bool Time::Msec::operator!=( const Time::Msec& oth ) const
{
    return count() != oth.count();
}

bool Time::Msec::operator<( const Time::Msec& oth ) const
{
    return count() < oth.count();
}

bool Time::Msec::operator<=( const Time::Msec& oth ) const
{
    return count() <= oth.count();
}

bool Time::Msec::operator>( const Time::Msec& oth ) const
{
    return count() > oth.count();
}

bool Time::Msec::operator>=( const Time::Msec& oth ) const
{
    return count() >= oth.count();
}

Time::Msec Time::Msec::operator+( const Time::Msec& oth ) const
{
    return count() + oth.count();
}

Time::Msec Time::Msec::operator-( const Time::Msec& oth ) const
{
    return count() - oth.count();
}

Time::Msec Time::Msec::operator*( size_t ratio ) const
{
    return count() * ratio;
}

Time::Msec Time::Msec::operator/( size_t ratio ) const
{
    return count() / ratio;
}

Time::Msec Time::Msec::operator%( size_t ratio ) const
{
    return count() % ratio;
}

Time::Msec& Time::Msec::operator=( const Time::Msec& oth )
{
    mMsec = oth.count();
    return *this;
}

Time::Msec& Time::Msec::operator+=( const Time::Msec& oth )
{
    mMsec += oth.count();
    return *this;
}

Time::Msec& Time::Msec::operator-=( const Time::Msec& oth )
{
    mMsec -= oth.count();
    return *this;
}

Time::Msec& Time::Msec::operator*=( size_t ratio )
{
    mMsec *= ratio;
    return *this;
}

Time::Msec& Time::Msec::operator/=( size_t ratio )
{
    mMsec /= ratio;
    return *this;
}

Time::Msec& Time::Msec::operator%=( size_t ratio )
{
    mMsec %= ratio;
    return *this;
}
