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

#include "win/File.h"

/*************************************************************************/
/* Win::File                                                             */
/*************************************************************************/
DWORD Win::File::Delete( PCTSTR name )
{
    if( TRUE != ::DeleteFile( name ) )
        return ::GetLastError();

    return ERROR_SUCCESS;
}

DWORD Win::File::Move( PCTSTR nameOld, PCTSTR nameNew )
{
    if( TRUE != ::MoveFile( nameOld, nameNew ) )
        return ::GetLastError();

    return ERROR_SUCCESS;
}

Win::File::File()
: Win::Handle(),
  Win::ReadableHandle(),
  Win::WritableHandle()
{
}

Win::File::File( PCTSTR name, DWORD mode, DWORD share, DWORD create )
: Win::Handle(),
  Win::ReadableHandle(),
  Win::WritableHandle()
{
    DWORD code = Open( name, mode, share, create );
    assert( ERROR_SUCCESS == code );
}

DWORD Win::File::GetSize( LARGE_INTEGER& size ) const
{
    if( TRUE != ::GetFileSizeEx( mHandle, &size ) )
        return ::GetLastError();

    return ERROR_SUCCESS;
}

DWORD Win::File::Open( PCTSTR name, DWORD mode, DWORD share, DWORD create )
{
    DWORD code = Close();
    if( ERROR_SUCCESS != code )
        return code;

    mHandle = ::CreateFile( name, mode, share, NULL, create, 0, NULL );
    if( TRUE != isValid() )
        return ::GetLastError();

    return ERROR_SUCCESS;
}

DWORD Win::File::SetPointer( LARGE_INTEGER dist, DWORD method, PLARGE_INTEGER result )
{
    if( TRUE != ::SetFilePointerEx( mHandle, dist, result, method ) )
        return ::GetLastError();

    return ERROR_SUCCESS;
}

DWORD Win::File::FlushBuffers()
{
    if( TRUE != ::FlushFileBuffers( mHandle ) )
        return ::GetLastError();

    return ERROR_SUCCESS;
}
