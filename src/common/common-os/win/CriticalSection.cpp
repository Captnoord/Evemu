/**
 * @file win/CriticalSection.cpp
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

#include "win/CriticalSection.h"

using namespace common;
using namespace common::win;

/*************************************************************************/
/* common::win::CriticalSection                                          */
/*************************************************************************/
CriticalSection::CriticalSection()
{
    ::InitializeCriticalSection( &mCriticalSection );
}

CriticalSection::~CriticalSection()
{
    ::DeleteCriticalSection( &mCriticalSection );
}

VOID CriticalSection::Enter()
{
    ::EnterCriticalSection( &mCriticalSection );
}

BOOL CriticalSection::TryEnter()
{
    return ::TryEnterCriticalSection( &mCriticalSection );
}

VOID CriticalSection::Leave()
{
    ::LeaveCriticalSection( &mCriticalSection );
}
