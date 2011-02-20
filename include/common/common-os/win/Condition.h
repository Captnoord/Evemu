/**
 * @file win/Condition.h
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

#ifndef __COMMON__WIN__CONDITION_H__INCL__
#define __COMMON__WIN__CONDITION_H__INCL__

#include "mt/Mutex.h"
#include "win/Event.h"

namespace common
{
    namespace win
    {
        /**
         * @brief Implementation of condition variable for Windows.
         *
         * @author Bloody.Rabbit
         */
        class Condition
        {
        public:
            /**
             * @brief A primary constructor.
             */
            Condition();

            /**
             * @brief Signals the condition variable.
             *
             * Signaling the condition makes exactly one
             * waiting thread wake up. If there is none,
             * the signal has no effect.
             *
             * @return An error code.
             */
            DWORD Signal();
            /**
             * @brief Broadcasts the condition variable.
             *
             * Broadcasting the condition makes all waiting
             * threads wake up. If there are none, the
             * broadcast has no effect.
             *
             * @return An error code.
             */
            DWORD Broadcast();

            /**
             * @brief Waits on the condition variable.
             *
             * @param[in] criticalSection The critical section this
             *                            condition is bound to.
             * @param[in] timeout         The wait timeout.
             *
             * @return An error code.
             */
            DWORD Wait( CriticalSection& criticalSection, const time::Msec& timeout = INFINITE );

        protected:
            /// The event for waiting threads.
            Event mWaitEvent;

            /// Current count of waiting threads.
            UINT mCurrentCount;
            /// Count of threads to free.
            UINT mToFreeCount;

            /// A mutex to protect this whole stuff.
            mt::Mutex mMutex;
        };
    }
}

#endif /* !__COMMON__WIN__CONDITION_H__INCL__ */
