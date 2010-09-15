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

#ifndef __MT__THREAD_H__INCL__
#define __MT__THREAD_H__INCL__

#include "mt/Target.h"
#include "time/Msec.h"

#ifdef WIN32
#   include "win/Thread.h"
#else /* !WIN32 */
#   include "posix/Thread.h"
#endif /* !WIN32 */

namespace Mt
{
    /**
     * @brief A wrapper around platform-specific thread handles.
     *
     * @author Bloody.Rabbit
     */
    class Thread
    {
    public:
        /**
         * @brief Obtains a Thread object for the calling thread itself.
         *
         * @return A Thread object for the calling thread.
         */
        static Thread self();

        /**
         * @brief Makes the calling thread sleep.
         *
         * @param[in] period How long to sleep.
         */
        static void Sleep( const Time::Msec& period );

        /**
         * @brief A default constructor.
         */
        Thread();
        /**
         * @brief A primary constructor.
         *
         * @param[in] target A Target to process.
         */
        Thread( Target* target );
        /**
         * @brief A copy constructor; copies the handle.
         *
         * @param[in] oth The handle to copy.
         */
        Thread( const Thread& oth );

        /**
         * @brief Obtains a thread ID.
         *
         * @return A thread ID.
         */
        uint32 id() const { return static_cast< uint32 >( mThread.id() ); }

        /**
         * @brief Waits for the thread.
         */
        void Wait() const;

        /**
         * @brief Compares two handles for equality.
         *
         * @param[in] oth The other handle in comparison.
         *
         * @retval true  The handles are equal.
         * @retval false The handles are not equal.
         */
        bool operator==( const Thread& oth ) const { return oth.mThread == mThread; }

        /**
         * @brief Creates a new thread.
         *
         * The new thread's handle is stored in this object;
         * the old one is forgotten.
         *
         * @param[in] target A Target to process.
         */
        void Create( Target* target );
        /**
         * @brief Terminates the thread.
         */
        void Terminate();

        /**
         * @brief A copy operator; copies the handle.
         *
         * @param[in] oth The handle to copy.
         *
         * @return Itself.
         */
        Thread& operator=( const Thread& oth ) { mThread = oth.mThread; return *this; }

    protected:
#   ifdef WIN32
        /**
         * @brief A protected constructor.
         *
         * @param[in] thread A Win::Thread to use.
         */
        Thread( const Win::Thread& thread );

        /// Startup routine for Windows threads.
        static DWORD WINAPI ThreadMain( PVOID arg );

        /// A thread handle used on Windows.
        Win::Thread mThread;
#   else /* !WIN32 */
        /**
         * @brief A protected constructor.
         *
         * @param[in] thread A Posix::Thread to use.
         */
        Thread( const Posix::Thread& thread );

        /// Startup routine for POSIX threads.
        static void* ThreadMain( void* arg );

        /// A thread handle used with POSIX threads.
        Posix::Thread mThread;
#   endif /* !WIN32 */
    };
}

#endif /* !__MT__THREAD_H__INCL__ */
