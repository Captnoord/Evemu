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
    Author:     Zhur
*/

#include "CommonPCH.h"

#include "zlib/Utils.h"

const uint8 Zlib::HEADER_BYTE = 0x78; //'x'

bool Zlib::IsDeflated( const Util::Buffer& data )
{
    return Zlib::HEADER_BYTE == data[0];
}

bool Zlib::DeflateData( Util::Buffer& data )
{
    Util::Buffer dataDeflated;
    if( !DeflateData( data, dataDeflated ) )
        return false;

    data = dataDeflated;
    return true;
}

bool Zlib::DeflateData( const Util::Buffer& input, Util::Buffer& output )
{
    const Util::Buffer::iterator< uint8 > out = output.end< uint8 >();

    size_t outputSize = compressBound( input.size() );
    output.ResizeAt( out, outputSize );

    int res = compress( &*out, (uLongf*)&outputSize, &input[0], input.size() );

    if( Z_OK == res )
    {
        output.ResizeAt( out, outputSize );
        return true;
    }
    else
    {
        output.ResizeAt( out, 0 );
        return false;
    }
}

bool Zlib::InflateData( Util::Buffer& data )
{
    Util::Buffer dataInflated;
    if( !InflateData( data, dataInflated ) )
        return false;

    data = dataInflated;
    return true;
}

bool Zlib::InflateData( const Util::Buffer& input, Util::Buffer& output )
{
    const Util::Buffer::iterator< uint8 > out = output.end< uint8 >();

    size_t outputSize = 0;
    size_t sizeMultiplier = 0;

    int res = 0;
    do
    {
        outputSize = ( input.size() << ++sizeMultiplier );
        output.ResizeAt( out, outputSize );

        res = uncompress( &*out, (uLongf*)&outputSize, &input[0], input.size() );
    } while( Z_BUF_ERROR == res );

    if( Z_OK == res )
    {
        output.ResizeAt( out, outputSize );
        return true;
    }
    else
    {
        output.ResizeAt( out, 0 );
        return false;
    }
}
