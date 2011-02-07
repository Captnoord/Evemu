/**
 * @file stdx/Utils.cpp
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
 * @author Aim, Captnoord, Zhur, Bloody.Rabbit
 */

#include "CommonStd.h"

#include "stdx/Utils.h"

using namespace common;

/*************************************************************************/
/* cstdio                                                                */
/*************************************************************************/
#ifndef HAVE_ASPRINTF
int asprintf( char** strp, const char* fmt, ... )
{
    va_list ap;

    va_start( ap, fmt );
    int res = ::vasprintf( strp, fmt, ap );
    va_end( ap );

    return res;
}
#endif /* !HAVE_ASPRINTF */

#ifndef HAVE_VASPRINTF
int vasprintf( char** strp, const char* fmt, va_list ap )
{
    //va_list ap_temp;
    //va_copy(ap_temp, ap);
    //int size = vsnprintf(NULL, 0, fmt, ap);
    int size = 0x8000;

    char* buff = (char*)::malloc( size + 1 );
    if( NULL == buff )
        return -1;

    size = ::vsnprintf( buff, size, fmt, ap );
    if( size < 0 )
        util::safeFree( buff );
    else
    {
        // do not waste memory
        buff = (char*)realloc( buff, size + 1 );
        buff[size] = '\0';

        (*strp) = buff;
    }

    return size;
}
#endif /* !HAVE_VASPRINTF */

int sprintf( std::string& into, const char* fmt, ... )
{
    va_list ap;
    va_start( ap, fmt );

    int code = ::vsprintf( into, fmt, ap );

    va_end( ap );

    return code;
}

int vsprintf( std::string& into, const char* fmt, va_list ap )
{
    char* buf = NULL;

    int code = ::vasprintf( &buf, fmt, ap );
    if( 0 <= code )
        into = buf;

    util::safeDelete( buf );
    return code;
}

/*************************************************************************/
/* ctime                                                                 */
/*************************************************************************/
#ifndef HAVE_LOCALTIME_R
tm* localtime_r( const time_t* timep, tm* result )
{
#   ifdef HAVE_LOCALTIME_S
    const errno_t err = ::localtime_s( result, timep );
    if( 0 != err )
    {
        ::errno = err;
        return NULL;
    }
#   else /* !HAVE_LOCALTIME_S */
    /* This is quite dangerous stuff (not necessarily
       thread-safe), but what can we do? Also,
       there is a chance that localtime will use
       thread-specific memory (MS's localtime does that). */
    ::memcpy( result, ::localtime( timep ), sizeof( tm ) );
#   endif /* !HAVE_LOCALTIME_S */

    return result;
}
#endif /* !HAVE_LOCALTIME_R */
