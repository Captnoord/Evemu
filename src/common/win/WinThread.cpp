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

#include "win/WinThread.h"

/*************************************************************************/
/* WinThread                                                       */
/*************************************************************************/
WinThread WinThread::self()
{
    return WinThread( ::GetCurrentThread(), ::GetCurrentThreadId() );
}

VOID WinThread::Sleep( DWORD milliseconds )
{
    if( 0 < milliseconds )
        ::Sleep( milliseconds );
}

WinThread::WinThread( PTHREAD_START_ROUTINE startAddress, PVOID param, SIZE_T stackSize )
{
    BOOL success;

    success = Create( startAddress, param, stackSize );
    assert( TRUE == success );
}

WinThread::WinThread( const WinThread& oth )
{
    // let the copy operator do the job
    *this = oth;
}

BOOL WinThread::GetExitCode( PDWORD exitCode ) const
{
    return ::GetExitCodeThread( mHandle, exitCode );
}

BOOL WinThread::Create( PTHREAD_START_ROUTE startAddress, PVOID param, SIZE_T stackSize )
{
    BOOL success;

    if( TRUE == isValid() )
    {
        success = Close();
        assert( TRUE == success );
    }

    mHandle = ::CreateThread( NULL, stackSize, startAddress, param, 0, &mThreadID );
    success = isValid();

    return success;
}

BOOL WinThread::Terminate( DWORD exitCode )
{
    return ::TerminateThread( mHandle, exitCode );
}

DWORD WinThread::Suspend()
{
    return ::SuspendThread( mHandle );
}

DWORD WinThread::Resume()
{
    return ::ResumeThread( mHandle );
}

BOOL WinThread::SetPriority( int priority )
{
    return ::SetThreadPriority( mHandle, priority );
}

WinThread& WinThread::operator=( const WinThread& oth )
{
    *(WinWaitableHandle*)this = oth;
    mThreadID = oth.mThreadID;

    return *this;
}
