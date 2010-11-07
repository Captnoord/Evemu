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

#include "std/File.h"

/*************************************************************************/
/* Std::File                                                             */
/*************************************************************************/
int Std::File::Rename( const char* oldName, const char* newName )
{
    if( 0 != ::rename( oldName, newName ) )
        return errno;

    return 0;
}

int Std::File::Remove( const char* name )
{
    if( 0 != ::remove( name ) )
        return errno;

    return 0;
}

Std::File::File()
: mFile( NULL )
{
}

Std::File::File( const char* name, const char* mode )
: mFile( NULL )
{
    bool success = Open( name, mode );
    assert( success );
}

Std::File::~File()
{
    bool success = Close();
    assert( success );
}

bool Std::File::eof() const
{
    return 0 != ::feof( mFile );
}

bool Std::File::error() const
{
    return 0 != ::ferror( mFile );
}

long int Std::File::tell() const
{
    return ::ftell( mFile );
}

bool Std::File::Open( const char* name, const char* mode )
{
    if( !Close() )
        return false;

    mFile = ::fopen( name, mode );
    return isValid();
}

bool Std::File::Close()
{
    if( isValid() )
    {
        if( 0 != ::fclose( mFile ) )
            return false;
    }

    mFile = NULL;
    return true;
}

bool Std::File::Seek( long int offset, int origin )
{
    return 0 == ::fseek( mFile, offset, origin );
}

bool Std::File::Flush()
{
    return 0 == ::fflush( mFile );
}

Std::File::Error Std::File::Read( Util::Data& data, size_t* bytesRead )
{
    size_t code = ::fread( &data[0], 1, data.size(), mFile );

    if( NULL != bytesRead )
        *bytesRead = code;

    if( data.size() == code )
        return ERROR_OK;
    else if( eof() )
        return ERROR_EOS;
    else
        return ERROR_READ;
}

Std::File::Error Std::File::Write( const Util::Data& data, size_t* bytesWritten )
{
    size_t code = ::fwrite( &data[0], 1, data.size(), mFile );

    if( NULL != bytesWritten )
        *bytesWritten = code;

    if( data.size() == code )
        return ERROR_OK;
    else
        return ERROR_WRITE;
}
