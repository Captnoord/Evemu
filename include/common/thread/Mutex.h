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

#ifndef __THREAD__MUTEX_H__INCL__
#define __THREAD__MUTEX_H__INCL__

#include "utils/Lock.h"

#ifdef WIN32
#   include "win/WinCriticalSection.h"
#else /* !WIN32 */
#   include "posix/PosixMutex.h"
#endif /* !WIN32 */

/**
 * @brief Common wrapper for platform-specific mutexes.
 *
 * @author Zhur, Bloody.Rabbit
 */
class Mutex
: public Lockable
{
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
#ifdef WIN32
    /// A critical section used for mutex implementation on Windows.
    WinCriticalSection mCriticalSection;
#else
    /// A pthread mutex used for mutex implementation using pthread library.
    PosixMutex mMutex;
#endif
};

/// Convenience typedef for Mutex's lock.
typedef Lock< Mutex > MutexLock;

#endif /* !__THREAD__MUTEX_H__INCL__ */
