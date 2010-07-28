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
    Author:     Zhur
*/

#include "CommonPCH.h"

#include "thread/Thread.h"
#include "time/TimeMsec.h"
#include "time/Timer.h"

/*************************************************************************/
/* Timer                                                                 */
/*************************************************************************/
Timer::Timer( size_t period, bool accurate )
: mEnd( 0 ),
  mPeriod( period ),
  mAccurate( accurate )
{
}

Timer::~Timer()
{
}

void Timer::Start()
{
    if( !accurate() || 0 == mEnd )
        SetMsecByNow( mEnd );

    mEnd += period();
}

bool Timer::Check( bool restart )
{
    size_t msec;
    SetMsecByNow( msec );

    if( mEnd <= msec )
    {
        if( restart )
            Start();

        return true;
    }
    else
        return false;
}

void Timer::Sleep( bool restart )
{
    size_t msec;
    SetMsecByNow( msec );

    if( msec < mEnd )
        Thread::Sleep( mEnd - msec );

    if( restart )
        Start();
}
