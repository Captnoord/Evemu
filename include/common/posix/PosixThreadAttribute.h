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

#ifndef __POSIX__POSIX_THREAD_ATTRIBUTE_H__INCL__
#define __POSIX__POSIX_THREAD_ATTRIBUTE_H__INCL__

#include "posix/PosixThread.h"

/**
 * @brief Wrapper around @c pthread_attr_t.
 *
 * @author Bloody.Rabbit
 */
class PosixThread::Attribute
{
    friend class PosixThread;

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
     * @return A value returned by @c pthread_attr_getdetachstate.
     */
    int GetDetachState( int* detachState ) const;
    /**
     * @brief Sets the detach state.
     *
     * @param[in] detachState A detach state to set.
     *
     * @return A value returned by @c pthread_attr_setdetachstate.
     */
    int SetDetachState( int detachState );

    /**
     * @brief Obtains the guard size.
     *
     * @param[out] guardSize A variable which receives the value.
     *
     * @return A value returned by @c pthread_attr_getguardsize.
     */
    int GetGuardSize( size_t* guardSize ) const;
    /**
     * @brief Sets the guard size.
     *
     * @param[in] guardSize A guard size to set.
     *
     * @return A value returned by @c pthread_attr_setguardsize.
     */
    int SetGuardSize( size_t guardSize );

    /**
     * @brief Obtains the scheduling inheritance.
     *
     * @param[out] inheritSched A variable which receives the value.
     *
     * @return A value returned by @c pthread_attr_getinheritsched.
     */
    int GetInheritSched( int* inheritSched ) const;
    /**
     * @brief Sets the scheduling inheritance.
     *
     * @param[in] inheritSched A scheduling inheritance to set.
     *
     * @return A value returned by @c pthread_attr_setinheritsched.
     */
    int SetInheritSched( int inheritSched );

    /**
     * @brief Obtains the scheduling param.
     *
     * @param[out] schedParam A variable which receives the value.
     *
     * @return A value returned by @c pthread_attr_getschedparam.
     */
    int GetSchedParam( sched_param* schedParam ) const;
    /**
     * @brief Sets the scheduling param.
     *
     * @param[in] schedParam A scheduling param to set.
     *
     * @return A value returned by @c pthread_attr_setschedparam.
     */
    int SetSchedParam( const sched_param* schedParam );

    /**
     * @brief Obtains the scheduling policy.
     *
     * @param[out] schedPolicy A variable which receives the value.
     *
     * @return A value returned by @c pthread_attr_getschedpolicy.
     */
    int GetSchedPolicy( int* schedPolicy ) const;
    /**
     * @brief Sets the scheduling policy.
     *
     * @param[in] schedPolicy A scheduling policy to set.
     *
     * @return A value returned by @c pthread_attr_setschedpolicy.
     */
    int SetSchedPolicy( int schedPolicy );

    /**
     * @brief Obtains the scope.
     *
     * @param[out] scope A variable which receives the value.
     *
     * @return A value returned by @c pthread_attr_getscope.
     */
    int GetScope( int* scope ) const;
    /**
     * @brief Sets the scope.
     *
     * @param[in] scope A scope to set.
     *
     * @return A value returned by @c pthread_attr_setscope.
     */
    int SetScope( int scope );

    /**
     * @brief Obtains the stack address and size.
     *
     * @param[out] stackAddr A variable which receives the value.
     * @param[out] stackSize A variable which receives the value.
     *
     * @return A value returned by @c pthread_attr_getstack.
     */
    int GetStack( void** stackAddr, size_t* stackSize ) const;
    /**
     * @brief Sets the stack address and size.
     *
     * @param[in] stackAddr A stack address to set.
     * @param[in] stackSize A stack size to set.
     *
     * @return A value returned by @c pthread_attr_setstack.
     */
    int SetStack( void* stackAddr, size_t stackSize );

    /**
     * @brief Obtains the stack size.
     *
     * @param[out] stackSize A variable which receives the value.
     *
     * @return A value returned by @c pthread_attr_getstacksize.
     */
    int GetStackSize( size_t* stackSize ) const;
    /**
     * @brief Sets the stack size.
     *
     * @param[in] stackSize The stack size to set.
     *
     * @return A value returned by @c pthread_attr_setstacksize.
     */
    int SetStackSize( size_t stackSize );

protected:
    /// The thread attribute itself.
    pthread_attr_t mAttribute;
};

#endif /* !__POSIX__POSIX_THREAD_ATTRIBUTE_H__INCL__ */
