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

#include "mt/ThreadMgr.h"
#include "util/Log.h"

/*************************************************************************/
/* Mt::ThreadMgr                                                         */
/*************************************************************************/
Mt::ThreadMgr::ThreadMgr( size_t limit )
: mLimit( limit )
{
    sLog.Debug( "Mt::ThreadMgr", "Thread manager started" );
}

Mt::ThreadMgr::~ThreadMgr()
{
    sLog.Debug( "Mt::ThreadMgr", "Thread manager shutting down" );

    Stop();

    sLog.Debug( "Mt::ThreadMgr", "Thread manager shut down" );
}

void Mt::ThreadMgr::Run( Mt::TargetEx* target )
{
    MutexLock lock( mMutex );

    mQueuedTargets.push( target );

    if( activeThreadCount() + queueLen() <= threadCount() )
    {
        // Just signal the condition, somebody will do it.
        mEvent.Signal();
    }
    else if( activeThreadCount() + queueLen() <= threadLimit() )
    {
        // Haven't reached the limit yet, spawn a worker.
        Thread t( this );
    }
}

void Mt::ThreadMgr::Stop()
{
    MutexLock lock( mMutex );

    // set limit, release all waiting threads
    SetThreadLimit( 0 );

    // release all working threads
    std::list< TargetEx* >::iterator cur, end;
    cur = mActiveTargets.begin();
    end = mActiveTargets.end();
    for(; cur != end; ++cur )
        ( *cur )->Stop();

    // wait until all threads quit
    while( !mThreads.empty() )
    {
        // we must copy the handle
        Thread t = mThreads.front();

        lock.Unlock();
        t.Wait();
        lock.Relock();
    }
}

void Mt::ThreadMgr::SetThreadLimit( size_t limit )
{
    MutexLock lock( mMutex );

    mLimit = limit;

    if( threadLimit() < threadCount()
        && activeThreadCount() < threadCount() )
    {
        // Broadcast thread event to make waiting threads quit
        mEvent.Broadcast();
    }
}

void Mt::ThreadMgr::Run()
{
    MutexLock lock( mMutex );

    mThreads.push_back( Thread::self() );
    const std::list< Thread >::iterator threadItr = --mThreads.end();

    while( threadCount() <= threadLimit() )
    {
        if( mQueuedTargets.empty() )
            mEvent.Wait( mMutex );
        else
        {
            mActiveTargets.push_back( mQueuedTargets.front() );
            mQueuedTargets.pop();
            const std::list< TargetEx* >::iterator targetItr = --mActiveTargets.end();

            lock.Unlock();
            Target::Process( *targetItr );
            lock.Relock();

            mActiveTargets.erase( targetItr );
        }
    }

    mThreads.erase( threadItr );
}
