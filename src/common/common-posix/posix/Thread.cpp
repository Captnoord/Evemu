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
using namespace common::posix;

/*************************************************************************/
/* common::posix::Thread                                                 */
/*************************************************************************/
const Thread::Attribute Thread::DEFAULT_ATTRIBUTE;

Thread Thread::self()
{
    return Thread( ::pthread_self() );
}

int Thread::GetConcurrency()
{
    return ::pthread_getconcurrency();
}

int Thread::SetConcurrency( int level )
{
    return ::pthread_setconcurrency( level );
}

int Thread::SetCancelState( int state, int* oldState )
{
    return ::pthread_setcancelstate( state, oldState );
}

int Thread::SetCancelType( int type, int* oldType )
{
    return ::pthread_setcanceltype( type, oldType );
}

void Thread::TestCancel()
{
    ::pthread_testcancel();
}

int Thread::Sleep( const time::Timeval& period )
{
    if( 0 != ::sleep( period.sec() ) )
        return -1;

    if( 0 != ::usleep( period.usec() ) )
        return errno;

    return 0;
}

Thread::Thread()
: mThread()
{
}

Thread::Thread( pthread_t thread )
: mThread( thread )
{
}

Thread::Thread( void* ( *startRoutine )( void* ), void* arg, const Attribute& attr )
: mThread()
{
    int code = Create( startRoutine, arg, attr );
    assert( 0 == code );
}

Thread::Thread( const Thread& oth )
: mThread( oth.mThread )
{
}

int Thread::Join( void** retVal ) const
{
    return ::pthread_join( mThread, retVal );
}

bool Thread::operator==( const Thread& oth ) const
{
    return 0 != ::pthread_equal( mThread, oth.mThread );
}

int Thread::Create( void* ( *startRoutine )( void* ), void* arg, const Attribute& attr )
{
    return ::pthread_create( &mThread, &attr.mAttribute, startRoutine, arg );
}

int Thread::Cancel()
{
    return ::pthread_cancel( mThread );
}

int Thread::Detach()
{
    return ::pthread_detach( mThread );
}

int Thread::GetSchedPolicyParam( int* policy, sched_param* schedParam ) const
{
    return ::pthread_getschedparam( mThread, policy, schedParam );
}

int Thread::SetSchedPolicyParam( int policy, const sched_param* schedParam )
{
    return ::pthread_setschedparam( mThread, policy, schedParam );
}

Thread& Thread::operator=( const Thread& oth )
{
    mThread = oth.mThread;

    return *this;
}

/*************************************************************************/
/* common::posix::Thread::Attribute                                      */
/*************************************************************************/
Thread::Attribute::Attribute()
{
    int code = ::pthread_attr_init( &mAttribute );
    assert( 0 == code );
}

Thread::Attribute::~Attribute()
{
    int code = ::pthread_attr_destroy( &mAttribute );
    assert( 0 == code );
}

int Thread::Attribute::GetDetachState( int* detachState ) const
{
    return ::pthread_attr_getdetachstate( &mAttribute, detachState );
}

int Thread::Attribute::SetDetachState( int detachState )
{
    return ::pthread_attr_setdetachstate( &mAttribute, detachState );
}

int Thread::Attribute::GetGuardSize( size_t* guardSize ) const
{
    return ::pthread_attr_getguardsize( &mAttribute, guardSize );
}

int Thread::Attribute::SetGuardSize( size_t guardSize )
{
    return ::pthread_attr_setguardsize( &mAttribute, guardSize );
}

int Thread::Attribute::GetInheritSched( int* inheritSched ) const
{
    return ::pthread_attr_getinheritsched( &mAttribute, inheritSched );
}

int Thread::Attribute::SetInheritSched( int inheritSched )
{
    return ::pthread_attr_setinheritsched( &mAttribute, inheritSched );
}

int Thread::Attribute::GetSchedParam( sched_param* schedParam ) const
{
    return ::pthread_attr_getschedparam( &mAttribute, schedParam );
}

int Thread::Attribute::SetSchedParam( const sched_param* schedParam )
{
    return ::pthread_attr_setschedparam( &mAttribute, schedParam );
}

int Thread::Attribute::GetSchedPolicy( int* schedPolicy ) const
{
    return ::pthread_attr_getschedpolicy( &mAttribute, schedPolicy );
}

int Thread::Attribute::SetSchedPolicy( int schedPolicy )
{
    return ::pthread_attr_setschedpolicy( &mAttribute, schedPolicy );
}

int Thread::Attribute::GetScope( int* scope ) const
{
    return ::pthread_attr_getscope( &mAttribute, scope );
}

int Thread::Attribute::SetScope( int scope )
{
    return ::pthread_attr_setscope( &mAttribute, scope );
}

int Thread::Attribute::GetStack( void** stackAddr, size_t* stackSize ) const
{
    return ::pthread_attr_getstack( &mAttribute, stackAddr, stackSize );
}

int Thread::Attribute::SetStack( void* stackAddr, size_t stackSize )
{
    return ::pthread_attr_setstack( &mAttribute, stackAddr, stackSize );
}

int Thread::Attribute::GetStackSize( size_t* stackSize ) const
{
    return ::pthread_attr_getstacksize( &mAttribute, stackSize );
}

int Thread::Attribute::SetStackSize( size_t stackSize )
{
    return ::pthread_attr_setstacksize( &mAttribute, stackSize );
}
