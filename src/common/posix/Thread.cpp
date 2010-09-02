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

#include "posix/Thread.h"
#include "posix/ThreadAttribute.h"

/*************************************************************************/
/* Posix::Thread                                                         */
/*************************************************************************/
const Posix::Thread::Attribute Posix::Thread::DEFAULT_ATTRIBUTE;

Posix::Thread Posix::Thread::self()
{
    return Posix::Thread( ::pthread_self() );
}

int Posix::Thread::GetConcurrency()
{
    return ::pthread_getconcurrency();
}

int Posix::Thread::SetConcurrency( int level )
{
    return ::pthread_setconcurrency( level );
}

int Posix::Thread::SetCancelState( int state, int* oldState )
{
    return ::pthread_setcancelstate( state, oldState );
}

int Posix::Thread::SetCancelType( int type, int* oldType )
{
    return ::pthread_setcanceltype( type, oldType );
}

void Posix::Thread::TestCancel()
{
    ::pthread_testcancel();
}

int Posix::Thread::Sleep( useconds_t microseconds )
{
    if( 0 < microseconds )
        return ::usleep( microseconds );
    else
        return 0;
}

Posix::Thread::Thread()
{
}

Posix::Thread::Thread( pthread_t thread )
: mThread( thread )
{
}

Posix::Thread::Thread( void* ( *startRoutine )( void* ), void* arg, const Attribute& attr )
{
    int code;

    code = Create( startRoutine, arg, attr );
    assert( 0 == code );
}

Posix::Thread::Thread( const Posix::Thread& oth )
{
    // let the copy constructor do the job
    *this = oth;
}

int Posix::Thread::Join( void** retVal ) const
{
    return ::pthread_join( mThread, retVal );
}

bool Posix::Thread::operator==( const Posix::Thread& oth ) const
{
    return 0 != ::pthread_equal( mThread, oth.mThread );
}

int Posix::Thread::Create( void* ( *startRoutine )( void* ), void* arg, const Attribute& attr )
{
    return ::pthread_create( &mThread, &attr.mAttribute, startRoutine, arg );
}

int Posix::Thread::Cancel()
{
    return ::pthread_cancel( mThread );
}

int Posix::Thread::Detach()
{
    return ::pthread_detach( mThread );
}

int Posix::Thread::GetSchedPolicyParam( int* policy, sched_param* schedParam ) const
{
    return ::pthread_getschedparam( mThread, policy, schedParam );
}

int Posix::Thread::SetSchedPolicyParam( int policy, const sched_param* schedParam )
{
    return ::pthread_setschedparam( mThread, policy, schedParam );
}

Posix::Thread& Posix::Thread::operator=( const Posix::Thread& oth )
{
    mThread = oth.mThread;
}
