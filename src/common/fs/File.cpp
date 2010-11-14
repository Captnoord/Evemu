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

#include "fs/File.h"

/*************************************************************************/
/* Fs::File                                                              */
/*************************************************************************/
#ifndef WIN32
const mode_t Fs::File::MODE = S_IRUSR | S_IWUSR
                            | S_IRGRP
                            | S_IROTH;
#endif /* !WIN32 */

#ifdef WIN32
const DWORD Fs::File::WIN_ORIGIN[ Fs::File::ORIGIN_COUNT ] =
{
    FILE_BEGIN,   // ORIGIN_BEGIN
    FILE_CURRENT, // ORIGIN_CURRENT
    FILE_END      // ORIGIN_END
};
#else /* !WIN32 */
const int Fs::File::POSIX_ORIGIN[ Fs::File::ORIGIN_COUNT ] =
{
    SEEK_SET, // ORIGIN_BEGIN
    SEEK_CUR, // ORIGIN_CURRENT
    SEEK_END  // ORIGIN_END
};
#endif /* !WIN32 */

bool Fs::File::Rename( const char* nameOld, const char* nameNew )
{
#ifdef WIN32
    return ERROR_SUCCESS == Win::File::Move( nameOld, nameNew );
#else /* !WIN32 */
    return 0 == Posix::File::Rename( nameOld, nameNew );
#endif /* !WIN32 */
}

bool Fs::File::Remove( const char* name )
{
#ifdef WIN32
    return ERROR_SUCCESS == Win::File::Delete( name );
#else /* !WIN32 */
    return 0 == Posix::File::Remove( name );
#endif /* !WIN32 */
}

Fs::File::File()
{
}

Fs::File::File( const char* name, Mode mode )
{
    bool success = Open( name, mode );
    assert( success );
}

bool Fs::File::isValid() const
{
#ifdef WIN32
    return TRUE == mFile.isValid();
#else /* !WIN32 */
    return mFile.isValid();
#endif /* !WIN32 */
}

bool Fs::File::GetSize( size_t& size ) const
{
#ifdef WIN32
    LARGE_INTEGER sizeWin;
    if( ERROR_SUCCESS != mFile.GetSize( sizeWin ) )
        return false;

    size = sizeWin.QuadPart;
    return true;
#else /* !WIN32 */
    struct stat st;
    if( 0 != mFile.Stat( st ) )
        return false;

    size = st.st_size;
    return true;
#endif /* !WIN32 */
}

bool Fs::File::Open( const char* name, Mode mode )
{
#ifdef WIN32
    DWORD winMode = 0;
    if( MODE_ACCESS_READ == ( mode & MODE_ACCESS_READ ) )
        winMode |= GENERIC_READ;
    if( MODE_ACCESS_WRITE == ( mode & MODE_ACCESS_WRITE ) )
        winMode |= GENERIC_WRITE;

    DWORD winShare = FILE_SHARE_READ | FILE_SHARE_WRITE;

    DWORD winCreate = 0;
    if( MODE_OPEN_EXISTING != ( mode & MODE_OPEN_EXISTING ) )
        winCreate = CREATE_NEW;
    else if( MODE_OPEN_NEW == ( mode & MODE_OPEN_NEW ) )
    {
        if( MODE_OPEN_TRUNCATE == ( mode & MODE_OPEN_TRUNCATE ) )
            winCreate = CREATE_ALWAYS;
        else
            winCreate = OPEN_ALWAYS;
    }
    else if( MODE_OPEN_TRUNCATE == ( mode & MODE_OPEN_TRUNCATE ) )
        winCreate = TRUNCATE_EXISTING;
    else
        winCreate = OPEN_EXISTING;

    return ERROR_SUCCESS == mFile.Open( name, winMode, winShare, winCreate );
#else /* !WIN32 */
    int flags = 0;

    if( MODE_ACCESS_READ == ( mode & MODE_ACCESS_READ ) )
    {
        if( MODE_ACCESS_WRITE == ( mode & MODE_ACCESS_WRITE ) )
            flags |= O_RDWR;
        else
            flags |= O_RDONLY;
    }
    else if( MODE_ACCESS_WRITE == ( mode & MODE_ACCESS_WRITE ) )
        flags |= O_WRONLY;

    if( MODE_OPEN_NEW == ( mode & MODE_OPEN_NEW ) )
        flags |= O_CREAT;
    if( MODE_OPEN_EXISTING != ( mode & MODE_OPEN_EXISTING ) )
        flags |= O_EXCL;
    if( MODE_OPEN_TRUNCATE == ( mode & MODE_OPEN_TRUNCATE ) )
        flags |= O_TRUNC;

    return 0 == mFile.Open( name, flags, MODE );
#endif /* !WIN32 */
}

bool Fs::File::Close()
{
#ifdef WIN32
    return ERROR_SUCCESS == mFile.Close();
#else /* !WIN32 */
    return 0 == mFile.Close();
#endif /* !WIN32 */
}

bool Fs::File::Seek( long int offset, Origin origin )
{
    assert( 0 <= origin && origin < ORIGIN_COUNT );

#ifdef WIN32
    LARGE_INTEGER offsetWin;
    offsetWin.QuadPart = offset;

    return ERROR_SUCCESS == mFile.SetPointer( offsetWin, WIN_ORIGIN[ origin ] );
#else /* !WIN32 */
    return 0 == mFile.Seek( offset, POSIX_ORIGIN[ origin ] );
#endif /* !WIN32 */
}

bool Fs::File::Flush()
{
#ifdef WIN32
    return ERROR_SUCCESS == mFile.FlushBuffers();
#else /* !WIN32 */
    return 0 == mFile.Sync();
#endif /* !WIN32 */
}

Fs::File::Error Fs::File::Read( Util::Data& data, size_t* bytesRead )
{
    return mFile.Read( data, bytesRead );
}

Fs::File::Error Fs::File::Write( const Util::Data& data, size_t* bytesWritten )
{
    return mFile.Write( data, bytesWritten );
}
