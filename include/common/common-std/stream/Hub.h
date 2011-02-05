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

#ifndef __STREAM__HUB_H__INCL__
#define __STREAM__HUB_H__INCL__

#include "stream/Output.h"

namespace Stream
{
    /**
     * @brief A hub which duplicates output.
     *
     * @author Bloody.Rabbit
     */
    template< typename T >
    class OutputHub
    : public Output< T >
    {
        /// Our base for readability.
        typedef Output< T > Base;

    public:
        /// An element of the stream.
        typedef typename Base::Element Element;
        /// A type of the target stream.
        typedef Output< T > Target;

        /**
         * @brief A destructor, destroys the target streams.
         */
        ~OutputHub()
        {
            typename std::set< Target* >::iterator cur, end;
            cur = mTargets.begin();
            end = mTargets.end();
            for(; cur != end; ++cur )
                delete *cur;
        }

        /**
         * @brief Writes an element into all target streams.
         *
         * @param[in] e The element to be written.
         *
         * @return An error code.
         */
        Error Write( const Element& e )
        {
            typename std::set< Target* >::iterator cur, end;
            cur = mTargets.begin();
            end = mTargets.end();
            for(; cur != end; ++cur )
            {
                const Error err = ( *cur )->Write( e );
                if( ERROR_OK != err )
                    return err;
            }

            return ERROR_OK;
        }
        /**
         * @brief Writes elements into all target streams.
         *
         * @param[in]  ep           An array of elements to be written.
         * @param[in]  count        Length of the <var>ep</var> array.
         * @param[out] countWritten Where to store the number
         *                          of written elements.
         *
         * @return An error code.
         */
        Error Write( const Element* ep, size_t count, size_t* countWritten = NULL )
        {
            typename std::set< Target* >::iterator cur, end;
            cur = mTargets.begin();
            end = mTargets.end();
            for(; cur != end; ++cur )
            {
                const Error err = ( *cur )->Write( ep, count, countWritten );
                if( ERROR_OK != err )
                    return err;
            }

            return ERROR_OK;
        }

        /**
         * @brief Adds a target stream.
         *
         * The stream is destroyed in our destructor.
         *
         * @param[in] t The target stream.
         */
        void add( Target* t )
        {
            mTargets.insert( t );
        }
        /**
         * @brief Removes a target stream.
         *
         * The caller is responsible for destruction of
         * the stream.
         *
         * @param[in] t The target stream.
         */
        void remove( Target* t )
        {
            mTargets.remove( t );
        }

    protected:
        /// All target streams.
        std::set< Target* > mTargets;
    };
}

#endif /* !__STREAM__HUB_H__INCL__ */
