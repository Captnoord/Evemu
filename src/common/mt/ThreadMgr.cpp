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
#include "utils/Log.h"

/*************************************************************************/
/* ThreadMgr                                                             */
/*************************************************************************/
ThreadMgr::ThreadMgr( size_t limit )
: mLimit( limit )
{
    sLog.Debug( "ThreadMgr", "Thread manager started" );
}

ThreadMgr::~ThreadMgr()
{
    sLog.Debug( "ThreadMgr", "Thread manager shutting down" );

    SetThreadLimit( 0 );
}

size_t ThreadMgr::threadCount() const
{
    MutexLock lock( mMutex );

    return activeThreadCount() + inactiveThreadCount();
}

size_t ThreadMgr::activeThreadCount() const
{
    MutexLock lock( mMutex );

    return mActiveWorkers.size();
}

size_t ThreadMgr::inactiveThreadCount() const
{
    MutexLock lock( mMutex );

    return mInactiveWorkers.size();
}

size_t ThreadMgr::threadLimit() const
{
    MutexLock lock( mMutex );

    return mLimit;
}

void ThreadMgr::Run( Thread::Target* target )
{
    MutexLock lock( mMutex );

    mTargets.push( target );

    if( 0 < inactiveThreadCount() )
        // Someone is waiting, signal the condition.
        mWorkerEvent.Signal();
    else if( threadCount() < threadLimit() )
        // Haven't reached the limit yet, spawn a worker.
        new Worker( this );
}

void ThreadMgr::SetThreadLimit( size_t limit )
{
    MutexLock lock( mMutex );

    mLimit = limit;

    if( threadLimit() < threadCount()
        && 0 < inactiveThreadCount() )
    {
        // Broadcast thread event to make waiting threads quit
        mWorkerEvent.Broadcast();
    }

    if( threadLimit() < activeThreadCount() )
    {
        // We need to stop some working threads ...
        for( size_t i = ( activeThreadCount() - threadLimit() ); 0 < i; --i )
        {
            Worker* worker = mActiveWorkers.front();

            // This also removes the worker from our list.
            worker->AssignMgr( NULL );
        }
    }
}

ThreadMgr::WorkerList::iterator ThreadMgr::AddWorker( Worker* worker )
{
    MutexLock lock( mMutex );

    mActiveWorkers.push_back( worker );
    return --mActiveWorkers.end();
}

void ThreadMgr::RemoveWorker( WorkerList::iterator& itr )
{
    MutexLock lock( mMutex );

    mActiveWorkers.erase( itr );
}

Thread::Target* ThreadMgr::GetTarget( WorkerList::iterator& itr )
{
    MutexLock lock( mMutex );

    // move the Worker from mActiveWorkers to mInactiveWorkers
    mInactiveWorkers.splice( mInactiveWorkers.end(),
                             mActiveWorkers, itr );
    itr = --mInactiveWorkers.end();

    Thread::Target* target = NULL;
    while( threadCount() <= threadLimit() )
    {
        if( !mTargets.empty() )
        {
            target = mTargets.front();
            mTargets.pop();
            break;
        }

        mWorkerEvent.Wait( mMutex );
    }

    // move the Worker back
    mActiveWorkers.splice( mActiveWorkers.end(),
                           mInactiveWorkers, itr );
    itr = --mActiveWorkers.end();

    return target;
}

/*************************************************************************/
/* ThreadMgr::Worker                                                     */
/*************************************************************************/
ThreadMgr::Worker::Worker( ThreadMgr* mgr )
: mMgr( NULL )
{
    AssignMgr( mgr );

    Thread thread( this );
}

ThreadMgr::Worker::~Worker()
{
    AssignMgr( NULL );
}

void ThreadMgr::Worker::AssignMgr( ThreadMgr* mgr )
{
    MutexLock lock( mMutex );

    if( NULL != mMgr )
        mMgr->RemoveWorker( mItr );

    mMgr = mgr;

    if( NULL != mMgr )
        mItr = mMgr->AddWorker( this );
}

Thread::Target* ThreadMgr::Worker::GetTarget()
{
    if( NULL == mMgr )
        return NULL;

    return mMgr->GetTarget( mItr );
}
