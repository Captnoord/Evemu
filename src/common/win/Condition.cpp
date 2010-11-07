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

#include "win/Condition.h"

/*************************************************************************/
/* Win::Condition                                                        */
/*************************************************************************/
Win::Condition::Condition()
: mCurrentCount( 0 ),
  mToFreeCount( 0 )
{
}

BOOL Win::Condition::Signal()
{
    Mt::MutexLock lock( mMutex );

    mToFreeCount = std::min( mToFreeCount + 1, mCurrentCount );
    return 0 < mToFreeCount ? mWaitEvent.Set() : TRUE;
}

BOOL Win::Condition::Broadcast()
{
    Mt::MutexLock lock( mMutex );

    mToFreeCount = mCurrentCount;
    return 0 < mToFreeCount ? mWaitEvent.Set() : TRUE;
}

DWORD Win::Condition::Wait( Win::CriticalSection& criticalSection, const Time::Msec& timeout )
{
    {
        Mt::MutexLock lock( mMutex );
        ++mCurrentCount;
    }

    criticalSection.Leave();
    DWORD code = mWaitEvent.Wait( timeout );
    criticalSection.Enter();

    {
        Mt::MutexLock lock( mMutex );
        assert( mToFreeCount <= mCurrentCount );

        /* It is important to do the stuff below ONLY IF
           we have been woken up intentionally. */
        if( WAIT_OBJECT_0 == code )
        {
            assert( 0 < mToFreeCount );
            --mToFreeCount;

            if( 0 < mToFreeCount )
                mWaitEvent.Set();
            else
                mWaitEvent.Reset();
        }
        /* We failed to wait, so act like we've never been
           here. However, we still need to keep integrity. */
        else if( mToFreeCount == mCurrentCount )
        {
            --mToFreeCount;

            if( 0 == mToFreeCount )
                mWaitEvent.Reset();
        }

        --mCurrentCount;
    }

    return code;
}
