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

#ifndef __COMMON__UTIL__SAFE_DELETE_H__INCL__
#define __COMMON__UTIL__SAFE_DELETE_H__INCL__

// Basic programming tips
// URL: http://nedprod.com/programs/index.html
// Note: always nullify pointers after deletion, why? because its safer on a MT application
#define ENABLE_SAFE_DELETE       1       // only delete and NULL after
#define ENABLE_EXTRA_SAFE_DELETE 1       // check the array for NULL pointer then delete and NULL after
//#define ENABLE_ULTRA_SAFE_DELETE 1       // check object and array for NULL pointer then delete and NULL after

#if defined( ENABLE_ULTRA_SAFE_DELETE )
#   define SafeDelete( p )      { if( p != NULL ) { delete p; p = NULL; } }
#   define SafeDeleteArray( p ) { if( p != NULL ) { delete[] p; p = NULL; } }
#   define SafeFree( p )        { if( p != NULL ) { free( p ); p = NULL; } }
#elif defined( ENABLE_EXTRA_SAFE_DELETE )
#   define SafeDelete( p )      { delete p; p = NULL; }
#   define SafeDeleteArray( p ) { if( p != NULL ) { delete[] p; p = NULL; } }
#   define SafeFree( p )        { if( p != NULL ) { free( p ); p = NULL; } }
#elif defined( ENABLE_SAFE_DELETE )
#   define SafeDelete( p )      { delete p; p = NULL; }
#   define SafeDeleteArray( p ) { delete[] p; p = NULL; }
#   define SafeFree( p )        { free( p ); p = NULL; }
#else /* !ENABLE_ULTRA_SAFE_DELETE && !ENABLE_EXTRA_SAFE_DELETE && !ENABLE_SAFE_DELETE */
#   define SafeDelete( p )      { delete p; }
#   define SafeDeleteArray( p ) { delete[] p; }
#   define SafeFree( p )        { free( p ); }
#endif /* !ENABLE_ULTRA_SAFE_DELETE && !ENABLE_EXTRA_SAFE_DELETE && !ENABLE_SAFE_DELETE */

#endif /* !__COMMON__UTIL__SAFE_DELETE_H__INCL__ */
