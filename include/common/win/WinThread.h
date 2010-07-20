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

#ifndef __WIN__WIN_THREAD_H__INCL__
#define __WIN__WIN_THREAD_H__INCL__

#include "win/WinHandle.h"

/**
 * @brief A handle to a thread.
 *
 * @author Bloody.Rabbit
 */
class WinThread
: public WinWaitableHandle
{
public:
    /**
     * @brief Obtains a WinThread handle for the calling thread itself.
     *
     * @return A WinThread handle for the calling thread.
     */
    static WinThread self();

    /**
     * @brief Makes caller thread sleep for specified amount of time.
     *
     * @param[in] milliseconds Number of milliseconds to sleep.
     */
    static VOID Sleep( DWORD milliseconds );

    /**
     * @brief Primary constructor, creates a new thread.
     *
     * @param[in] startAddress Address of method where the thread should start.
     * @param[in] param        Parameter to pass to start method.
     * @param[in] stackSize    The size of stack of the new thread.
     */
    WinThread( PTHREAD_START_ROUTINE startAddress, PVOID param, SIZE_T stackSize = 0 );
    /**
     * @brief Copy constructor, duplicates the thread handle.
     *
     * @param[in] oth The handle to duplicate.
     */
    WinThread( const WinThread& oth );

    /**
     * @brief Obtains a thread ID.
     *
     * @return A thread ID.
     */
    DWORD id() const { return mThreadID; }

    /**
     * @brief Obtains an exit code of the thread.
     *
     * @param[out] exitCode A variable which receives the value.
     *
     * @return A value returned by @c GetExitCodeThread.
     */
    BOOL GetExitCode( PDWORD exitCode ) const;

    /**
     * @brief Creates a new thread.
     *
     * The handle to the new thread is stored in this object;
     * the old one is forgotten.
     *
     * @param[in] startAddress Address of method where the thread should start.
     * @param[in] param        Parameter to pass to start method.
     * @param[in] stackSize    The size of stack of the new thread.
     *
     * @retval TRUE  Creation succeeded.
     * @retval FALSE Creation failed.
     */
    BOOL Create( PTHREAD_START_ROUTINE startAddress, PVOID param, SIZE_T stackSize = 0 );
    /**
     * @brief Terminates the thread.
     *
     * @param[in] exitCode An exit code for the thread.
     *
     * @return A value returned by @c TerminateThread.
     */
    BOOL Terminate( DWORD exitCode );

    /**
     * @brief Suspends the thread.
     *
     * @return A value returned by @c SuspendThread.
     */
    DWORD Suspend();
    /**
     * @brief Resumes a thread.
     *
     * @return A value returned by @c ResumeThread.
     */
    DWORD Resume();

    /**
     * @brief Sets the thread's priority.
     *
     * @param[in] priority The priority to set.
     *
     * @return A value returned by @c SetThreadPriority.
     */
    BOOL SetPriority( int priority );

    /**
     * @brief Copy operator; duplicates the thread handle.
     *
     * @param[in] oth The thread handle to duplicate.
     *
     * @return Itself.
     */
    WinThread& operator=( const WinThread& oth );

protected:
    /**
     * @brief A protected constructor.
     *
     * @param[in] handle A thread handle.
     * @param[in] id     A thread ID.
     */
    WinThread( HANDLE handle, DWORD id );

    /// A thread ID.
    DWORD mThreadID;
};

#endif /* !__WIN__WIN_THREAD_H__INCL__ */
