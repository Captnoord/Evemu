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

#include "posix/Condition.h"

using namespace common;
using namespace common::posix;

/*************************************************************************/
/* common::posix::Condition                                              */
/*************************************************************************/
const Condition::Attribute Condition::DEFAULT_ATTRIBUTE;

Condition::Condition( const Attribute& attr )
{
    int code = ::pthread_cond_init( &mCondition, &attr.mAttribute );
    assert( 0 == code );
}

Condition::~Condition()
{
    int code = ::pthread_cond_destroy( &mCondition );
    assert( 0 == code );
}

int Condition::Signal()
{
    return ::pthread_cond_signal( &mCondition );
}

int Condition::Broadcast()
{
    return ::pthread_cond_broadcast( &mCondition );
}

int Condition::Wait( Mutex& mutex )
{
    return ::pthread_cond_wait( &mCondition, &mutex.mMutex );
}

int Condition::TimedWait( Mutex& mutex, const time::Timespec& time )
{
    return ::pthread_cond_timedwait( &mCondition, &mutex.mMutex, &time.ts() );
}

/*************************************************************************/
/* common::posix::Condition::Attribute                                   */
/*************************************************************************/
Condition::Attribute::Attribute()
{
    int code = ::pthread_condattr_init( &mAttribute );
    assert( 0 == code );
}

Condition::Attribute::Attribute( int processShared )
{
    int code = ::pthread_condattr_init( &mAttribute );
    assert( 0 == code );

    code = SetProcessShared( processShared );
    assert( 0 == code );
}

Condition::Attribute::~Attribute()
{
    int code = ::pthread_condattr_destroy( &mAttribute );
    assert( 0 == code );
}

int Condition::Attribute::GetProcessShared( int* processShared ) const
{
    return ::pthread_condattr_getpshared( &mAttribute, processShared );
}

int Condition::Attribute::SetProcessShared( int processShared )
{
    return ::pthread_condattr_setpshared( &mAttribute, processShared );
}
