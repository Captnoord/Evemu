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
#include "mt/Target.h"
#include "mt/Thread.h"
#include "util/Singleton.h"

namespace Mt
{
    /**
     * @brief Thread manager, does thread pooling.
     *
     * @author Bloody.Rabbit
     */
    class ThreadMgr
    : public TargetEx,
      public Util::Singleton< ThreadMgr >
    {
    public:
        /**
         * @brief A primary constructor.
         *
         * @param[in] limit Thread count limit.
         */
        ThreadMgr( size_t limit = 100 );
        /// A primary destructor.
        ~ThreadMgr();

        /// Obtains current count of enqueued targets.
        size_t queueLen() const { return mQueuedTargets.size(); }
        /// Obtains current thread count.
        size_t threadCount() const { return mThreads.size(); }
        /// Obtains current active thread count.
        size_t activeThreadCount() const { return mActiveTargets.size(); }

        /// Obtains current thread count limit.
        size_t threadLimit() const { return mLimit; }

        /**
         * @brief Adds a new target to the processing queue.
         *
         * @param[in] target The new target.
         */
        void Run( TargetEx* target );
        /// Immediately stops all processing.
        void Stop();

        /**
         * @brief Sets new thread count limit.
         *
         * @param[in] limit The new limit.
         */
        void SetThreadLimit( size_t limit );

    protected:
        /// Singleton cares about our destruction, always return <code>false</code>.
        bool deleteOnExit() { return false; }
        /// The processing loop.
        void Run();

        /// A queue of pending targets.
        std::queue< TargetEx* > mQueuedTargets;
        /// A list of our threads.
        std::list< Thread > mThreads;
        /// A list of active targets.
        std::list< TargetEx* > mActiveTargets;

        /// A wait condition for threads.
        Condition mEvent;

        /// Thread limit.
        size_t mLimit;

        /// A Mutex to protect this object.
        mutable Mutex mMutex;
    };
}

/// A macro for easier access.
#define sThreadMgr \
    ( Mt::ThreadMgr::get() )

#endif /* !__MT__THREAD_MGR_H__INCL__ */
