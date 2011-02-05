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

#include "CommonWin.h"

#include "win/ConsoleScreenBuffer.h"

using namespace common;
using namespace common::win;

/*************************************************************************/
/* common::win::ConsoleScreenBuffer                                      */
/*************************************************************************/
const ConsoleScreenBuffer ConsoleScreenBuffer::DEFAULT_OUTPUT_SCREEN(
    ::GetStdHandle( STD_OUTPUT_HANDLE )
);

ConsoleScreenBuffer::ConsoleScreenBuffer( HANDLE handle )
: Handle( handle )
{
}

ConsoleScreenBuffer::ConsoleScreenBuffer( DWORD desiredAccess, DWORD shareMode )
: Handle()
{
    DWORD code = Create( desiredAccess, shareMode );
    assert( ERROR_SUCCESS == code );
}

DWORD ConsoleScreenBuffer::Create( DWORD desiredAccess, DWORD shareMode )
{
    DWORD code = Close();
    if( ERROR_SUCCESS != code )
        return code;

    mHandle = ::CreateConsoleScreenBuffer( desiredAccess, shareMode, NULL, CONSOLE_TEXTMODE_BUFFER, NULL );
    if( TRUE != isValid() )
        return ::GetLastError();

    return ERROR_SUCCESS;
}

DWORD ConsoleScreenBuffer::SetActive()
{
    if( TRUE != ::SetConsoleActiveScreenBuffer( mHandle ) )
        return ::GetLastError();

    return ERROR_SUCCESS;
}

DWORD ConsoleScreenBuffer::SetTextAttributes( WORD attributes )
{
    if( TRUE != ::SetConsoleTextAttribute( mHandle, attributes ) )
        return ::GetLastError();

    return ERROR_SUCCESS;
}
