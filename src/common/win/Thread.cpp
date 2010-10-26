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

#include "CommonPCH.h"

#include "win/Thread.h"

/*************************************************************************/
/* Win::Thread                                                           */
/*************************************************************************/
Win::Thread Win::Thread::self()
{
    return Win::Thread( ::GetCurrentThread(),
                        ::GetCurrentThreadId() );
}

VOID Win::Thread::Sleep( const Time::Msec& period )
{
    ::Sleep( static_cast< DWORD >( period.count() ) );
}

Win::Thread::Thread()
: Win::Handle(),
  Win::WaitableHandle(),
  mThreadId( 0 )
{
}

Win::Thread::Thread( HANDLE handle, DWORD id )
: Win::Handle( handle ),
  Win::WaitableHandle( handle ),
  mThreadId( id )
{
}

Win::Thread::Thread( PTHREAD_START_ROUTINE startAddress, PVOID param, SIZE_T stackSize )
: Win::Handle(),
  Win::WaitableHandle(),
  mThreadId( 0 )
{
    BOOL success = Create( startAddress, param, stackSize );
    assert( TRUE == success );
}

Win::Thread::Thread( const Win::Thread& oth )
: Win::Handle( oth ),
  Win::WaitableHandle( oth ),
  mThreadId( oth.id() )
{
}

BOOL Win::Thread::GetExitCode( PDWORD exitCode ) const
{
    return ::GetExitCodeThread( mHandle, exitCode );
}

BOOL Win::Thread::Create( PTHREAD_START_ROUTINE startAddress, PVOID param, SIZE_T stackSize )
{
    BOOL success = CloseEx();
    assert( TRUE == success );

    mHandle = ::CreateThread( NULL, stackSize,
                              startAddress, param,
                              0, &mThreadId );
    return isValid();
}

BOOL Win::Thread::Terminate( DWORD exitCode )
{
    return ::TerminateThread( mHandle, exitCode );
}

DWORD Win::Thread::Suspend()
{
    return ::SuspendThread( mHandle );
}

DWORD Win::Thread::Resume()
{
    return ::ResumeThread( mHandle );
}

BOOL Win::Thread::SetPriority( int priority )
{
    return ::SetThreadPriority( mHandle, priority );
}

Win::Thread& Win::Thread::operator=( const Win::Thread& oth )
{
    static_cast< Win::WaitableHandle& >( *this ) = oth;
    mThreadId = oth.id();

    return *this;
}
