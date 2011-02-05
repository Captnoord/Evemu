/**
 * @file util/RefPtr.h
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

#ifndef __COMMON__UTIL__REF_PTR_H__INCL__
#define __COMMON__UTIL__REF_PTR_H__INCL__

namespace common
{
    namespace util
    {
        /**
         * @brief A reference-counted object.
         *
         * This class has all stuff needed to cooperate with
         * RefPtr. If you want some of your classes to be
         * reference-counted, derive them from this class.
         *
         * @author Bloody.Rabbit
         */
        class RefObject
        {
            template< typename T >
            friend class RefPtr;

        public:
            /**
             * @brief Initializes reference count.
             *
             * @param[in] refCount Initial reference count.
             */
            RefObject( size_t refCount );

            /**
             * @brief Destructor; must be virtual.
             *
             * Must be virtual if proper destructor should be
             * invoked upon destruction.
             */
            virtual ~RefObject();

        protected:
            /**
             * @brief Increments reference count of object by one.
             */
            void IncRef() const;
            /**
             * @brief Decrements reference count of object by one.
             *
             * If reference count of object reaches zero, object
             * is deleted.
             */
            void DecRef() const;

            /// Reference count of instance.
            mutable size_t mRefCount;
        };

        /**
         * @brief Reference-counting-based smart pointer.
         *
         * This smart pointer cares about acquiring/releasing reference
         * of the stored object.
         *
         * @author Bloody.Rabbit
         */
        template< typename T >
        class RefPtr
        {
        public:
            /**
             * @brief Acts as static_cast.
             */
            template< typename T2 >
            static RefPtr StaticCast( const RefPtr< T2 >& oth );

            /**
             * @brief Primary constructor.
             *
             * @param[in] p Pointer to object to be referenced.
             */
            explicit RefPtr( T* p = NULL );
            /**
             * @brief Copy constructor.
             *
             * @param[in] oth Object to copy the reference from.
             */
            RefPtr( const RefPtr& oth );
            /**
             * @brief Casting copy constructor.
             *
             * @param[in] oth Object to copy the reference from.
             */
            template< typename T2 >
            RefPtr( const RefPtr< T2 >& oth );
            /**
             * @brief Destructor, releases reference.
             */
            ~RefPtr();

            /**
             * @brief Copy operator.
             *
             * @param[in] oth Object to copy the reference from.
             */
            RefPtr& operator=( const RefPtr& oth );
            /**
             * @brief Casting copy operator.
             *
             * @param[in] oth Object to copy the reference from.
             */
            template< typename T2 >
            RefPtr& operator=( const RefPtr< T2 >& oth );

            /**
             * @brief Obtains the stored reference.
             *
             * @return The stored reference.
             */
            T* get() const { return mPtr; }

            /**
             * @brief Checks for validity of the stored reference.
             *
             * @return True if stores a reference, false otherwise.
             */
            operator bool() const { return NULL != get(); }

            /**
             * @brief Obtains the stored reference.
             *
             * @return The stored reference.
             */
            T& operator*() const { assert( *this ); return *get(); }
            /**
             * @brief Obtains the stored reference.
             *
             * @return The stored reference.
             */
            T* operator->() const { assert( *this ); return get(); }

            /**
             * @brief Compares two references.
             *
             * @return True if both references are of same object, false if not.
             */
            template< typename T2 >
            bool operator==( const RefPtr< T2 >& oth ) const { return get() == oth.get(); }

        protected:
            /// The pointer to the reference-counted object.
            T* mPtr;
        };

#       include "util/RefPtr.inl"
    }
}

#endif /* !__COMMON__UTIL__REF_PTR_H__INCL__ */
