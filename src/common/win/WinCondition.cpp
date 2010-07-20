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

#include "win/WinCondition.h"

/*************************************************************************/
/* WinCondition                                                          */
/*************************************************************************/
WinCondition::WinCondition()
: mCurrentCount( 0 ),
  mToFreeCount( 0 )
{
}

BOOL WinCondition::Signal()
{
    BOOL success;

    {
        MutexLock lock( mMutex );

        mToFreeCount = std::min( mToFreeCount + 1, mCurrentCount );

        if( 0 < mToFreeCount )
            success = mWaitEvent.Set();
        else
            success = TRUE;
    }

    return success;
}

BOOL WinCondition::Broadcast()
{
    BOOL success;

    {
        MutexLock lock( mMutex );

        mToFreeCount = mCurrentCount;

        if( 0 < mToFreeCount )
            success = mWaitEvent.Set();
        else
            success = TRUE;
    }

    return success;
}

DWORD WinCondition::Wait( WinCriticalSection& criticalSection, DWORD timeout )
{
    DWORD code = WAIT_OBJECT_0;

    MutexLock lock( mMutex );
    ++mCurrentCount;

    while( WAIT_OBJECT_0 == code
           && 0 == mToFreeCount )
    {
        lock.Unlock();

        criticalSection.Leave();
        code = mWaitEvent.Wait( timeout );
        criticalSection.Enter();

        lock.Relock();
    }

    if( 0 < mToFreeCount )
        --mToFreeCount;

    if( 0 < mToFreeCount )
        mWaitEvent.Set();

    --mCurrentCount;

    return code;
}
