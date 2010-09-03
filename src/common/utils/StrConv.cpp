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

#include "utils/StrConv.h"

template<>
std::string strFrom< bool >( const bool& val )
{
    return ( val ? "true" : "false" );
}

template<>
bool strTo< bool >( const char* str )
{
    if( !strcasecmp( str, "true" ) )
        return true;
    else if( !strcasecmp( str, "false" ) )
        return false;
    else if( !strcasecmp( str, "yes" ) )
        return true;
    else if( !strcasecmp( str, "no" ) )
        return false;
    else if( !strcasecmp( str, "y" ) )
        return true;
    else if( !strcasecmp( str, "n" ) )
        return false;
    else if( !strcasecmp( str, "on" ) )
        return true;
    else if( !strcasecmp( str, "off" ) )
        return false;
    else if( !strcasecmp( str, "enable" ) )
        return true;
    else if( !strcasecmp( str, "disable" ) )
        return false;
    else if( !strcasecmp( str, "enabled" ) )
        return true;
    else if( !strcasecmp( str, "disabled" ) )
        return false;
    else if( strTo< int >( str ) )
        return true;
    else
        return false;
}

template<>
std::string strFrom< int64 >( const int64& val )
{
    std::string str;
    sprintf( str, "%"PRId64, val );
    return str;
}

template<>
int64 strTo< int64 >( const char* str )
{
    int64 v = 0;
    sscanf( str, "%"SCNd64, &v );
    return v;
}

template<>
std::string strFrom< uint64 >( const uint64& val )
{
    std::string str;
    sprintf( str, "%"PRIu64, val );
    return str;
}

template<>
uint64 strTo< uint64 >( const char* str )
{
    uint64 v = 0;
    sscanf( str, "%"SCNu64, &v );
    return v;
}

template<>
std::string strFrom< long double >( const long double& val )
{
    std::string str;
    sprintf( str, "%Lf", val );
    return str;
}

template<>
long double strTo< long double >( const char* str )
{
    long double v = 0.0;
    sscanf( str, "%Lf", &v );
    return v;
}
