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

#include "CommonPosix.h"

#include "posix/Thread.h"

using namespace common;
using namespace common::mt;

/*************************************************************************/
/* common::mt::Thread                                                    */
/*************************************************************************/
void* ThreadMain( void* arg )
{
    /* Setup correct cancelability state and type so that
       cancel works as desired. */
    int code = posix::Thread::SetCancelState( PTHREAD_CANCEL_ENABLE, NULL );
    assert( 0 == code );

    code = posix::Thread::SetCancelType( PTHREAD_CANCEL_ASYNCHRONOUS, NULL );
    assert( 0 == code );

    Target::Run( reinterpret_cast< Target* >( arg ) );

    return NULL;
}

Thread Thread::self()
{
    return Thread( new posix::Thread( posix::Thread::self() ) );
}

void Thread::Sleep( const time::Msec& period )
{
    int code = posix::Thread::Sleep( period );
    assert( 0 == code );
}

Thread::Thread()
: mThread( new posix::Thread )
{
}

Thread::Thread( Target* target )
: mThread( new posix::Thread )
{
    Create( target );
}

Thread::Thread( const Thread& oth )
: mThread( new posix::Thread( *oth.mThread ) )
{
}

Thread::Thread( posix::Thread* thread )
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
    int code = mThread->Join( NULL );
    assert( 0 == code );
}

bool Thread::operator==( const Thread& oth ) const
{
    return *mThread == *oth.mThread;
}

void Thread::Create( Target* target )
{
    int code = mThread->Create( ThreadMain, target );
    assert( 0 == code );
}

void Thread::Terminate()
{
    int code = mThread->Cancel();
    assert( 0 == code );
}

Thread& Thread::operator=( const Thread& oth )
{
    *mThread = *oth.mThread;
    return *this;
}
