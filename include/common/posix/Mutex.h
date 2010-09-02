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

#ifndef __POSIX__MUTEX_H__INCL__
#define __POSIX__MUTEX_H__INCL__

namespace Posix
{
    /**
     * @brief Wrapper around <code>pthread_mutex_t</code>.
     *
     * @author Bloody.Rabbit
     */
    class Mutex
    {
        friend class Condition;

    public:
        class Attribute;

        /// The default attribute of new mutexes.
        static const Attribute DEFAULT_ATTRIBUTE;

        /**
         * @brief The primary constructor.
         *
         * @param[in] attr The mutex attribute to use.
         */
        Mutex( const Attribute& attr = DEFAULT_ATTRIBUTE );
        /// A destructor.
        ~Mutex();

        /**
         * @brief Locks the mutex.
         *
         * Blocks until the mutex is successfully locked or
         * an error is encountered.
         *
         * @return A value returned by <code>pthread_mutex_lock</code>.
         */
        int Lock();
        /**
         * @brief Tries to lock the mutex.
         *
         * Returns immediately; the return value indicates
         * whether the mutex has been locked or not.
         *
         * @return A value returned by <code>pthread_mutex_trylock</code>.
         */
        int TryLock();

        /**
         * @brief Unlocks the mutex.
         *
         * @return A value returned by <code>pthread_mutex_unlock</code>.
         */
        int Unlock();

    protected:
        /// The mutex itself.
        pthread_mutex_t mMutex;
    };
}

#endif /* !__POSIX__MUTEX_H__INCL__ */
