/**
 * @file util/SafeDelete.h
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
 * @author Bloody.Rabbit
 */

#ifndef __COMMON__UTIL__SAFE_DELETE_H__INCL__
#define __COMMON__UTIL__SAFE_DELETE_H__INCL__

namespace common
{
    namespace util
    {
        /**
         * @brief Deletes and nullifies a pointer.
         *
         * Basic programming tips
         * URL: http://nedprod.com/programs/index.html
         * Note: always nullify pointers after deletion, why? because its safer on a MT application
         *
         * @param[in] p The pointer.
         */
        template< typename T >
        inline void safeDelete( T*& p )
        {
            delete p;
            p = NULL;
        }

        /**
         * @brief Deletes and nullifies an array pointer.
         *
         * @param[in] p The array pointer.
         *
         * @see safeDelete< T >( T*& p )
         */
        template< typename T >
        inline void safeDeleteArray( T*& p )
        {
            delete[] p;
            p = NULL;
        }

        /**
         * @brief Frees and nullifies a pointer.
         *
         * @param[in] p The pointer.
         *
         * @see safeDelete< T >( T*& p )
         */
        template< typename T >
        inline void safeFree( T*& p )
        {
            ::free( p );
            p = NULL;
        }
    }
}

#endif /* !__COMMON__UTIL__SAFE_DELETE_H__INCL__ */
