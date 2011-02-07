/**
 * @file stdx/Utils.h
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

#ifndef __COMMON__STDX__UTILS_H__INCL__
#define __COMMON__STDX__UTILS_H__INCL__

/*
 * cfloat, cmath
 */

#if defined( HAVE__FINITE )
#   define finite _finite
#elif defined( HAVE_STD_FINITE )
#   define finite std::finite
#elif defined( HAVE_STD_ISFINITE )
#   define finite std::isfinite
#endif /* defined( HAVE_STD_ISFINITE ) */

#if defined( HAVE__ISNAN )
#   define isnan _isnan
#elif defined( HAVE_STD_ISNAN )
#   define isnan std::isnan
#endif /* defined( HAVE_STD_ISNAN ) */

/*
 * cstdarg
 */

#ifndef HAVE_VA_COPY
#   define va_copy( dest, src ) \
    ( memcpy( &( dest ), &( src ), sizeof( va_list ) ) )
#endif /* !HAVE_VA_COPY */

/*
 * cstdio
 */

#ifndef HAVE_ASPRINTF
int asprintf( char** strp, const char* fmt, ... );
#endif /* !HAVE_ASPRINTF */

#ifndef HAVE_VASPRINTF
int vasprintf( char** strp, const char* fmt, va_list ap );
#endif /* !HAVE_VASPRINTF */

#ifndef HAVE_SNPRINTF
#   define snprintf _snprintf
#endif /* !HAVE_SNPRINTF */

#ifndef HAVE_VSNPRINTF
#   define vsnprintf _vsnprintf
#endif /* !HAVE_VSNPRINTF */

/**
 * @brief sprintf() for <code>std::string</code>.
 *
 * @param[in] into Where to store the result.
 * @param[in] fmt  The format string.
 * @param[in] ...  Arguments.
 *
 * @return A value returned by vasprintf().
 */
int sprintf( std::string& into, const char* fmt, ... );
/**
 * @brief vsprintf() for <code>std::string</code>.
 *
 * @param[in] into Where to store the result.
 * @param[in] fmt  The format string.
 * @param[in] ap   Arguments.
 *
 * @return A value returned by vasprintf().
 */
int vsprintf( std::string& into, const char* fmt, va_list ap );

/*
 * cstdlib
 */

#ifndef HAVE_RANDOM
#   define random rand
#endif /* !HAVE_RANDOM */

#ifndef HAVE_SRANDOM
#   define srandom srand
#endif /* !HAVE_SRANDOM */

/*
 * cstring
 */

#ifndef HAVE_STRCASECMP
#   define strcasecmp _stricmp
#endif /* !HAVE_STRCASECMP */

#ifndef HAVE_STRNCASECMP
#   define strncasecmp _strnicmp
#endif /* !HAVE_STRNCASECMP */

#ifndef HAVE_STRDUP
#   define strdup _strdup
#endif /* !HAVE_STRDUP */

/*
 * ctime
 */

#ifndef HAVE_LOCALTIME_R
tm* localtime_r( const time_t* timep, tm* result );
#endif /* !HAVE_LOCALTIME_R */

#endif /* !__COMMON__STDX__UTILS_H__INCL__ */
