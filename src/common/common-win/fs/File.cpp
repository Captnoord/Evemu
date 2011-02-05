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
/* Fs::File                                                              */
/*************************************************************************/
bool Fs::File::Rename( const char* nameOld, const char* nameNew )
{
    return ERROR_SUCCESS == Win::File::Move( nameOld, nameNew );
}

bool Fs::File::Remove( const char* name )
{
    return ERROR_SUCCESS == Win::File::Delete( name );
}

Fs::File::File()
: mFile( new Win::File )
{
}

Fs::File::File( const char* name, Mode mode )
: mFile( new Win::File )
{
    bool success = Open( name, mode );
    assert( success );
}

Fs::File::~File()
{
    SafeDelete( mFile );
}

bool Fs::File::isValid() const
{
    return TRUE == mFile->isValid();
}

bool Fs::File::GetSize( size_t& size ) const
{
    LARGE_INTEGER sizeWin;
    if( ERROR_SUCCESS != mFile->GetSize( sizeWin ) )
        return false;

    size = sizeWin.QuadPart;
    return true;
}

bool Fs::File::Open( const char* name, Mode mode )
{
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

    return ERROR_SUCCESS == mFile->Open( name, winMode, winShare, winCreate );
}

bool Fs::File::Close()
{
    return ERROR_SUCCESS == mFile->Close();
}

bool Fs::File::Seek( long int offset, Origin origin )
{
    static const DWORD WIN_ORIGIN[ ORIGIN_COUNT ] =
    {
        FILE_BEGIN,   // ORIGIN_BEGIN
        FILE_CURRENT, // ORIGIN_CURRENT
        FILE_END      // ORIGIN_END
    };

    assert( 0 <= origin && origin < ORIGIN_COUNT );

    LARGE_INTEGER offsetWin;
    offsetWin.QuadPart = offset;

    return ERROR_SUCCESS == mFile->SetPointer( offsetWin, WIN_ORIGIN[ origin ] );
}

bool Fs::File::Flush()
{
    return ERROR_SUCCESS == mFile->FlushBuffers();
}

Fs::File::Error Fs::File::Read( Util::Data& data, size_t* bytesRead )
{
    return mFile->Read( data, bytesRead );
}

Fs::File::Error Fs::File::Write( const Util::Data& data, size_t* bytesWritten )
{
    return mFile->Write( data, bytesWritten );
}
