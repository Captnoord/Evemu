/**
 * @file mt/Thread.cpp
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

#include "mt/Thread.h"

using namespace common;
using namespace common::mt;

/*************************************************************************/
/* common::mt::Thread                                                    */
/*************************************************************************/
Thread Thread::self()
{
#ifdef WIN32
    return Thread( win::Thread::self() );
#else /* !WIN32 */
    return Thread( posix::Thread::self() );
#endif /* !WIN32 */
}

void Thread::Sleep( const time::Msec& period )
{
#ifdef WIN32
    win::Thread::Sleep( period );
#else /* !WIN32 */
    int code = posix::Thread::Sleep( period );
    assert( 0 == code );
#endif /* !WIN32 */
}

Thread::Thread()
{
}

Thread::Thread( Target* target )
{
    Create( target );
}

Thread::Thread( const Thread& oth )
: mThread( oth.mThread )
{
}

#ifdef WIN32
Thread::Thread( const win::Thread& thread )
: mThread( thread )
{
}
#else /* !WIN32 */
Thread::Thread( const posix::Thread& thread )
: mThread( thread )
{
}
#endif /* !WIN32 */

unsigned int Thread::id() const
{
    return static_cast< unsigned int >( mThread.id() );
}

void Thread::Wait() const
{
#ifdef WIN32
    DWORD code = mThread.Wait();
    assert( ERROR_SUCCESS == code );
#else /* !WIN32 */
    int code = mThread.Join( NULL );
    assert( 0 == code );
#endif /* !WIN32 */
}

bool Thread::operator==( const Thread& oth ) const
{
#ifdef WIN32
    return TRUE == ( mThread == oth.mThread );
#else /* !WIN32 */
    return mThread == oth.mThread;
#endif /* !WIN32 */
}

void Thread::Create( Target* target )
{
#ifdef WIN32
    DWORD code = mThread.Create( ThreadMain, target );
    assert( ERROR_SUCCESS == code );
#else /* !WIN32 */
    int code = mThread.Create( ThreadMain, target );
    assert( 0 == code );
#endif /* !WIN32 */
}

void Thread::Terminate()
{
#ifdef WIN32
    DWORD code = mThread.Terminate( -1 );
    assert( ERROR_SUCCESS == code );
#else /* !WIN32 */
    int code = mThread.Cancel();
    assert( 0 == code );
#endif /* !WIN32 */
}

Thread& Thread::operator=( const Thread& oth )
{
    mThread = oth.mThread;
    return *this;
}

#ifdef WIN32
DWORD WINAPI Thread::ThreadMain( PVOID arg )
#else /* !WIN32 */
void* Thread::ThreadMain( void* arg )
#endif /* !WIN32 */
{
#ifndef WIN32
    /* Setup correct cancelability state and type so that
       cancel works as desired. */
    int code = posix::Thread::SetCancelState( PTHREAD_CANCEL_ENABLE, NULL );
    assert( 0 == code );

    code = posix::Thread::SetCancelType( PTHREAD_CANCEL_ASYNCHRONOUS, NULL );
    assert( 0 == code );
#endif /* !WIN32 */

    Target::Run( reinterpret_cast< Target* >( arg ) );

#ifdef WIN32
    return 0;
#else /* !WIN32 */
    return NULL;
#endif /* !WIN32 */
}
