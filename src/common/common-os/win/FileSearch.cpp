/**
 * @file win/FileSearch.cpp
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

#include "CommonWin.h"

#include "win/FileSearch.h"

using namespace common;
using namespace common::win;

/*************************************************************************/
/* common::win::FileSearch                                               */
/*************************************************************************/
FileSearch::FileSearch()
: Handle()
{
}

FileSearch::~FileSearch()
{
    DWORD code = Close();
    assert( ERROR_SUCCESS == code );
}

DWORD FileSearch::Find( PCTSTR name, PWIN32_FIND_DATA data )
{
    DWORD code = Close();
    if( ERROR_SUCCESS != code )
        return code;

    mHandle = ::FindFirstFile( name, data );
    if( TRUE != isValid() )
        return ::GetLastError();

    return ERROR_SUCCESS;
}

DWORD FileSearch::FindNext( PWIN32_FIND_DATA data )
{
    if( TRUE != ::FindNextFile( mHandle, data ) )
        return ::GetLastError();

    return ERROR_SUCCESS;
}

DWORD FileSearch::Close()
{
    if( TRUE == isValid() )
    {
        if( TRUE != ::FindClose( mHandle ) )
            return ::GetLastError();
    }

    mHandle = INVALID_HANDLE_VALUE;
    return ERROR_SUCCESS;
}
