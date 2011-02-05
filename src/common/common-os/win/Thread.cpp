/**
 * @file win/Thread.cpp
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

#include "CommonWin.h"

#include "win/Thread.h"

using namespace common;
using namespace common::win;

/*************************************************************************/
/* common::win::Thread                                                   */
/*************************************************************************/
Thread Thread::self()
{
    return Thread( ::GetCurrentThread(),
                   ::GetCurrentThreadId() );
}

VOID Thread::Sleep( const time::Msec& period )
{
    ::Sleep( static_cast< DWORD >( period.count() ) );
}

Thread::Thread()
: Handle(),
  WaitableHandle(),
  mThreadId( 0 )
{
}

Thread::Thread( HANDLE handle, DWORD id )
: Handle( handle ),
  WaitableHandle( handle ),
  mThreadId( id )
{
}

Thread::Thread( PTHREAD_START_ROUTINE startAddress, PVOID param, SIZE_T stackSize )
: Handle(),
  WaitableHandle(),
  mThreadId( 0 )
{
    DWORD code = Create( startAddress, param, stackSize );
    assert( ERROR_SUCCESS == code );
}

Thread::Thread( const Thread& oth )
: Handle( oth ),
  WaitableHandle( oth ),
  mThreadId( oth.id() )
{
}

DWORD Thread::GetExitCode( PDWORD exitCode ) const
{
    if( TRUE != ::GetExitCodeThread( mHandle, exitCode ) )
        return ::GetLastError();

    return ERROR_SUCCESS;
}

DWORD Thread::Create( PTHREAD_START_ROUTINE startAddress, PVOID param, SIZE_T stackSize )
{
    DWORD code = Close();
    if( ERROR_SUCCESS != code )
        return code;

    mHandle = ::CreateThread( NULL, stackSize, startAddress, param, 0, &mThreadId );
    if( TRUE != isValid() )
        return ::GetLastError();

    return ERROR_SUCCESS;
}

DWORD Thread::Terminate( DWORD exitCode )
{
    if( TRUE != ::TerminateThread( mHandle, exitCode ) )
        return ::GetLastError();

    return ERROR_SUCCESS;
}

DWORD Thread::Suspend( PDWORD prevCount )
{
    DWORD count = ::SuspendThread( mHandle );
    if( -1 == count )
        return ::GetLastError();

    if( NULL != prevCount )
        *prevCount = count;

    return ERROR_SUCCESS;
}

DWORD Thread::Resume( PDWORD prevCount )
{
    DWORD count = ::ResumeThread( mHandle );
    if( -1 == count )
        return ::GetLastError();

    if( NULL != prevCount )
        *prevCount = count;

    return ERROR_SUCCESS;
}

DWORD Thread::SetPriority( int priority )
{
    if( TRUE != ::SetThreadPriority( mHandle, priority ) )
        return ::GetLastError();

    return ERROR_SUCCESS;
}

Thread& Thread::operator=( const Thread& oth )
{
    static_cast< WaitableHandle& >( *this ) = oth;
    mThreadId = oth.id();

    return *this;
}
