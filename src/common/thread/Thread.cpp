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

#include "thread/Thread.h"
#include "time/TimeConst.h"

/*************************************************************************/
/* Thread                                                                */
/*************************************************************************/
Thread Thread::self()
{
#ifdef WIN32
    return Thread( WinThread::self() );
#else /* !WIN32 */
    return Thread( PosixThread::self() );
#endif /* !WIN32 */
}

void Thread::Sleep( size_t milliseconds )
{
    if( 0 < milliseconds )
    {
#ifdef WIN32
        WinThread::Sleep( static_cast< DWORD >( milliseconds ) );
#else /* !WIN32 */
        int code = PosixThread::Sleep( static_cast< useconds_t >( milliseconds * USEC_PER_MSEC ) );
        assert( 0 == code );
#endif /* !WIN32 */
    }
}

Thread::Thread()
{
}

Thread::Thread( Target* target )
{
    Create( target );
}

Thread::Thread( const Thread& oth )
{
    // let the copy operator do the job
    *this = oth;
}

#ifdef WIN32
Thread::Thread( const WinThread& thread )
#else /* !WIN32 */
Thread::Thread( const PosixThread& thread )
#endif /* !WIN32 */
: mThread( thread )
{
}

void Thread::Wait() const
{
#ifdef WIN32
    DWORD code = mThread.Wait();
    assert( WAIT_FAILED != code );
#else /* !WIN32 */
    int code = mThread.Join( NULL );
    assert( 0 == code );
#endif /* !WIN32 */
}

void Thread::Create( Target* target )
{
#ifdef WIN32
    BOOL success = mThread.Create( ThreadMain, target );
    assert( TRUE == success );
#else /* !WIN32 */
    int code = mThread.Create( ThreadMain, target );
    assert( 0 == code );
#endif /* !WIN32 */
}

void Thread::Terminate()
{
#ifdef WIN32
    BOOL success = mThread.Terminate( -1 );
    assert( TRUE == success );
#else /* !WIN32 */
    int code = mThread.Cancel();
    assert( 0 == code );
#endif /* !WIN32 */
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
    int code;

    code = PosixThread::SetCancelState( PTHREAD_CANCEL_ENABLE, NULL );
    assert( 0 == code );

    code = PosixThread::SetCancelType( PTHREAD_CANCEL_ASYNCHRONOUS, NULL );
    assert( 0 == code );
#endif /* !WIN32 */

    Target* target = reinterpret_cast< Target* >( arg );
    target->Run();

    if( target->deleteOnExit() )
        SafeDelete( target );

#ifdef WIN32
    return 0;
#else /* !WIN32 */
    return NULL;
#endif /* !WIN32 */
}
