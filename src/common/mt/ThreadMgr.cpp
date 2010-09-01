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

    SetThreadLimit( 0 );
}

size_t Mt::ThreadMgr::threadCount() const
{
    MutexLock lock( mMutex );

    return activeThreadCount() + inactiveThreadCount();
}

size_t Mt::ThreadMgr::activeThreadCount() const
{
    MutexLock lock( mMutex );

    return mActiveWorkers.size();
}

size_t Mt::ThreadMgr::inactiveThreadCount() const
{
    MutexLock lock( mMutex );

    return mInactiveWorkers.size();
}

size_t Mt::ThreadMgr::threadLimit() const
{
    MutexLock lock( mMutex );

    return mLimit;
}

void Mt::ThreadMgr::Run( Mt::Target* target )
{
    MutexLock lock( mMutex );

    mTargets.push( target );

    if( 0 < inactiveThreadCount() )
        // Someone is waiting, signal the condition.
        mWorkerEvent.Signal();
    else if( threadCount() < threadLimit() )
        // Haven't reached the limit yet, spawn a worker.
        Thread t( new Worker( this ) );
}

void Mt::ThreadMgr::SetThreadLimit( size_t limit )
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

Mt::ThreadMgr::WorkerList::iterator Mt::ThreadMgr::AddWorker( Worker* worker )
{
    MutexLock lock( mMutex );

    mActiveWorkers.push_back( worker );
    return --mActiveWorkers.end();
}

void Mt::ThreadMgr::RemoveWorker( WorkerList::iterator& itr )
{
    MutexLock lock( mMutex );

    mActiveWorkers.erase( itr );
}

Mt::Target* Mt::ThreadMgr::GetTarget( WorkerList::iterator& itr )
{
    MutexLock lock( mMutex );

    // move the Worker from mActiveWorkers to mInactiveWorkers
    mInactiveWorkers.splice( mInactiveWorkers.end(),
                             mActiveWorkers, itr );
    itr = --mInactiveWorkers.end();

    Mt::Target* target = NULL;
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
/* Mt::ThreadMgr::Worker                                                 */
/*************************************************************************/
Mt::ThreadMgr::Worker::Worker( Mt::ThreadMgr* mgr )
: mMgr( NULL )
{
    AssignMgr( mgr );
}

Mt::ThreadMgr::Worker::~Worker()
{
    AssignMgr( NULL );
}

void Mt::ThreadMgr::Worker::AssignMgr( Mt::ThreadMgr* mgr )
{
    MutexLock lock( mMutex );

    if( NULL != mMgr )
        mMgr->RemoveWorker( mItr );

    mMgr = mgr;

    if( NULL != mMgr )
        mItr = mMgr->AddWorker( this );
}

Mt::Target* Mt::ThreadMgr::Worker::GetTarget()
{
    if( NULL == mMgr )
        return NULL;

    return mMgr->GetTarget( mItr );
}
