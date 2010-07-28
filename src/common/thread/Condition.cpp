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

#include "thread/Condition.h"
#include "time/TimeMsec.h"
#include "time/TimeTimespec.h"

/*************************************************************************/
/* Condition                                                             */
/*************************************************************************/
void Condition::Signal()
{
#ifdef WIN32
    BOOL success = mCondition.Signal();
    assert( TRUE == success );
#else /* !WIN32 */
    int code = mCondition.Signal();
    assert( 0 == code );
#endif /* !WIN32 */
}

void Condition::Broadcast()
{
#ifdef WIN32
    BOOL success = mCondition.Broadcast();
    assert( TRUE == success );
#else /* !WIN32 */
    int code = mCondition.Broadcast();
    assert( 0 == code );
#endif /* !WIN32 */
}

void Condition::Wait( Mutex& mutex )
{
#ifdef WIN32
    BOOL success = mCondition.Wait( mutex.mCriticalSection );
    assert( TRUE == success );
#else /* !WIN32 */
    int code = mCondition.Wait( mutex.mMutex );
    assert( 0 == code );
#endif /* !WIN32 */
}

void Condition::TimedWait( Mutex& mutex, size_t timeout )
{
#ifdef WIN32
    BOOL success = mCondition.Wait( mutex.mCriticalSection,
                                    static_cast< DWORD >( timeout ) );
    assert( TRUE == success );
#else /* !WIN32 */
    // make absolute time
    size_t msec;
    SetMsecByNow( msec );
    msec += timeout;

    // convert it to timespec
    timespec ts;
    SetTimespecByMsec( ts, msec );

    int code = mCondition.TimedWait( mutex.mMutex, &ts );
    assert( 0 == code );
#endif /* !WIN32 */
}
