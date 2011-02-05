/**
 * @file posix/Mutex.cpp
 *
 * This file is part of EVEmu: EVE Online Server Emulator.<br>
 * Copyright (C) 2006-2011 The EVEmu Team<br>
 * For the latest information visit <i>http://evemu.org</i>.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA, or go to
 * <i>http://www.gnu.org/copyleft/lesser.txt</i>.
 *
 * @author Bloody.Rabbit
 */

#include "CommonOs.h"

#include "posix/Mutex.h"

using namespace common;
using namespace common::posix;

/*************************************************************************/
/* common::posix::Mutex                                                  */
/*************************************************************************/
const Mutex::Attribute Mutex::DEFAULT_ATTRIBUTE(
#if defined( CYGWIN ) || defined( APPLE )
    PTHREAD_MUTEX_RECURSIVE
#else
    PTHREAD_MUTEX_RECURSIVE_NP
#endif
 );

Mutex::Mutex( const Attribute& attr )
{
    int code = ::pthread_mutex_init( &mMutex, &attr.mAttribute );
    assert( 0 == code );
}

Mutex::~Mutex()
{
    int code = ::pthread_mutex_destroy( &mMutex );
    assert( 0 == code );
}

int Mutex::Lock()
{
    return ::pthread_mutex_lock( &mMutex );
}

int Mutex::TryLock()
{
    return ::pthread_mutex_trylock( &mMutex );
}

int Mutex::Unlock()
{
    return ::pthread_mutex_unlock( &mMutex );
}

/*************************************************************************/
/* common::posix::Mutex::Attribute                                       */
/*************************************************************************/
Mutex::Attribute::Attribute()
{
    int code = ::pthread_mutexattr_init( &mAttribute );
    assert( 0 == code );
}

Mutex::Attribute::Attribute( int type )
{
    int code = ::pthread_mutexattr_init( &mAttribute );
    assert( 0 == code );

    code = SetType( type );
    assert( 0 == code );
}

Mutex::Attribute::~Attribute()
{
    int code = ::pthread_mutexattr_destroy( &mAttribute );
    assert( 0 == code );
}

int Mutex::Attribute::GetType( int* type ) const
{
    return ::pthread_mutexattr_gettype( &mAttribute, type );
}

int Mutex::Attribute::SetType( int type )
{
    return ::pthread_mutexattr_settype( &mAttribute, type );
}
