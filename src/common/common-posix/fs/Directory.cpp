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

#include "CommonPosix.h"

#include "posix/Directory.h"

using namespace common;
using namespace common::fs;

/*************************************************************************/
/* common::fs::Directory                                                 */
/*************************************************************************/
bool Directory::Create( const char* path )
{
    static const mode_t MODE = S_IRUSR | S_IWUSR | S_IXUSR
                             | S_IRGRP           | S_IXGRP
                             | S_IROTH           | S_IXOTH;

    int code = posix::Directory::Create( path, MODE );
    return 0 == code || EEXIST == code;
}

bool Directory::Remove( const char* path )
{
    return 0 == posix::Directory::Remove( path );
}

Directory::Directory()
: mDirectory( new posix::Directory )
{
}

Directory::~Directory()
{
    SafeDelete( mDirectory );
}

bool Directory::isValid() const
{
    return mDirectory->isValid();
}

bool Directory::Open( const char* path, char* name, size_t len )
{
    if( 0 != mDirectory->Open( path ) )
        return false;

    return Next( name, len );
}

bool Directory::Next( char* name, size_t len )
{
    dirent entry;
    dirent* result;
    if( 0 != mDirectory->Read( entry, result ) )
        return false;
    else if( NULL == result )
        return false;

    ::strncpy( name, entry.d_name, len );
    name[ len - 1 ] = '\0';

    return true;
}

bool Directory::Close()
{
    return 0 == mDirectory->Close();
}
