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

#include "posix/ThreadAttribute.h"

/*************************************************************************/
/* Posix::Thread::Attribute                                              */
/*************************************************************************/
Posix::Thread::Attribute::Attribute()
{
    int code;

    code = ::pthread_attr_init( &mAttribute );
    assert( 0 == code );
}

Posix::Thread::Attribute::~Attribute()
{
    int code;

    code = ::pthread_attr_destroy( &mAttribute );
    assert( 0 == code );
}

int Posix::Thread::Attribute::GetDetachState( int* detachState ) const
{
    return ::pthread_attr_getdetachstate( &mAttribute, detachState );
}

int Posix::Thread::Attribute::SetDetachState( int detachState )
{
    return ::pthread_attr_setdetachstate( &mAttribute, detachState );
}

int Posix::Thread::Attribute::GetGuardSize( size_t* guardSize ) const
{
    return ::pthread_attr_getguardsize( &mAttribute, guardSize );
}

int Posix::Thread::Attribute::SetGuardSize( size_t guardSize )
{
    return ::pthread_attr_setguardsize( &mAttribute, guardSize );
}

int Posix::Thread::Attribute::GetInheritSched( int* inheritSched ) const
{
    return ::pthread_attr_getinheritsched( &mAttribute, inheritSched );
}

int Posix::Thread::Attribute::SetInheritSched( int inheritSched )
{
    return ::pthread_attr_setinheritsched( &mAttribute, inheritSched );
}

int Posix::Thread::Attribute::GetSchedParam( sched_param* schedParam ) const
{
    return ::pthread_attr_getschedparam( &mAttribute, schedParam );
}

int Posix::Thread::Attribute::SetSchedParam( const sched_param* schedParam )
{
    return ::pthread_attr_setschedparam( &mAttribute, schedParam );
}

int Posix::Thread::Attribute::GetSchedPolicy( int* schedPolicy ) const
{
    return ::pthread_attr_getschedpolicy( &mAttribute, schedPolicy );
}

int Posix::Thread::Attribute::SetSchedPolicy( int schedPolicy )
{
    return ::pthread_attr_setschedpolicy( &mAttribute, schedPolicy );
}

int Posix::Thread::Attribute::GetScope( int* scope ) const
{
    return ::pthread_attr_getscope( &mAttribute, scope );
}

int Posix::Thread::Attribute::SetScope( int scope )
{
    return ::pthread_attr_setscope( &mAttribute, scope );
}

int Posix::Thread::Attribute::GetStack( void** stackAddr, size_t* stackSize ) const
{
    return ::pthread_attr_getstack( &mAttribute, stackAddr, stackSize );
}

int Posix::Thread::Attribute::SetStack( void* stackAddr, size_t stackSize )
{
    return ::pthread_attr_setstack( &mAttribute, stackAddr, stackSize );
}

int Posix::Thread::Attribute::GetStackSize( size_t* stackSize ) const
{
    return ::pthread_attr_getstacksize( &mAttribute, stackSize );
}

int Posix::Thread::Attribute::SetStackSize( size_t stackSize )
{
    return ::pthread_attr_setstacksize( &mAttribute, stackSize );
}
