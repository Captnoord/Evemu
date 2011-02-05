/**
 * @file fs/Directory.cpp
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

#include "fs/Directory.h"

using namespace common;
using namespace common::fs;

/*************************************************************************/
/* common::fs::Directory                                                 */
/*************************************************************************/
#ifndef WIN32
const mode_t Directory::MODE = S_IRUSR | S_IWUSR | S_IXUSR
                             | S_IRGRP           | S_IXGRP
                             | S_IROTH           | S_IXOTH;
#endif /* !WIN32 */

bool Directory::Create( const char* path )
{
#ifdef WIN32
    DWORD code = win::Directory::Create( path );
    return ERROR_SUCCESS == code || ERROR_ALREADY_EXISTS == code;
#else /* !WIN32 */
    int code = posix::Directory::Create( path, MODE );
    return 0 == code || EEXIST == code;
#endif /* !WIN32 */
}

bool Directory::Remove( const char* path )
{
#ifdef WIN32
    return ERROR_SUCCESS == win::Directory::Remove( path );
#else /* !WIN32 */
    return 0 == posix::Directory::Remove( path );
#endif /* !WIN32 */
}

bool Directory::isValid() const
{
#ifdef WIN32
    return TRUE == mSearch.isValid();
#else /* !WIN32 */
    return mDirectory.isValid();
#endif /* !WIN32 */
}

bool Directory::Open( const char* path, char* name, size_t len )
{
#ifdef WIN32
    WIN32_FIND_DATA findData;
    if( ERROR_SUCCESS != mSearch.Find( path, &findData ) )
        return false;

    ::strncpy( name, findData.cFileName, len );
    name[ len - 1 ] = '\0';

    return true;
#else /* !WIN32 */
    if( 0 != mDirectory.Open( path ) )
        return false;

    return Next( name, len );
#endif /* !WIN32 */
}

bool Directory::Next( char* name, size_t len )
{
#ifdef WIN32
    WIN32_FIND_DATA findData;
    if( ERROR_SUCCESS != mSearch.FindNext( &findData ) )
        return false;

    ::strncpy( name, findData.cFileName, len );
    name[ len - 1 ] = '\0';

    return true;
#else /* !WIN32 */
    dirent entry;
    dirent* result;
    if( 0 != mDirectory.Read( entry, result ) )
        return false;
    else if( NULL == result )
        return false;

    ::strncpy( name, entry.d_name, len );
    name[ len - 1 ] = '\0';

    return true;
#endif /* !WIN32 */
}

bool Directory::Close()
{
#ifdef WIN32
    return ERROR_SUCCESS == mSearch.Close();
#else /* !WIN32 */
    return 0 == mDirectory.Close();
#endif /* !WIN32 */
}
