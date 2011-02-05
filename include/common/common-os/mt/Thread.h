/**
 * @file mt/Thread.h
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

#ifndef __COMMON__MT__THREAD_H__INCL__
#define __COMMON__MT__THREAD_H__INCL__

#include "mt/Target.h"
#include "time/Msec.h"

#ifdef WIN32
#   include "win/Thread.h"
#else /* !WIN32 */
#   include "posix/Thread.h"
#endif /* !WIN32 */

namespace common
{
    namespace mt
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
            static void Sleep( const time::Msec& period );

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
            unsigned int id() const;

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
            bool operator==( const Thread& oth ) const;

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
            Thread& operator=( const Thread& oth );

        protected:
#       ifdef WIN32
            /**
             * @brief A protected constructor.
             *
             * @param[in] thread The Windows thread.
             */
            Thread( const win::Thread& thread );

            /// Windows thread.
            win::Thread mThread;

            /// The entry point for new Windows threads.
            static DWORD WINAPI ThreadMain( PVOID arg );
#       else /* !WIN32 */
            /**
             * @brief A protected constructor.
             *
             * @param[in] thread The Posix thread.
             */
            Thread( const posix::Thread& thread );

            /// Posix thread.
            posix::Thread mThread;

            /// The entry point for new Posix threads.
            static void* ThreadMain( void* arg );
#       endif /* !WIN32 */
        };
    }
}

#endif /* !__COMMON__MT__THREAD_H__INCL__ */
