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

#ifndef __MT__MUTEX_H__INCL__
#define __MT__MUTEX_H__INCL__

#include "util/Lock.h"

#ifndef MT_MUTEX_TYPE
#   define MT_MUTEX_TYPE void
#endif /* !MT_MUTEX_TYPE */

/**
 * @brief Classes and utilities to manage multithreading.
 */
namespace Mt
{
    /**
     * @brief Common wrapper for platform-specific mutexes.
     *
     * @author Zhur, Bloody.Rabbit
     */
    class Mutex
    : public Util::Lockable
    {
        friend class Condition;

    public:
        /**
         * @brief A default constructor, initializes the object.
         */
        Mutex();
        /**
         * @brief A destructor, cleans the object up.
         */
        ~Mutex();

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
        /// The implementing object.
        MT_MUTEX_TYPE* mMutex;
    };

    /// Convenience typedef for Mutex lock.
    typedef Util::Lock< Mutex > MutexLock;
}

#endif /* !__MT__MUTEX_H__INCL__ */
