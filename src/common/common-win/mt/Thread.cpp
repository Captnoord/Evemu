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

/*************************************************************************/
/* Mt::Thread                                                            */
/*************************************************************************/
DWORD WINAPI ThreadMain( PVOID arg )
{
    Mt::Target::Run( reinterpret_cast< Mt::Target* >( arg ) );

    return 0;
}

Mt::Thread Mt::Thread::self()
{
    return Mt::Thread( new Win::Thread( Win::Thread::self() ) );
}

void Mt::Thread::Sleep( const Time::Msec& period )
{
    Win::Thread::Sleep( period );
}

Mt::Thread::Thread()
: mThread( new Win::Thread )
{
}

Mt::Thread::Thread( Mt::Target* target )
: mThread( new Win::Thread )
{
    Create( target );
}

Mt::Thread::Thread( const Mt::Thread& oth )
: mThread( new Win::Thread( *oth.mThread ) )
{
}

Mt::Thread::Thread( Win::Thread* thread )
: mThread( thread )
{
}

Mt::Thread::~Thread()
{
    SafeDelete( mThread );
}

unsigned int Mt::Thread::id() const
{
    return static_cast< unsigned int >( mThread->id() );
}

void Mt::Thread::Wait() const
{
    DWORD code = mThread->Wait();
    assert( ERROR_SUCCESS == code );
}

bool Mt::Thread::operator==( const Mt::Thread& oth ) const
{
    return TRUE == ( *mThread == *oth.mThread );
}

void Mt::Thread::Create( Mt::Target* target )
{
    DWORD code = mThread->Create( ThreadMain, target );
    assert( ERROR_SUCCESS == code );
}

void Mt::Thread::Terminate()
{
    DWORD code = mThread->Terminate( -1 );
    assert( ERROR_SUCCESS == code );
}

Mt::Thread& Mt::Thread::operator=( const Mt::Thread& oth )
{
    *mThread = *oth.mThread;
    return *this;
}
