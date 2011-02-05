/**
 * @file mt/Mutex.cpp
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

#include "mt/Mutex.h"

using namespace common;
using namespace common::mt;

/*************************************************************************/
/* common::mt::Mutex                                                     */
/*************************************************************************/
void Mutex::Lock()
{
#ifdef WIN32
    mMutex.Enter();
#else /* !WIN32 */
    int code = mMutex.Lock();
    assert( 0 == code );
#endif /* !WIN32 */
}

bool Mutex::TryLock()
{
#ifdef WIN32
    return TRUE == mMutex.TryEnter();
#else /* !WIN32 */
    int code = mMutex.TryLock();
    assert( 0 == code || EBUSY == code );

    return 0 == code;
#endif /* !WIN32 */
}

void Mutex::Unlock()
{
#ifdef WIN32
    mMutex.Leave();
#else /* !WIN32 */
    int code = mMutex.Unlock();
    assert( 0 == code );
#endif /* !WIN32 */
}
