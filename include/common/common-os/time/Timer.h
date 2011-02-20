/**
 * @file time/Timer.h
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

#ifndef __COMMON__TIME__TIMER_H__INCL__
#define __COMMON__TIME__TIMER_H__INCL__

#include "time/Msec.h"

namespace common
{
    namespace time
    {
        /**
         * @brief A simple timer.
         *
         * @author Bloody.Rabbit
         */
        class Timer
        {
        public:
            /**
             * @brief A primary constructor.
             *
             * @param[in] period   The timer period.
             * @param[in] accurate Use accurate timing.
             */
            Timer( const Msec& period, bool accurate = false );

            /**
             * @brief Obtains timer period.
             *
             * @return The period.
             */
            const Msec& period() const { return mPeriod; }
            /**
             * @brief Obtains accurate timing option.
             *
             * @retval true  Accurate timing is on.
             * @retval false Accurate timing is off.
             */
            bool accurate() const { return mAccurate; }

            /**
             * @brief Starts the timer.
             *
             * After starting the timer, you can check if the
             * <var>period</var> has passed by calling Check().
             *
             *   - if the timer is not accurate:
             *       It simply makes the Check() return <code>true</code>
             *       after exactly <var>period</var> from now.
             *   - if the timer is accurate:
             *       -# and this is the first call:
             *            The behavior is same as if the timer is not accurate.
             *       -# and this is some subsequent call:
             *            It takes the last timeout time and advances it into
             *            the future by <var>period</var>.
             */
            void Start();
            /**
             * @brief Checks the timer.
             *
             * Checks if the <var>period</var> has passed since the last
             * call of Start().
             *
             * @param[in] restart Pass <code>true</code> to have the timer
             *                    restarted when the <var>period</var> has passed.
             *                    It has the same effect as calling Start()
             *                    right after the Check() returns
             *                    <code>true</code>.
             *
             * @retval true  The period has already passed.
             * @retval false The period has not passed yet.
             */
            bool Check( bool restart = true );
            /**
             * @brief Suspends the calling thread.
             *
             * The calling thread is made to sleep until the
             * <var>period</var> passes.
             *
             * @param[in] restart Pass <code>true</code> to have the timer
             *                    restarted when the <var>period</var> has
             *                    passed. It has the same effect as calling
             *                    Start() right after the Sleep() returns.
             */
            void Sleep( bool restart = true );

        protected:
            /// The timeout.
            Msec mTimeout;

            /// The timer period.
            const Msec mPeriod;
            /// True if accurate.
            const bool mAccurate;
        };
    }
}

#endif /* !__COMMON__TIME__TIMER_H__INCL__ */
