/**
 * @file mt/Mutex.h
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

#ifndef __COMMON__MT__MUTEX_H__INCL__
#define __COMMON__MT__MUTEX_H__INCL__

#include "util/Lock.h"

#ifdef WIN32
#   include "win/Mutex.h"
#else /* !WIN32 */
#   include "posix/Mutex.h"
#endif /* !WIN32 */

namespace common
{
    /**
     * @brief Classes and utilities to manage multithreading.
     */
    namespace mt
    {
        /**
         * @brief Common wrapper for platform-specific mutexes.
         *
         * @author Zhur, Bloody.Rabbit
         */
        class Mutex
        : public util::Lockable
        {
            friend class Condition;

        public:
            /**
             * @brief Locks the mutex.
             */
            void Lock();
            /**
             * @brief Attempts to lock the mutex.
             *
             * @retval true  Mutex successfully locked.
             * @retval false Mutex locked by another thread.
             */
            bool TryLock();

            /**
             * @brief Unlocks the mutex.
             */
            void Unlock();

        protected:
#       ifdef WIN32
            /// Windows mutex.
            win::Mutex mMutex;
#       else /* !WIN32 */
            /// Posix mutex.
            posix::Mutex mMutex;
#       endif /* !WIN32 */
        };

        /// Convenience typedef for Mutex lock.
        typedef util::Lock< Mutex > MutexLock;
    }
}

#endif /* !__COMMON__MT__MUTEX_H__INCL__ */
