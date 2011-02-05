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

#include "CommonWin.h"

#include "win/Thread.h"

using namespace common;
using namespace common::mt;

/*************************************************************************/
/* common::mt::Thread                                                    */
/*************************************************************************/
DWORD WINAPI ThreadMain( PVOID arg )
{
    Target::Run( reinterpret_cast< Target* >( arg ) );

    return 0;
}

Thread Thread::self()
{
    return Thread( new win::Thread( win::Thread::self() ) );
}

void Thread::Sleep( const time::Msec& period )
{
    win::Thread::Sleep( period );
}

Thread::Thread()
: mThread( new win::Thread )
{
}

Thread::Thread( Target* target )
: mThread( new win::Thread )
{
    Create( target );
}

Thread::Thread( const Thread& oth )
: mThread( new win::Thread( *oth.mThread ) )
{
}

Thread::Thread( win::Thread* thread )
: mThread( thread )
{
}

Thread::~Thread()
{
    SafeDelete( mThread );
}

unsigned int Thread::id() const
{
    return static_cast< unsigned int >( mThread->id() );
}

void Thread::Wait() const
{
    DWORD code = mThread->Wait();
    assert( ERROR_SUCCESS == code );
}

bool Thread::operator==( const Thread& oth ) const
{
    return TRUE == ( *mThread == *oth.mThread );
}

void Thread::Create( Target* target )
{
    DWORD code = mThread->Create( ThreadMain, target );
    assert( ERROR_SUCCESS == code );
}

void Thread::Terminate()
{
    DWORD code = mThread->Terminate( -1 );
    assert( ERROR_SUCCESS == code );
}

Thread& Thread::operator=( const Thread& oth )
{
    *mThread = *oth.mThread;
    return *this;
}
