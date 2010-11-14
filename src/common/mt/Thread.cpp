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

#include "mt/Thread.h"

/*************************************************************************/
/* Mt::Thread                                                            */
/*************************************************************************/
Mt::Thread Mt::Thread::self()
{
#ifdef WIN32
    return Mt::Thread( Win::Thread::self() );
#else /* !WIN32 */
    return Mt::Thread( Posix::Thread::self() );
#endif /* !WIN32 */
}

void Mt::Thread::Sleep( const Time::Msec& period )
{
#ifdef WIN32
    Win::Thread::Sleep( period );
#else /* !WIN32 */
    int code = Posix::Thread::Sleep( period );
    assert( 0 == code );
#endif /* !WIN32 */
}

Mt::Thread::Thread()
{
}

Mt::Thread::Thread( Mt::Target* target )
{
    Create( target );
}

Mt::Thread::Thread( const Mt::Thread& oth )
{
    // let the copy operator do the job
    *this = oth;
}

#ifdef WIN32
Mt::Thread::Thread( const Win::Thread& thread )
#else /* !WIN32 */
Mt::Thread::Thread( const Posix::Thread& thread )
#endif /* !WIN32 */
: mThread( thread )
{
}

void Mt::Thread::Wait() const
{
#ifdef WIN32
    DWORD code = mThread.Wait();
    assert( ERROR_SUCCESS == code );
#else /* !WIN32 */
    int code = mThread.Join( NULL );
    assert( 0 == code );
#endif /* !WIN32 */
}

void Mt::Thread::Create( Mt::Target* target )
{
#ifdef WIN32
    DWORD code = mThread.Create( ThreadMain, target );
    assert( ERROR_SUCCESS == code );
#else /* !WIN32 */
    int code = mThread.Create( ThreadMain, target );
    assert( 0 == code );
#endif /* !WIN32 */
}

void Mt::Thread::Terminate()
{
#ifdef WIN32
    DWORD code = mThread.Terminate( -1 );
    assert( ERROR_SUCCESS == code );
#else /* !WIN32 */
    int code = mThread.Cancel();
    assert( 0 == code );
#endif /* !WIN32 */
}

#ifdef WIN32
DWORD WINAPI Mt::Thread::ThreadMain( PVOID arg )
#else /* !WIN32 */
void* Mt::Thread::ThreadMain( void* arg )
#endif /* !WIN32 */
{
#ifndef WIN32
    /* Setup correct cancelability state and type so that
       cancel works as desired. */
    int code = Posix::Thread::SetCancelState( PTHREAD_CANCEL_ENABLE, NULL );
    assert( 0 == code );

    code = Posix::Thread::SetCancelType( PTHREAD_CANCEL_ASYNCHRONOUS, NULL );
    assert( 0 == code );
#endif /* !WIN32 */

    Mt::Target::Process( reinterpret_cast< Mt::Target* >( arg ) );

#ifdef WIN32
    return 0;
#else /* !WIN32 */
    return NULL;
#endif /* !WIN32 */
}
