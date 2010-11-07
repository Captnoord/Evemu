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

#include "time/Timeval.h"

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
        /**
         * @brief Creates a cancellation point in the calling thread.
         */
        static void TestCancel();

        /**
         * @brief Makes the caller thread sleep for specified amount of time.
         *
         * @param[in] period How much time to spend sleeping.
         *
         * @return An error code.
         */
        static int Sleep( const Time::Timeval& period );

        /**
         * @brief A default constructor.
         */
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

    /**
     * @brief Wrapper around <code>pthread_attr_t</code>.
     *
     * @author Bloody.Rabbit
     */
    class Thread::Attribute
    {
        friend class Thread;

    public:
        /**
         * @brief A default constructor.
         */
        Attribute();
        /**
         * @brief A destructor.
         */
        ~Attribute();

        /**
         * @brief Obtains the detach state.
         *
         * @param[out] detachState A variable which receives the value.
         *
         * @return A value returned by <code>pthread_attr_getdetachstate</code>.
         */
        int GetDetachState( int* detachState ) const;
        /**
         * @brief Sets the detach state.
         *
         * @param[in] detachState A detach state to set.
         *
         * @return A value returned by <code>pthread_attr_setdetachstate</code>.
         */
        int SetDetachState( int detachState );

        /**
         * @brief Obtains the guard size.
         *
         * @param[out] guardSize A variable which receives the value.
         *
         * @return A value returned by <code>pthread_attr_getguardsize</code>.
         */
        int GetGuardSize( size_t* guardSize ) const;
        /**
         * @brief Sets the guard size.
         *
         * @param[in] guardSize A guard size to set.
         *
         * @return A value returned by <code>pthread_attr_setguardsize</code>.
         */
        int SetGuardSize( size_t guardSize );

        /**
         * @brief Obtains the scheduling inheritance.
         *
         * @param[out] inheritSched A variable which receives the value.
         *
         * @return A value returned by <code>pthread_attr_getinheritsched</code>.
         */
        int GetInheritSched( int* inheritSched ) const;
        /**
         * @brief Sets the scheduling inheritance.
         *
         * @param[in] inheritSched A scheduling inheritance to set.
         *
         * @return A value returned by <code>pthread_attr_setinheritsched</code>.
         */
        int SetInheritSched( int inheritSched );

        /**
         * @brief Obtains the scheduling param.
         *
         * @param[out] schedParam A variable which receives the value.
         *
         * @return A value returned by <code>pthread_attr_getschedparam</code>.
         */
        int GetSchedParam( sched_param* schedParam ) const;
        /**
         * @brief Sets the scheduling param.
         *
         * @param[in] schedParam A scheduling param to set.
         *
         * @return A value returned by <code>pthread_attr_setschedparam</code>.
         */
        int SetSchedParam( const sched_param* schedParam );

        /**
         * @brief Obtains the scheduling policy.
         *
         * @param[out] schedPolicy A variable which receives the value.
         *
         * @return A value returned by <code>pthread_attr_getschedpolicy</code>.
         */
        int GetSchedPolicy( int* schedPolicy ) const;
        /**
         * @brief Sets the scheduling policy.
         *
         * @param[in] schedPolicy A scheduling policy to set.
         *
         * @return A value returned by <code>pthread_attr_setschedpolicy</code>.
         */
        int SetSchedPolicy( int schedPolicy );

        /**
         * @brief Obtains the scope.
         *
         * @param[out] scope A variable which receives the value.
         *
         * @return A value returned by <code>pthread_attr_getscope</code>.
         */
        int GetScope( int* scope ) const;
        /**
         * @brief Sets the scope.
         *
         * @param[in] scope A scope to set.
         *
         * @return A value returned by <code>pthread_attr_setscope</code>.
         */
        int SetScope( int scope );

        /**
         * @brief Obtains the stack address and size.
         *
         * @param[out] stackAddr A variable which receives the value.
         * @param[out] stackSize A variable which receives the value.
         *
         * @return A value returned by <code>pthread_attr_getstack</code>.
         */
        int GetStack( void** stackAddr, size_t* stackSize ) const;
        /**
         * @brief Sets the stack address and size.
         *
         * @param[in] stackAddr A stack address to set.
         * @param[in] stackSize A stack size to set.
         *
         * @return A value returned by <code>pthread_attr_setstack</code>.
         */
        int SetStack( void* stackAddr, size_t stackSize );

        /**
         * @brief Obtains the stack size.
         *
         * @param[out] stackSize A variable which receives the value.
         *
         * @return A value returned by <code>pthread_attr_getstacksize</code>.
         */
        int GetStackSize( size_t* stackSize ) const;
        /**
         * @brief Sets the stack size.
         *
         * @param[in] stackSize The stack size to set.
         *
         * @return A value returned by <code>pthread_attr_setstacksize</code>.
         */
        int SetStackSize( size_t stackSize );

    protected:
        /// The thread attribute itself.
        pthread_attr_t mAttribute;
    };
}

#endif /* !__POSIX__THREAD_H__INCL__ */
