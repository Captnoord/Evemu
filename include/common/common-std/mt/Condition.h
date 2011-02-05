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

#ifndef __MT__CONDITION_H__INCL__
#define __MT__CONDITION_H__INCL__

#include "mt/Mutex.h"
#include "time/Msec.h"

#ifndef MT_CONDITION_TYPE
#   define MT_CONDITION_TYPE void
#endif /* !MT_CONDITION_TYPE */

namespace Mt
{
    /**
     * @brief A wrapper around platform-specific conditions.
     *
     * @author Bloody.Rabbit
     */
    class Condition
    {
    public:
        /**
         * @brief A default constructor, initializes the object.
         */
        Condition();
        /**
         * @brief A destructor, cleans the object up.
         */
        ~Condition();

        /**
         * @brief Signals the condition.
         */
        void Signal();
        /**
         * @brief Broadcasts the condition.
         */
        void Broadcast();

        /**
         * @brief Waits on the condition variable.
         *
         * @param[in] mutex The mutex this condition is bound to.
         */
        void Wait( Mutex& mutex );
        /**
         * @brief Waits on the condition variable with a timeout.
         *
         * @param[in] mutex   The mutex this condition is bound to.
         * @param[in] timeout The timeout.
         */
        void TimedWait( Mutex& mutex, const Time::Msec& timeout );

    protected:
        /// The implementing object.
        MT_CONDITION_TYPE* mCondition;
    };
}

#endif /* !__MT__CONDITION_H__INCL__ */
