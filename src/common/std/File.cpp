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
    return ::rename( oldName, newName );
}

int Std::File::Remove( const char* name )
{
    return ::remove( name );
}

Std::File::File()
: mFile( NULL )
{
}

Std::File::File( const char* name, const char* mode )
: mFile( NULL )
{
    bool success = Open( name, mode );
    assert( true == success );
}

Std::File::~File()
{
    int code = CloseEx();
    assert( 0 == code );
}

int Std::File::eof() const
{
    return ::feof( mFile );
}

int Std::File::error() const
{
    return ::ferror( mFile );
}

long int Std::File::tell() const
{
    return ::ftell( mFile );
}

bool Std::File::Open( const char* name, const char* mode )
{
    if( !CloseEx() )
        return false;

    mFile = ::fopen( name, mode );
    return isValid();
}

int Std::File::Close()
{
    return ::fclose( mFile );
}

int Std::File::CloseEx()
{
    return isValid() ? Close() : 0;
}

int Std::File::Seek( long int offset, int origin )
{
    return ::fseek( mFile, offset, origin );
}

size_t Std::File::Read( Buffer& into )
{
    return ::fread( &into[0], 1, into.size(), mFile );
}

size_t Std::File::Write( const Buffer& data )
{
    return ::fwrite( &data[0], 1, data.size(), mFile );
}

int Std::File::Flush()
{
    return ::fflush( mFile );
}
