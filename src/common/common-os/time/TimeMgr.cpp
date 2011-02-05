/**
 * @file time/TimeMgr.cpp
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

#include "mt/ThreadMgr.h"
#include "time/Const.h"
#include "time/TimeMgr.h"

using namespace common;
using namespace common::time;

/*************************************************************************/
/* common::time::TimeMgr                                                 */
/*************************************************************************/
TimeMgr::TimeMgr( size_t period )
: mRun( true ),
  mRunTimer( period )
{
    // do a single update ourselves
    Update();
    // start the update thread
    sThreadMgr.Run( this );
}

Tm TimeMgr::nowTm() const
{
    Tm t;

    {
        mt::MutexLock lock( mMutex );
        t = mTm;
    }

    return t;
}

WinTime TimeMgr::nowWin() const
{
    WinTime wt;

    {
        mt::MutexLock lock( mMutex );

#   ifdef WIN32
        wt = mWinTime;
#   else /* !WIN32 */
        wt = mTimeval + Timeval::SEC * EPOCH_DIFF_SEC;
#   endif /* !WIN32 */
    }

    return wt;
}

Timeval TimeMgr::nowUnix() const
{
    Timeval tv;

    {
        mt::MutexLock lock( mMutex );

#   ifdef WIN32
        tv = mWinTime - WinTime::SEC * EPOCH_DIFF_SEC;
#   else /* !WIN32 */
        tv = mTimeval;
#   endif /* !WIN32 */
    }

    return tv;
}

void TimeMgr::Update()
{
    mt::MutexLock lock( mMutex );

#ifdef WIN32
    FILETIME ft;
    ::GetSystemTimeAsFileTime( &ft );
    mWinTime = ( static_cast< uint64 >( ft.dwHighDateTime ) << 32 )
               | static_cast< uint64 >( ft.dwLowDateTime );

    time_t t = ::time( NULL );
    assert( -1 != t );
    mTm = t;
#else /* !WIN32 */
    timeval tv;
    int code = ::gettimeofday( &tv, NULL );
    assert( 0 == code );

    mTimeval = tv;
    mTm = tv.tv_sec;
#endif /* !WIN32 */
}

void TimeMgr::Run()
{
    mt::MutexLock lock( mMutex );

    mRunTimer.Start();
    while( mRun )
    {
        // update the time stuff
        Update();

        // wait for the next update time
        lock.Unlock();
        mRunTimer.Sleep();
        lock.Relock();
    }
}

void TimeMgr::Stop()
{
    mRun = false;
}
