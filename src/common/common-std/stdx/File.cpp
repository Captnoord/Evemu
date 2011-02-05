/**
 * @file stdx/File.cpp
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

#include "CommonStd.h"

#include "stdx/File.h"

using namespace common;
using namespace common::stdx;

/*************************************************************************/
/* common::stdx::File                                                    */
/*************************************************************************/
int File::Rename( const char* oldName, const char* newName )
{
    if( 0 != ::rename( oldName, newName ) )
        return errno;

    return 0;
}

int File::Remove( const char* name )
{
    if( 0 != ::remove( name ) )
        return errno;

    return 0;
}

File::File()
: mFile( NULL )
{
}

File::File( const char* name, const char* mode )
: mFile( NULL )
{
    bool success = Open( name, mode );
    assert( success );
}

File::~File()
{
    bool success = Close();
    assert( success );
}

bool File::eof() const
{
    return 0 != ::feof( mFile );
}

bool File::error() const
{
    return 0 != ::ferror( mFile );
}

long int File::tell() const
{
    return ::ftell( mFile );
}

bool File::Open( const char* name, const char* mode )
{
    if( !Close() )
        return false;

    mFile = ::fopen( name, mode );
    return isValid();
}

bool File::Close()
{
    if( isValid() )
    {
        if( 0 != ::fclose( mFile ) )
            return false;
    }

    mFile = NULL;
    return true;
}

bool File::Seek( long int offset, int origin )
{
    return 0 == ::fseek( mFile, offset, origin );
}

bool File::Flush()
{
    return 0 == ::fflush( mFile );
}

stream::Error File::Read( util::Data& data, size_t* bytesRead )
{
    size_t code = ::fread( &data[0], 1, data.size(), mFile );

    if( NULL != bytesRead )
        *bytesRead = code;

    if( data.size() == code )
        return stream::ERROR_OK;
    else if( eof() )
        return stream::ERROR_EOS;
    else
        return stream::ERROR_READ;
}

stream::Error File::Write( const util::Data& data, size_t* bytesWritten )
{
    size_t code = ::fwrite( &data[0], 1, data.size(), mFile );

    if( NULL != bytesWritten )
        *bytesWritten = code;

    if( data.size() == code )
        return stream::ERROR_OK;
    else
        return stream::ERROR_WRITE;
}

int File::Printf( const char* format, ... )
{
    va_list ap;
    va_start( ap, format );

    int code = Printf( format, ap );

    va_end( ap );

    return code;
}

int File::Printf( const char* format, va_list ap )
{
    return ::vfprintf( mFile, format, ap );
}
