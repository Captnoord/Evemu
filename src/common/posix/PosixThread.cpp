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

#include "posix/PosixThread.h"
#include "posix/PosixThreadAttribute.h"

/*************************************************************************/
/* PosixThread                                                           */
/*************************************************************************/
const PosixThread::Attribute PosixThread::DEFAULT_ATTRIBUTE;

PosixThread PosixThread::self()
{
    return PosixThread( ::pthread_self() );
}

int PosixThread::GetConcurrency()
{
    return ::pthread_getconcurrency();
}

int PosixThread::SetConcurrency( int level )
{
    return ::pthread_setconcurrency( level );
}

int PosixThread::SetCancelState( int state, int* oldState )
{
    return ::pthread_setcancelstate( state, oldState );
}

int PosixThread::SetCancelType( int type, int* oldType )
{
    return ::pthread_setcanceltype( type, oldType );
}

void PosixThread::TestCancel()
{
    ::pthread_testcancel();
}

int PosixThread::Sleep( useconds_t microseconds )
{
    if( 0 < microseconds )
        return ::usleep( microseconds );
    else
        return 0;
}

PosixThread::PosixThread()
{
}

PosixThread::PosixThread( pthread_t thread )
: mThread( thread )
{
}

PosixThread::PosixThread( void* ( *startRoutine )( void* ), void* arg, const Attribute& attr )
{
    int code;

    code = Create( startRoutine, arg, attr );
    assert( 0 == code );
}

PosixThread::PosixThread( const PosixThread& oth )
{
    // let the copy constructor do the job
    *this = oth;
}

int PosixThread::Join( void** retVal ) const
{
    return ::pthread_join( mThread, retVal );
}

bool PosixThread::operator==( const PosixThread& oth ) const
{
    return 0 != ::pthread_equal( mThread, oth.mThread );
}

int PosixThread::Create( void* ( *startRoutine )( void* ), void* arg, const Attribute& attr )
{
    return ::pthread_create( &mThread, &attr.mAttribute, startRoutine, arg );
}

int PosixThread::Cancel()
{
    return ::pthread_cancel( mThread );
}

int PosixThread::Detach()
{
    return ::pthread_detach( mThread );
}

int PosixThread::GetSchedPolicyParam( int* policy, sched_param* schedParam ) const
{
    return ::pthread_getschedparam( mThread, policy, schedParam );
}

int PosixThread::SetSchedPolicyParam( int policy, const sched_param* schedParam )
{
    return ::pthread_setschedparam( mThread, policy, schedParam );
}

PosixThread& PosixThread::operator=( const PosixThread& oth )
{
    mThread = oth.mThread;
}
