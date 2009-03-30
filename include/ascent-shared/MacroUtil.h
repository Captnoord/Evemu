/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2008 Ascent Team <http://www.ascentemu.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author:		Captnoord
 */

#ifndef __MACRO_UTIL_H
#define __MACRO_UTIL_H


/** Memory released macro's
 *
 */
#define ASCENT_MALLOC(x) malloc(x)
#define ASCENT_FREE(x) free(x)
#define ASCENT_REALLOC(x,y) realloc(x,y)
#define ASCENT_MEMCPY(x,y,z) memcpy(x,y,z)
#define ASCENT_DELETE delete
#define ASCENT_DELETE_ARRAY delete []

/** This is a collection of utility macro's, the functionality of these macro's may vary.
 *
 */

/* Basic programming tips
 * URL: http://nedprod.com/programs/index.html
 * Note: always nullify pointers after deletion
 */
#define ASCENT_ENABLE_SAFE_DELETE
#define ASCENT_ENABLE_EXTRA_SAFE_DELETE

#ifndef ASCENT_ENABLE_SAFE_DELETE
#  define SafeDelete(p) { ASCENT_DELETE p; }
#  define SafeDeleteArray(p) { ASCENT_DELETE_ARRAY p; }
#  define SafeFree(x) { ASCENT_FREE(x); }
#else
#  ifndef ASCENT_ENABLE_EXTRA_SAFE_DELETE
#    define SafeDelete(p) { ASCENT_DELETE p; p = NULL; }
#    define SafeDeleteArray(p) { ASCENT_DELETE_ARRAY p; p = NULL; }
#    define SafeFree(x) { ASCENT_FREE(x); x = NULL; }
#  else
#    define SafeDelete(p) { if (p != NULL){ delete p; p = NULL; } }
#    define SafeDeleteArray(p) { if (p != NULL) { delete [] p; p = NULL; } }
#    define SafeFree(x) {if (x != NULL) {ASCENT_FREE(x); x = NULL; } }
#  endif//ASCENT_ENABLE_EXTRA_SAFE_DELETE
#endif//ASCENT_ENABLE_SAFE_DELETE

#endif//__MACRO_UTIL_H
