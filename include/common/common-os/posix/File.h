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

#ifndef __COMMON__POSIX__FILE_H__INCL__
#define __COMMON__POSIX__FILE_H__INCL__

#include "posix/Fd.h"

namespace common
{
    namespace posix
    {
        /**
         * @brief A POSIX file.
         *
         * @author Bloody.Rabbit
         */
        class File
        : public ReadableFd,
          public WritableFd
        {
        public:
            /**
             * @brief Renames a file.
             *
             * @param[in] nameOld The old name.
             * @param[in] nameNew The new name.
             *
             * @return An error code.
             */
            static int Rename( const char* nameOld, const char* nameNew );
            /**
             * @brief Removes a file.
             *
             * @param[in] name A name of the file.
             *
             * @return An error code.
             */
            static int Remove( const char* name );
            /**
             * @brief Obtains information about a file without opening it.
             *
             * @param[in]  name A name of the file.
             * @param[out] buf  Where to store the information.
             *
             * @return An error code.
             */
            static int Stat( const char* name, struct stat& buf );

            /**
             * @brief A default constructor.
             */
            File();
            /**
             * @brief A primary constructor.
             *
             * @param[in] name  A name of the file.
             * @param[in] flags Flags for the open.
             * @param[in] mode  Mode for a new file; ignored
             *                  if no file is created.
             */
            File( const char* name, int flags, mode_t mode );

            /**
             * @brief Obtains information about the file.
             *
             * @param[out] buf Where to store the info.
             *
             * @return An error code.
             */
            int Stat( struct stat& buf ) const;

            /**
             * @brief Opens the file.
             *
             * @param[in] name  A name of the file.
             * @param[in] flags Flags for the open.
             * @param[in] mode  Mode for a new file; ignored
             *                  if no file is created.
             *
             * @return An error code.
             */
            int Open( const char* name, int flags, mode_t mode );

            /**
             * @brief Seeks within the file.
             *
             * @param[in]  offset The offset to seek.
             * @param[in]  whence The origin of the seek.
             * @param[out] result Where to store the resulting offset.
             *
             * @return An error code.
             */
            int Seek( off_t offset, int whence, off_t* result = NULL );
            /**
             * @brief Synchronises changes to the file.
             *
             * @return An error code.
             */
            int Sync();
        };
    }
}

#endif /* !__COMMON__POSIX__FILE_H__INCL__ */
