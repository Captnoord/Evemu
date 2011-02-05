/**
 * @file util/Buffer.h
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

#ifndef __COMMON__UTIL__BUFFER_H__INCL__
#define __COMMON__UTIL__BUFFER_H__INCL__

#include "util/Data.h"
#include "util/Misc.h"

namespace common
{
    namespace util
    {
        /**
         * @brief A dynamic buffer.
         *
         * This class adds memory management to Data. As a result,
         * you can append, resize etc., but it cannot be used with
         * automatic memory (eg. stack variables).
         *
         * @see Class Data which is a cheap generic data wrapper.
         *
         * @author Bloody.Rabbit
         */
        class Buffer
        : public Data
        {
        public:
            /**
             * @brief Creates a buffer of given length.
             *
             * Resizes buffer to be <var>len</var> bytes long, filling
             * it with <var>value</var>.
             *
             * @param[in] len  Length of buffer to be created.
             * @param[in] fill Value to fill the buffer with.
             */
            Buffer( size_t len = 0, const uint8& fill = 0 );
            /**
             * @brief Creates a buffer with given content.
             *
             * Fills the buffer with the content determined by
             * iterators <var>first</var> and <var>last</var>.
             *
             * @param[in] first Iterator pointing to first element.
             * @param[in] last  Iterator pointing to element after the last one.
             */
            template< typename Iter >
            Buffer( Iter first, Iter last );
            /**
             * @brief A copy constructor.
             *
             * @param[in] oth The buffer to copy.
             */
            Buffer( const Buffer& oth );
            /**
             * @brief A destructor.
             *
             * Deletes the buffer.
             */
            ~Buffer();

            /********************************************************************/
            /* Write methods                                                    */
            /********************************************************************/
            /**
             * @brief Appends a single value to the buffer.
             *
             * @param[in] value Value to be appended.
             */
            template< typename T >
            void Append( const T& value );
            /**
             * @brief Appends a sequence of elements to the buffer.
             *
             * @param[in] first Iterator pointing to first element.
             * @param[in] last  Iterator pointing to element after the last one.
             */
            template< typename Iter >
            void Append( Iter first, Iter last );

            /**
             * @brief Assigns a single value to the buffer.
             *
             * @param[in] value New content.
             */
            template< typename T >
            void Assign( const T& value );
            /**
             * @brief Assigns a sequence of elements to the buffer.
             *
             * @param[in] first Iterator pointing to first element.
             * @param[in] last  Iterator pointing to element after the last one.
             */
            template< typename Iter >
            void Assign( Iter first, Iter last );

            /**
             * @brief Appends a value to the buffer.
             *
             * @param[in] value Value to be appended.
             */
            template< typename T >
            Buffer& operator<<( const T& value );
            /**
             * @brief Assigns a new value to the buffer.
             *
             * @param[in] value New content.
             */
            template< typename T >
            Buffer& operator=( const T& value );

            /**
             * @brief A copy operator.
             *
             * @param[in] value A buffer to be copied.
             *
             * @return A reference to self.
             */
            Buffer& operator=( const Buffer& value );

            /********************************************************************/
            /* Size methods                                                     */
            /********************************************************************/
            /**
             * @brief Obtains a current capacity of the buffer.
             *
             * @return Current capacity, in bytes.
             */
            size_t capacity() const { return mCapacity; }

            /**
             * @brief Reserves (pre-allocates) memory for the buffer.
             *
             * Pre-allocates memory for buffer to hold at least
             * <var>requiredCount</var> number of elements.
             *
             * Should be used in cases where lazy reallocating
             * can negatively affect performance.
             *
             * @param[in] requiredCount The least reserved number of elements.
             */
            template< typename T >
            void Reserve( size_t requiredCount );

            /**
             * @brief Reserves (pre-allocates) memory for the buffer
             *        at specific point.
             *
             * Pre-allocates memory for buffer to hold at least
             * <var>requiredCount</var> number of elements, counting from
             * <var>index</var>.
             *
             * Should be used in cases where lazy reallocating can negatively
             * affect performance.
             *
             * @param[in] index         The point at which the memory should
             *                          be reserved.
             * @param[in] requiredCount The least reserved number of elements.
             */
            template< typename T >
            void ReserveAt( ConstIterator< T > index, size_t requiredCount );

            /**
             * @brief Resizes buffer.
             *
             * Changes size of buffer to hold <var>requiredCount</var>
             * number of elements, possibly reallocating it.
             *
             * @param[in] requiredCount The number of elements to hold.
             * @param[in] fill          During buffer expansion the gap
             *                          will be filled by this value.
             */
            template< typename T >
            void Resize( size_t requiredCount, const T& fill = 0 );

            /**
             * @brief Resizes buffer.
             *
             * Changes size of buffer to hold <var>requiredCount</var>
             * number of elements, counting from <var>index</var>,
             * possibly reallocating it.
             *
             * @param[in] index         The point at which the buffer
             *                          should be resized.
             * @param[in] requiredCount The number of elements to hold.
             * @param[in] fill          During buffer expansion the gap
             *                          will be filled by this value.
             */
            template< typename T >
            void ResizeAt( ConstIterator< T > index, size_t requiredCount,
                           const T& fill = 0 );

        protected:
            /// Current capacity of buffer, in bytes.
            size_t mCapacity;

            /**
             * @brief Resizes buffer.
             *
             * Similar to Resize, but does not care
             * about the gaps that may be created.
             *
             * @param[in] requiredCount The number of elements to hold.
             */
            template< typename T >
            void _Resize( size_t requiredCount );

            /**
             * @brief Resizes buffer.
             *
             * Similar to ResizeAt, but does not care
             * about the gaps that may be created.
             *
             * @param[in] index         The point at which the buffer
             *                          should be resized.
             * @param[in] requiredCount The number of elements to hold.
             */
            template< typename T >
            void _ResizeAt( ConstIterator< T > index, size_t requiredCount );

            /**
             * @brief Reallocates buffer.
             *
             * Reallocates buffer so it can efficiently store
             * given amount of data.
             *
             * @param[in] requiredSize The least required new size
             *                         of buffer, in bytes.
             */
            void _Reallocate( size_t requiredSize );

            /**
             * @brief Calculates buffer capacity.
             *
             * Based on current capacity and required size of the buffer,
             * this function calculates capacity of buffer to be allocated.
             *
             * @param[in] currentCapacity Current capacity of buffer, in bytes.
             * @param[in] requiredSize    Required size of buffer, in bytes.
             *
             * @return Capacity to be allocated.
             */
            static size_t _CalcBufferCapacity( size_t currentCapacity,
                                               size_t requiredSize );
        };

#       include "util/Buffer.inl"
    }
}

#endif /* !__COMMON__UTIL__BUFFER_H__INCL__ */
