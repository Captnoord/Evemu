/**
 * @file time/Timer.cpp
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
 * @author Zhur
 */

#include "CommonOs.h"

#include "mt/Thread.h"
#include "time/TimeMgr.h"
#include "time/Timer.h"

using namespace common;
using namespace common::time;

/*************************************************************************/
/* common::time::Timer                                                   */
/*************************************************************************/
Timer::Timer( const Msec& period, bool accurate )
: mTimeout( 0 ),
  mPeriod( period ),
  mAccurate( accurate )
{
}

void Timer::Start()
{
    if( !accurate() || mTimeout == 0 )
    {
#   ifdef WIN32
        mTimeout = sTimeMgr.nowWin();
#   else /* !WIN32 */
        mTimeout = sTimeMgr.nowUnix();
#   endif /* !WIN32 */
    }

    mTimeout += period();
}

bool Timer::Check( bool restart )
{
#ifdef WIN32
    Msec msec = sTimeMgr.nowWin();
#else /* !WIN32 */
    Msec msec = sTimeMgr.nowUnix();
#endif /* !WIN32 */

    if( mTimeout <= msec )
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
#ifdef WIN32
    Msec msec = sTimeMgr.nowWin();
#else /* !WIN32 */
    Msec msec = sTimeMgr.nowUnix();
#endif /* !WIN32 */

    if( msec < mTimeout )
        mt::Thread::Sleep( mTimeout - msec );

    if( restart )
        Start();
}
