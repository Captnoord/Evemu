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

#ifndef __STREAM__OUTPUT_H__INCL__
#define __STREAM__OUTPUT_H__INCL__

#include "stream/Error.h"
#include "util/Data.h"

namespace Stream
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
        virtual ~Output()
        {
        }

        /**
         * @brief Writes an element to the stream.
         *
         * @param[in] e The element to be written.
         *
         * @return An error code.
         */
        virtual Error Write( const Element& e ) { return Write( &e, 1 ); }
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
        virtual Error Write( const Element* ep, size_t count, size_t* countWritten = NULL ) = 0;
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
        virtual ~Output()
        {
        }

        /**
         * @brief Writes some data to the stream.
         *
         * @param[in]  data         What data to write.
         * @param[out] bytesWritten Where to store the number
         *                          of written bytes.
         *
         * @return An error code.
         */
        virtual Error Write( const Util::Data& data, size_t* bytesWritten = NULL ) = 0;
    };
}

#endif /* !__STREAM__OUTPUT_H__INCL__ */
