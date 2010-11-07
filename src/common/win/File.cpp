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

#include "win/File.h"

/*************************************************************************/
/* Win::File                                                             */
/*************************************************************************/
BOOL Win::File::Delete( PCTSTR name )
{
    return ::DeleteFile( name );
}

BOOL Win::File::Move( PCTSTR nameOld, PCTSTR nameNew )
{
    return ::MoveFile( nameOld, nameNew );
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
    BOOL success = Open( name, mode, share, create );
    assert( TRUE == success );
}

BOOL Win::File::GetSize( LARGE_INTEGER& size ) const
{
    return ::GetFileSizeEx( mHandle, &size );
}

BOOL Win::File::Open( PCTSTR name, DWORD mode, DWORD share, DWORD create )
{
    if( TRUE != Close() )
        return FALSE;

    mHandle = ::CreateFile( name, mode, share, NULL, create, 0, NULL );
    return isValid();
}

BOOL Win::File::SetPointer( LARGE_INTEGER dist, DWORD method, PLARGE_INTEGER result )
{
    return ::SetFilePointerEx( mHandle, dist, result, method );
}

BOOL Win::File::FlushBuffers()
{
    return ::FlushFileBuffers( mHandle );
}
