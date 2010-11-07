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

#ifndef __UTIL__STREAM_H__INCL__
#define __UTIL__STREAM_H__INCL__

#include "util/Data.h"

namespace Util
{
    /**
     * @brief Base class for streams.
     *
     * @author Bloody.Rabbit
     */
    template< typename T >
    class Stream
    {
    public:
        /// An element of the stream.
        typedef T Element;

        /// An enum for all possible stream errors.
        enum Error
        {
            ERROR_OK   = 0, ///< No error.
            ERROR_READ,     ///< An error occurred during reading.
            ERROR_WRITE,    ///< An error occurred during writing.
            ERROR_TRYLATER, ///< Could not read/write all (requested) data, try again later.
            ERROR_EOS       ///< End of stream encountered.
        };
    };

    /**
     * @brief An input stream.
     *
     * @author Bloody.Rabbit
     */
    template< typename T >
    class InputStream
    : virtual public Stream< T >
    {
        /// Our base class.
        typedef Stream< T > Base;

    public:
        /// An element of the stream.
        typedef typename Base::Element Element;
        /// A stream error.
        typedef typename Base::Error   Error;

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
    class InputStream< void >
    : virtual public Stream< void >
    {
        /// Our base class.
        typedef Stream< void > Base;

    public:
        /// An element of the stream.
        typedef Base::Element Element;
        /// A stream error.
        typedef Base::Error   Error;

        /**
         * @brief Reads some data from the stream.
         *
         * @param[out] data      Where to store the output data.
         * @param[out] bytesRead Where to store the number of
         *                       read bytes.
         *
         * @return An error code.
         */
        virtual Error Read( Data& data, size_t* bytesRead = NULL ) = 0;
    };

    /**
     * @brief An output stream.
     *
     * @author Bloody.Rabbit
     */
    template< typename T >
    class OutputStream
    : virtual public Stream< T >
    {
        /// Our base class.
        typedef Stream< T > Base;

    public:
        /// An element of the stream.
        typedef typename Base::Element Element;
        /// A stream error.
        typedef typename Base::Error   Error;

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
    class OutputStream< void >
    : virtual public Stream< void >
    {
        /// Our base class.
        typedef Stream< void > Base;

    public:
        /// An element of the stream.
        typedef Base::Element Element;
        /// A stream error.
        typedef Base::Error   Error;

        /**
         * @brief Writes some data to the stream.
         *
         * @param[in]  data         What data to write.
         * @param[out] bytesWritten Where to store the number
         *                          of written bytes.
         *
         * @return An error code.
         */
        virtual Error Write( const Data& data, size_t* bytesWritten = NULL ) = 0;
    };
}

#endif /* !__UTIL__STREAM_H__INCL__ */
