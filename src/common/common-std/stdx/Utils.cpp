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
    Author:     Aim, Captnoord, Zhur, Bloody.Rabbit
*/

#include "CommonStd.h"

#include "stdx/Utils.h"

#ifdef WIN32
int asprintf( char** strp, const char* fmt, ... )
{
    va_list ap;

    va_start( ap, fmt );
    int res = vasprintf( strp, fmt, ap );
    va_end( ap );

    return res;
}

int vasprintf( char** strp, const char* fmt, va_list ap )
{
    //va_list ap_temp;
    //va_copy(ap_temp, ap);
    //int size = vsnprintf(NULL, 0, fmt, ap);
    int size = 0x8000;

    char* buff = (char*)malloc( size + 1 );
    if( buff == NULL )
        return -1;

    size = vsnprintf( buff, size, fmt, ap );
    if( size < 0 )
    {
        SafeFree( buff );
    }
    else
    {
        // do not waste memory
        buff = (char*)realloc( buff, size + 1 );
        buff[size] = '\0';

        (*strp) = buff;
    }

    return size;
}
#endif /* WIN32 */

#ifdef CYGWIN
char* strupr( char* str )
{
    for( size_t i = 0; 0 != str[i]; ++i )
        str[i] = toupper( str[i] );

    return str;
}

char* strlwr( char* str )
{
    for( size_t i = 0; 0 != str[i]; ++i )
        str[i] = tolower( str[i] );

    return str;
}
#endif /* CYGWIN */

int sprintf( std::string& into, const char* fmt, ... )
{
    va_list ap;
    va_start( ap, fmt );

    int code = vsprintf( into, fmt, ap );

    va_end( ap );

    return code;
}

int vsprintf( std::string& into, const char* fmt, va_list ap )
{
    char* buf = NULL;

    int code = vasprintf( &buf, fmt, ap );
    if( 0 <= code )
        into = buf;

    SafeDelete( buf );
    return code;
}
