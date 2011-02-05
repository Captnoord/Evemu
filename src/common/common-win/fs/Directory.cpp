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

#include "win/Directory.h"
#include "win/FileSearch.h"

/*************************************************************************/
/* Fs::Directory                                                         */
/*************************************************************************/
bool Fs::Directory::Create( const char* path )
{
    DWORD code = Win::Directory::Create( path );
    return ERROR_SUCCESS == code || ERROR_ALREADY_EXISTS == code;
}

bool Fs::Directory::Remove( const char* path )
{
    return ERROR_SUCCESS == Win::Directory::Remove( path );
}

bool Fs::Directory::isValid() const
{
    return TRUE == mDirectory->isValid();
}

bool Fs::Directory::Open( const char* path, char* name, size_t len )
{
    WIN32_FIND_DATA findData;
    if( ERROR_SUCCESS != mDirectory->Find( path, &findData ) )
        return false;

    ::strncpy( name, findData.cFileName, len );
    name[ len - 1 ] = '\0';

    return true;
}

bool Fs::Directory::Next( char* name, size_t len )
{
    WIN32_FIND_DATA findData;
    if( ERROR_SUCCESS != mDirectory->FindNext( &findData ) )
        return false;

    ::strncpy( name, findData.cFileName, len );
    name[ len - 1 ] = '\0';

    return true;
}

bool Fs::Directory::Close()
{
    return ERROR_SUCCESS == mDirectory->Close();
}
