/**
 * @file mt/Condition.cpp
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

#include "mt/Condition.h"
#include "time/TimeMgr.h"

using namespace common;
using namespace common::mt;

/*************************************************************************/
/* common::mt::Condition                                                 */
/*************************************************************************/
void Condition::Signal()
{
#ifdef WIN32
    DWORD code = mCondition.Signal();
    assert( ERROR_SUCCESS == code );
#else /* !WIN32 */
    int code = mCondition.Signal();
    assert( 0 == code );
#endif /* !WIN32 */
}

void Condition::Broadcast()
{
#ifdef WIN32
    DWORD code = mCondition.Broadcast();
    assert( ERROR_SUCCESS == code );
#else /* !WIN32 */
    int code = mCondition.Broadcast();
    assert( 0 == code );
#endif /* !WIN32 */
}

void Condition::Wait( Mutex& mutex )
{
#ifdef WIN32
    DWORD code = mCondition.Wait( mutex.mMutex );
    assert( ERROR_SUCCESS == code );
#else /* !WIN32 */
    int code = mCondition.Wait( mutex.mMutex );
    assert( 0 == code );
#endif /* !WIN32 */
}

void Condition::TimedWait( Mutex& mutex, const time::Msec& timeout )
{
#ifdef WIN32
    DWORD code = mCondition.Wait( mutex.mMutex, timeout );
    assert( ERROR_SUCCESS == code );
#else /* !WIN32 */
    int code = mCondition.TimedWait( mutex.mMutex, sTimeMgr.nowUnix() + timeout );
    assert( 0 == code );
#endif /* !WIN32 */
}
