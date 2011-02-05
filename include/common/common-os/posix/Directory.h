/**
 * @file posix/Directory.h
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

#ifndef __COMMON__POSIX__DIRECTORY_H__INCL__
#define __COMMON__POSIX__DIRECTORY_H__INCL__

namespace common
{
    namespace posix
    {
        /**
         * @brief Represents a directory stream.
         *
         * @author Bloody.Rabbit
         */
        class Directory
        {
        public:
            /**
             * @brief Creates a new directory.
             *
             * @param[in] path A path to the new directory.
             * @param[in] mode A mode for the new directory.
             *
             * @return An error code.
             */
            static int Create( const char* path, mode_t mode );
            /**
             * @brief Removes a directory.
             *
             * @param[in] path A path to the directory.
             *
             * @return An error code.
             */
            static int Remove( const char* path );

            /**
             * @brief A default constructor.
             */
            Directory();
            /**
             * @brief A primary constructor.
             *
             * @param[in] name A name of the directory to open.
             */
            Directory( const char* name );
            /**
             * @brief A destructor; closes the directory.
             */
            ~Directory();

            /**
             * @brief Checks if the directory is valid.
             *
             * @retval true  The directory is valid.
             * @retval false The directory is invalid.
             */
            bool isValid() const { return NULL != mDir; }
            /**
             * @brief Convenient conversion to boolean.
             *
             * @return A value returned by isValid.
             */
            operator bool() const { return isValid(); }

            /**
             * @brief Obtains current position in the directory.
             *
             * @return The current position.
             */
            long int tell() const;

            /**
             * @brief Opens a directory.
             *
             * @param[in] name A name of the directory.
             *
             * @return An error code.
             */
            int Open( const char* name );
            /**
             * @brief Closes the directory.
             *
             * Does not fail even if the directory is
             * already closed.
             *
             * @return An error code.
             */
            int Close();

            /**
             * @brief Reads the next directory entry.
             *
             * @param[out] entry  Where to store the entry.
             * @param[out] result A result of the read.
             *
             * @return An error code.
             */
            int Read( dirent& entry, dirent*& result );
            /**
             * @brief Seeks within the directory.
             *
             * @param[in] loc A value returned by prior call to tell.
             */
            void Seek( long int loc );
            /**
             * @brief Starts reading the directory from the beginning.
             */
            void Rewind();

        protected:
            /// The directory.
            DIR* mDir;
        };
    }
}

#endif /* !__COMMON__POSIX__DIRECTORY_H__INCL__ */
