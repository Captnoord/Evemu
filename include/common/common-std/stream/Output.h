/**
 * @file stream/Output.h
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

#ifndef __COMMON__STREAM__OUTPUT_H__INCL__
#define __COMMON__STREAM__OUTPUT_H__INCL__

#include "stream/Error.h"
#include "util/Data.h"

namespace common
{
    namespace stream
    {
        /**
         * @brief An output stream.
         *
         * @author Bloody.Rabbit
         */
        template< typename T >
        class Output
        {
        public:
            /// An element of the stream.
            typedef T Element;

            /**
             * @brief A destructor; empty, but virtual.
             */
            virtual ~Output();

            /**
             * @brief Writes an element to the stream.
             *
             * @param[in] e The element to be written.
             *
             * @return An error code.
             */
            virtual Error Write( const Element& e );
            /**
             * @brief Writes multiple elements to the stream.
             *
             * @param[in]  ep           An array of elements to be written.
             * @param[in]  count        Length of the <var>ep</var> array.
             * @param[out] countWritten Where to store the number
             *                          of written elements.
             *
             * @return An error code.
             */
            virtual Error Write( const Element* ep, size_t count, size_t* countWritten = NULL );
        };

        /**
         * @brief A specialization for binary streams.
         *
         * @author Bloody.Rabbit
         */
        template<>
        class Output< void >
        {
        public:
            /// An element of the stream.
            typedef void Element;

            /**
             * @brief A destructor; empty, but virtual.
             */
            virtual ~Output();

            /**
             * @brief Writes some data to the stream.
             *
             * @param[in]  data         What data to write.
             * @param[out] bytesWritten Where to store the number
             *                          of written bytes.
             *
             * @return An error code.
             */
            virtual Error Write( const util::Data& data, size_t* bytesWritten = NULL ) = 0;
        };

#   include "stream/Output.inl"
    }
}

#endif /* !__STREAM__OUTPUT_H__INCL__ */
