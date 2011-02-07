/**
 * @file win/Handle.h
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

#ifndef __COMMON__WIN__HANDLE_H__INCL__
#define __COMMON__WIN__HANDLE_H__INCL__

#include "time/Msec.h"

namespace common
{
    /**
     * @brief A bunch of wrappers around Windows API.
     */
    namespace win
    {
        /**
         * @brief Wrapper around Windows <code>HANDLE</code>.
         *
         * @author Bloody.Rabbit
         */
        class Handle
        {
        public:
            /**
             * @brief Primary constructor.
             *
             * This constructor takes ownership of the handle,
             * i.e. closes it on destruction.
             *
             * @param[in] handle The handle.
             */
            Handle( HANDLE handle = INVALID_HANDLE_VALUE );
            /**
             * @brief Copy constructor, duplicates the handle.
             *
             * @param[in] oth The handle to duplicate.
             */
            Handle( const Handle& oth );
            /**
             * @brief A destructor, closes the handle.
             */
            ~Handle();

            /**
             * @brief Checks if the handle is valid.
             *
             * @retval TRUE  The handle is valid.
             * @retval FALSE The handle is invalid.
             */
            BOOL isValid() const { return ( INVALID_HANDLE_VALUE == mHandle ? FALSE : TRUE ); }
            /**
             * @brief Convenience cast to bool, calls isValid().
             *
             * @return A value returned by isValid().
             */
            operator BOOL() const { return isValid(); }

            /**
             * @brief Compares two handles for equality.
             *
             * @param[in] oth The other handle in comparison.
             *
             * @retval TRUE  The handles are equal.
             * @retval FALSE The handles are not equal.
             */
            BOOL operator==( const Handle& oth ) const { return ( oth.mHandle == mHandle ? TRUE : FALSE ); }

            /**
             * @brief Closes the handle.
             *
             * This method does not fail if the handle
             * is already closed.
             *
             * @return An error code.
             */
            DWORD Close();

            /**
             * @brief Copy operator, duplicates the handle.
             *
             * @param[in] oth The handle to duplicate.
             *
             * @return Itself.
             */
            Handle& operator=( const Handle& oth );

        protected:
            /// The handle itself.
            HANDLE mHandle;
        };

        /**
         * @brief A handle which you can read from.
         *
         * @author Bloody.Rabbit
         */
        class ReadableHandle
        : virtual public Handle,
          public stream::Input< void >
        {
        public:
            /**
             * @brief A primary constructor.
             *
             * @param[in] handle The handle.
             */
            ReadableHandle( HANDLE handle = INVALID_HANDLE_VALUE );

            /**
             * @brief Reads data from the handle.
             *
             * @param[out] data      Where to store the read data.
             * @param[out] bytesRead Where to store a number of read bytes.
             *
             * @return An error code.
             */
            stream::Error Read( util::Data& data, size_t* bytesRead = NULL );
        };

        /**
         * @brief A handle which you can write to.
         *
         * @author Bloody.Rabbit
         */
        class WritableHandle
        : virtual public Handle,
          public stream::Output< void >
        {
        public:
            /**
             * @brief A primary constructor.
             *
             * @param[in] handle The handle.
             */
            WritableHandle( HANDLE handle = INVALID_HANDLE_VALUE );

            /**
             * @brief Writes data to the handle.
             *
             * @param[in]  data         The data to be written.
             * @param[out] bytesWritten Where to store a number of written bytes.
             *
             * @return An error code.
             */
            stream::Error Write( const util::Data& data, size_t* bytesWritten = NULL );
        };

        /**
         * @brief A handle which you can wait for.
         *
         * @author Bloody.Rabbit
         */
        class WaitableHandle
        : virtual public Handle
        {
        public:
            /**
             * @brief A primary constructor.
             *
             * @param[in] handle The handle.
             */
            WaitableHandle( HANDLE handle = INVALID_HANDLE_VALUE );

            /**
             * @brief Waits for the handle.
             *
             * @param[in]  timeout     The wait timeout.
             * @param[out] wakeupEvent Where to store any wakeup event.
             *
             * @return An error code.
             */
            DWORD Wait( const time::Msec& timeout = INFINITE, PDWORD wakeupEvent = NULL ) const;
        };
    }
}

#endif /* !__COMMON__WIN__HANDLE_H__INCL__ */
