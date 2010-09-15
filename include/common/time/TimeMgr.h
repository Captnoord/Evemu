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

#ifndef __TIME__TIME_MGR_H__INCL__
#define __TIME__TIME_MGR_H__INCL__

#include "mt/Condition.h"
#include "mt/Target.h"
#include "std/Tm.h"
#include "time/Const.h"
#include "time/Timer.h"
#include "time/Timeval.h"
#include "time/WinTime.h"
#include "util/Singleton.h"

namespace Time
{
    /**
     * @brief Takes care of timing.
     *
     * @author Bloody.Rabbit
     */
    class TimeMgr
    : public Util::Singleton< TimeMgr >,
      protected Mt::TargetEx
    {
    public:
        /**
         * @brief A primary constructor.
         *
         * @param[in] period The update period.
         */
        TimeMgr( size_t period = 20 );

        /// Obtains the update period.
        size_t period() const { return mRunTimer.period(); }

        /**
         * @brief Obtains time passed in a human readable form.
         *
         * For details on what time this is, see Std::Tm or
         * documentation of <code>struct tm</code> from
         * Standard C library.
         *
         * @return The time.
         */
        Std::Tm nowTm() const;
        /**
         * @brief Obtains time passed since the Windows Epoch.
         *
         * The Windows epoch is the midnight on January 1, 1601.
         *
         * @return The time.
         */
        WinTime nowWin() const;
        /**
         * @brief Obtains time passed since the Unix Epoch.
         *
         * The Unix epoch is the time 00:00:00 UTC on 1 January 1970.
         *
         * @return The time.
         */
        Timeval nowUnix() const;

    protected:
        /// Never delete on exit.
        bool deleteOnExit() { return false; }

        /// Updates the stored time values.
        void Update();
        /// The main update loop.
        void Run();
        /// The stop method.
        void Stop() { mRun = false; }

#   ifdef WIN32
        /// "Now" in WinTime.
        WinTime mWinTime;
#   else /* !WIN32 */
        /// "Now" in Timeval.
        Timeval mTimeval;
#   endif /* !WIN32 */

        /// "Now" in Std::Tm.
        Std::Tm mTm;

        /// <code>true</code> if the thread should run.
        bool mRun;
        /// The update timer.
        Timer mRunTimer;

        /// A Mutex protection for this object.
        mutable Mt::Mutex mMutex;
    };
}

/// Evaluates to a TimeMgr instance.
#define sTimeMgr \
    ( Time::TimeMgr::get() )

#endif /* !__TIME__TIME_MGR_H__INCL__ */
