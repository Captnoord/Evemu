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

#ifndef __POSIX__THREAD_H__INCL__
#define __POSIX__THREAD_H__INCL__

namespace Posix
{
    /**
     * @brief Wrapper around <code>pthread_t</code>.
     *
     * @author Bloody.Rabbit
     */
    class Thread
    {
    public:
        class Attribute;

        /// The default attribute of new threads.
        static const Attribute DEFAULT_ATTRIBUTE;

        /**
         * @brief Obtains a Thread object for calling thread itself.
         *
         * @return A Thread object for the calling thread.
         */
        static Thread self();

        /**
         * @brief Obtains a current concurrency level.
         *
         * @return A value returned by <code>pthread_getconcurrency</code>.
         */
        static int GetConcurrency();
        /**
         * @brief Sets a concurrency level.
         *
         * @param[in] level The concurrency level to set.
         *
         * @return A value returned by <code>pthread_setconcurrency</code>.
         */
        static int SetConcurrency( int level );

        /**
         * @brief Sets the calling thread's cancelability state.
         *
         * @param[in]  state    The cancelability state to set.
         * @param[out] oldState The old cancelability state.
         *
         * @return A value returned by <code>pthread_setcancelstate</code>.
         */
        static int SetCancelState( int state, int* oldState );
        /**
         * @brief Sets the calling thread's cancelability type.
         *
         * @param[in]  type    The cancelability type to set.
         * @param[out] oldType The old cancelability type.
         *
         * @return A value returned by <code>pthread_setcanceltype</code>.
         */
        static int SetCancelType( int type, int* oldType );
        /// Creates a cancellation point in the calling thread.
        static void TestCancel();

        /**
         * @brief Makes the caller thread sleep for specified amount of time.
         *
         * @param[in] microseconds Amount of microseconds to sleep.
         *
         * @return A value returned by <code>usleep</code>.
         */
        static int Sleep( useconds_t microseconds );

        /// A default constructor.
        Thread();
        /**
         * @brief A primary constructor.
         *
         * @param[in] startRoutine An address of routine where the thread should start.
         * @param[in] arg          An argument to pass to start routine.
         * @param[in] attr         An attribute of new thread.
         */
        Thread( void* ( *startRoutine )( void* ), void* arg, const Attribute& attr = DEFAULT_ATTRIBUTE );
        /**
         * @brief A copy constructor; copies the thread handle.
         *
         * @param[in] oth The thread handle to copy.
         */
        Thread( const Thread& oth );

        /**
         * @brief Obtains an ID of the thread.
         *
         * @return An ID of the thread.
         */
        int id() const { return static_cast< int >( mThread ); }

        /**
         * @brief Joins the thread.
         *
         * @param[out] retVal A variable which receives value returned by the thread.
         *
         * @return A value returned by <code>pthread_join</code>.
         */
        int Join( void** retVal ) const;

        /**
         * @brief A compare operator; compares two thread handles.
         *
         * @param[in] oth The other thread handle in comparison.
         *
         * @retval true  The handles are equal.
         * @retval false The handles are not equal.
         */
        bool operator==( const Thread& oth ) const;

        /**
         * @brief Creates a new thread.
         *
         * A handle to the new thread is stored in this object;
         * the old one is forgotten.
         *
         * @param[in] startRoutine An address of routine where the thread should start.
         * @param[in] arg          An argument to pass to start routine.
         * @param[in] attr         An attribute of new thread.
         *
         * @return A value returned by <code>pthread_create</code>.
         */
        int Create( void* ( *startRoutine )( void* ), void* arg, const Attribute& attr = DEFAULT_ATTRIBUTE );
        /**
         * @brief Cancels the thread.
         *
         * @return A value returned by <code>pthread_cancel</code>.
         */
        int Cancel();
        /**
         * @brief Detaches the thread.
         *
         * @return A value returned by <code>pthread_detach</code>.
         */
        int Detach();

        /**
         * @brief Obtains the scheduling policy and param.
         *
         * @param[out] schedPolicy A variable which receives the value.
         * @param[out] schedParam  A variable which receives the value.
         *
         * @return A value returned by <code>pthread_getschedparam</code>.
         */
        int GetSchedPolicyParam( int* schedPolicy, sched_param* schedParam ) const;
        /**
         * @brief Sets the scheduling policy and param.
         *
         * @param[in] schedPolicy A scheduling policy to set.
         * @param[in] schedParam  A scheduling param to set.
         *
         * @return A value returned by <code>pthread_setschedparam</code>.
         */
        int SetSchedPolicyParam( int schedPolicy, const sched_param* schedParam );

        /**
         * @brief A copy operator; copies the thread handle.
         *
         * @param[in] oth The thread handle to copy.
         *
         * @return Itself.
         */
        Thread& operator=( const Thread& oth );

    protected:
        /**
         * @brief A protected constructor.
         *
         * @param[in] thread A thread handle.
         */
        Thread( pthread_t thread );

        /// The thread handle itself.
        pthread_t mThread;
    };
}

#endif /* !__POSIX__THREAD_H__INCL__ */
