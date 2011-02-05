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

#ifndef __STREAM__INPUT_H__INCL__
#define __STREAM__INPUT_H__INCL__

#include "stream/Error.h"
#include "util/Data.h"

namespace Stream
{
    /**
     * @brief An input stream.
     *
     * @author Bloody.Rabbit
     */
    template< typename T >
    class Input
    {
    public:
        /// An element of the stream.
        typedef T Element;

        /**
         * @brief A destructor; empty, but virtual.
         */
        virtual ~Input()
        {
        }

        /**
         * @brief Reads an element from the stream, ignoring errors.
         *
         * @return The element.
         */
        Element Read()
        {
            Element e;
            Read( e );
            return e;
        }

        /**
         * @brief Reads an element from the stream.
         *
         * @param[out] e Where to store the element.
         *
         * @return An error code.
         */
        virtual Error Read( Element& e ) { return Read( &e, 1 ); }
        /**
         * @brief Reads multiple elements from the stream.
         *
         * @param[out] ep        Where to store the elements.
         * @param[in]  count     A count of elements in
         *                       the <var>ep</var> array.
         * @param[out] countRead Where the actual count of read
         *                       elements will be stored.
         *
         * @return An error code.
         */
        virtual Error Read( Element* ep, size_t count, size_t* countRead = NULL ) = 0;
    };

    /**
     * @brief A specialization for binary streams.
     *
     * @author Bloody.Rabbit
     */
    template<>
    class Input< void >
    {
    public:
        /// An element of the stream.
        typedef void Element;

        /**
         * @brief A destructor; empty, but virtual.
         */
        virtual ~Input()
        {
        }

        /**
         * @brief Reads some data from the stream.
         *
         * @param[out] data      Where to store the output data.
         * @param[out] bytesRead Where to store the number of
         *                       read bytes.
         *
         * @return An error code.
         */
        virtual Error Read( Util::Data& data, size_t* bytesRead = NULL ) = 0;
    };
}

#endif /* !__STREAM__INPUT_H__INCL__ */
