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

#ifndef __MT__THREAD_MGR_H__INCL__
#define __MT__THREAD_MGR_H__INCL__

#include "mt/Condition.h"
#include "mt/Mutex.h"
#include "mt/Worker.h"
#include "utils/Singleton.h"

namespace Mt
{
    /**
     * @brief Thread manager, does thread pooling.
     *
     * @author Bloody.Rabbit
     */
    class ThreadMgr
    : public Singleton< ThreadMgr >
    {
    public:
        /**
         * @brief A primary constructor.
         *
         * @param[in] limit Thread count limit.
         */
        ThreadMgr( size_t limit = 100 );
        /*
         * @brief A primary destructor.
         */
        ~ThreadMgr();

        /// Obtains current thread count.
        size_t threadCount() const;
        /// Obtains current active thread count.
        size_t activeThreadCount() const;
        /// Obtains current inactive thread count.
        size_t inactiveThreadCount() const;

        /// Obtains current thread count limit.
        size_t threadLimit() const;

        /**
         * @brief Adds a new target to the processing queue.
         *
         * @param[in] target The new target.
         */
        void Run( Target* target );

        /**
         * @brief Sets new thread count limit.
         *
         * @param[in] limit The new limit.
         */
        void SetThreadLimit( size_t limit );

    protected:
        class Worker;

        /// Typedef for target queue.
        typedef std::queue< Target* > TargetQueue;
        /// Typedef for working threads list.
        typedef std::list< Worker* > WorkerList;

        /**
         * @brief Registers new Worker.
         *
         * @param[in] worker The new worker to be registered.
         *
         * @return The worker's iterator.
         */
        WorkerList::iterator AddWorker( Worker* worker );
        /**
         * @brief Unregisters a Worker.
         *
         * @param[in] itr The worker's iterator.
         */
        void RemoveWorker( WorkerList::iterator& itr );

        /**
         * @brief Obtains a new target.
         *
         * @param[in] itr The worker's iterator.
         *
         * @return A Target to process.
         * @retval NULL A signal to quit.
         */
        Target* GetTarget( WorkerList::iterator& itr );

        /// A queue of pending targets.
        TargetQueue mTargets;
        /// A list of active workers.
        WorkerList mActiveWorkers;
        /// A list of inactive workers.
        WorkerList mInactiveWorkers;
        /// A Condition for workers.
        Condition mWorkerEvent;

        /// Thread limit.
        size_t mLimit;

        /// A Mutex to protect this object.
        mutable Mutex mMutex;
    };

    /// A macro for easier access.
    #define sThreadMgr \
        ThreadMgr::get()

    /**
     * @brief A ThreadMgr's worker thread.
     *
     * This class assumes it's dynamically allocated,
     * owned and run by only a single thread.
     *
     * @author Bloody.Rabbit
     */
    class ThreadMgr::Worker
    : public Mt::Worker
    {
    public:
        /// Primary constructor.
        Worker( ThreadMgr* mgr );
        /// A destructor.
        ~Worker();

        /**
         * @brief Assigns a new manager.
         *
         * @param[in] mgr The new manager.
         */
        void AssignMgr( ThreadMgr* mgr );

    protected:
        /// @see Target::deleteOnExit().
        bool deleteOnExit() { return true; }
        /// @see Mt::Worker::GetTarget().
        Target* GetTarget();

        /// The bound thread manager.
        ThreadMgr* mMgr;
        /// Our iterator.
        WorkerList::iterator mItr;

        /// A Mutex to protect this object.
        Mutex mMutex;
    };
}

#endif /* !__MT__THREAD_MGR_H__INCL__ */
