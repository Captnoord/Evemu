/**
 * @file CommonStd.h
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

#ifndef __COMMON_STD_H__INCL__
#define __COMMON_STD_H__INCL__

/*************************************************************************/
/* common-std                                                            */
/*************************************************************************/
/*
 * Custom config include.
 */
#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif /* HAVE_CONFIG_H */

/*
 * Various configuration.
 */
#ifdef MSVC
#   define _CRT_SECURE_NO_WARNINGS                  1
#   define _CRT_SECURE_NO_DEPRECATE                 1
#   define _CRT_SECURE_COPP_OVERLOAD_STANDARD_NAMES 1

#   define _SCL_SECURE_NO_WARNINGS 1
#endif /* MSVC */

#ifdef HAVE_CRTDBG_H
#   ifndef NDEBUG
#       define _CRTDBG_MAP_ALLOC 1
#   endif /* !NDEBUG */
#endif /* HAVE_CRTDBG_H */

// We must "explicitly request" the format strings ...
#ifdef HAVE_INTTYPES_H
#   define __STDC_FORMAT_MACROS 1
#endif /* HAVE_INTTYPES_H */

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
#ifdef HAVE_TR1_PREFIX
#   include <tr1/tuple>
#   include <tr1/unordered_map>
#   include <tr1/unordered_set>
#else /* !HAVE_TR1_PREFIX */
#   include <tuple>
#   include <unordered_map>
#   include <unordered_set>
#endif /* !HAVE_TR1_PREFIX */

/*
 * Some other useful stuff.
 */
#ifdef HAVE_CRTDBG_H
#   ifndef NDEBUG
#       include <crtdbg.h>
#       define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#   endif /* !NDEBUG */
#endif /* HAVE_CRTDBG_H */

#ifdef HAVE_INTTYPES_H
#   include <inttypes.h>
#endif /* HAVE_INTTYPES_H */

/*
 * Include several our common-enough headers.
 */
#include "stdx/Types.h"
#include "stdx/Utils.h"

#include "util/SafeDelete.h"

#endif /* !__COMMON_STD_H__INCL__ */
