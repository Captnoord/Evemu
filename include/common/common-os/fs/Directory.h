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

#ifndef __COMMON__FS__DIRECTORY_H__INCL__
#define __COMMON__FS__DIRECTORY_H__INCL__

#ifdef WIN32
#   include "win/Directory.h"
#   include "win/FileSearch.h"
#else /* !WIN32 */
#   include "posix/Directory.h"
#endif /* !WIN32 */

namespace common
{
    namespace fs
    {
        /**
         * @brief A class for filesystem directories.
         *
         * @author Bloody.Rabbit
         */
        class Directory
        {
        public:
#ifndef WIN32
            /// Access mode for new directories.
            static const mode_t MODE;
#endif /* !WIN32 */

            /**
             * @brief Creates a new directory.
             *
             * This method does not fail event if the
             * directory already exists.
             *
             * @param[in] path A path to the new directory.
             *
             * @retval true  Creation succeeded.
             * @retval false Creation failed.
             */
            static bool Create( const char* path );
            /**
             * @brief Removes a directory.
             *
             * @param[in] path A path to the directory.
             *
             * @retval true  Removal succeeded.
             * @retval false Removal failed.
             */
            static bool Remove( const char* path );

            /**
             * @brief Checks if the directory handle is valid.
             *
             * @retval true  The directory handle is valid.
             * @retval false The directory handle is invalid.
             */
            bool isValid() const;
            /**
             * @brief A convenient bool conversion operator.
             *
             * @return A value returned by isValid().
             */
            operator bool() const { return isValid(); }

            /**
             * @brief Opens a directory
             *
             * @param[in]  path A path to the directory.
             * @param[out] name Where to store the next entry's name.
             * @param[out] len  Amount of space available at <var>name</var>.
             *
             * @retval true  Opened successfully.
             * @retval false Failed to open.
             */
            bool Open( const char* path, char* name, size_t len );
            /**
             * @brief Reads the next entry in the directory.
             *
             * @param[out] name Where to store the next entry's name.
             * @param[out] len  Amount of space available at <var>name</var>.
             *
             * @retval true  Operation successfull.
             * @retval false Operation failed (possibly no entries left).
             */
            bool Next( char* name, size_t len );
            /**
             * @brief Closes the directory.
             *
             * The method succeeds even if the handle is
             * already invalid.
             *
             * @retval true  Closed successfully.
             * @retval false Failed to close.
             */
            bool Close();

        protected:
#       ifdef WIN32
            /// Used to enumerate objects in the directory.
            win::FileSearch mSearch;
#       else /* !WIN32 */
            /// A handle to the directory.
            posix::Directory mDirectory;
#       endif /* !WIN32 */
        };
    }
}

#endif /* !__COMMON__FS__DIRECTORY_H__INCL__ */
