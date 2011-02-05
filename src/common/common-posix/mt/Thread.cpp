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

/*************************************************************************/
/* Mt::Thread                                                            */
/*************************************************************************/
void* ThreadMain( void* arg )
{
    /* Setup correct cancelability state and type so that
       cancel works as desired. */
    int code = Posix::Thread::SetCancelState( PTHREAD_CANCEL_ENABLE, NULL );
    assert( 0 == code );

    code = Posix::Thread::SetCancelType( PTHREAD_CANCEL_ASYNCHRONOUS, NULL );
    assert( 0 == code );

    Mt::Target::Run( reinterpret_cast< Mt::Target* >( arg ) );

    return NULL;
}

Mt::Thread Mt::Thread::self()
{
    return Mt::Thread( new Posix::Thread( Posix::Thread::self() ) );
}

void Mt::Thread::Sleep( const Time::Msec& period )
{
    int code = Posix::Thread::Sleep( period );
    assert( 0 == code );
}

Mt::Thread::Thread()
: mThread( new Posix::Thread )
{
}

Mt::Thread::Thread( Mt::Target* target )
: mThread( new Posix::Thread )
{
    Create( target );
}

Mt::Thread::Thread( const Mt::Thread& oth )
: mThread( new Posix::Thread( *oth.mThread ) )
{
}

Mt::Thread::Thread( Posix::Thread* thread )
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
    int code = mThread->Join( NULL );
    assert( 0 == code );
}

bool Mt::Thread::operator==( const Mt::Thread& oth ) const
{
    return *mThread == *oth.mThread;
}

void Mt::Thread::Create( Mt::Target* target )
{
    int code = mThread->Create( ThreadMain, target );
    assert( 0 == code );
}

void Mt::Thread::Terminate()
{
    int code = mThread->Cancel();
    assert( 0 == code );
}

Mt::Thread& Mt::Thread::operator=( const Mt::Thread& oth )
{
    *mThread = *oth.mThread;
    return *this;
}
