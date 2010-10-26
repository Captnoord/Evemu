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

#ifndef __STD__COMMON_H__INCL__
#define __STD__COMMON_H__INCL__

/**
 * @namespace Std
 * @brief Our extensions of standard library.
 */

/*
 * Since there is a slight chance that the standard headers below
 * will include platform-dependent headers, we need to include the
 * platform headers beforehand so that our platform configuration
 * is preserved.
 */
#ifdef WIN32
#   include "win/Common.h"
#else /* !WIN32 */
#   include "posix/Common.h"
#endif /* !WIN32 */

/*
 * (Non-)MSVC configuration.
 */
#ifdef MSVC
#   define _CRT_SECURE_NO_WARNINGS                  1
#   define _CRT_SECURE_NO_DEPRECATE                 1
#   define _CRT_SECURE_COPP_OVERLOAD_STANDARD_NAMES 1

#   ifndef NDEBUG
#       define _CRTDBG_MAP_ALLOC 1
#   endif /* !NDEBUG */

#   define _SCL_SECURE_NO_WARNINGS 1
#else /* !MSVC */
// We must "explicitly request" the format strings ...
#   define __STDC_FORMAT_MACROS 1
#endif /* !MSVC */

/*
 * Standard C library includes.
 */
#ifndef MSVC
#   include <inttypes.h>
#endif /* !MSVC */

/*
 * Standard C++ library includes.
 */
#include <cassert>
#include <cerrno>
#include <cfloat>
#include <climits>
#include <cmath>
#include <csignal>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

/*
 * Standard Template Library includes.
 */
#include <algorithm>
#include <list>
#include <map>
#include <memory>
#include <new>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

/*
 * Technical Report 1 Standard Template Library includes.
 *
 * Note: my fellow developers please read 'http://en.wikipedia.org/wiki/Technical_Report_1'. I know its a wiki page
 *       but it gives you the general idea.
 */
#ifdef MSVC
#   include <tuple>
#   include <unordered_map>
#   include <unordered_set>
#else /* !MSVC */
#   include <tr1/tuple>
#   include <tr1/unordered_map>
#   include <tr1/unordered_set>
#endif /* !MSVC */

/*
 * MSVC memory leak detection includes.
 */
#ifdef MSVC
#   ifndef NDEBUG
#       include <crtdbg.h>
#       define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#   endif /* !NDEBUG */
#endif /* MSVC */

#endif /* !__STD__COMMON_H__INCL__ */
