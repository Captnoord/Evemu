/**
 * @file win/File.h
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

#ifndef __COMMON__WIN__FILE_H__INCL__
#define __COMMON__WIN__FILE_H__INCL__

#include "win/Handle.h"

namespace common
{
    namespace win
    {
        /**
         * @brief A Windows' file.
         *
         * @author Bloody.Rabbit
         */
        class File
        : public ReadableHandle,
          public WritableHandle
        {
        public:
            /**
             * @brief Deletes a file.
             *
             * @param[in] name A name of the file.
             *
             * @return An error code.
             */
            static DWORD Delete( PCTSTR name );
            /**
             * @brief Moves a file.
             *
             * @param[in] nameOld The old name.
             * @param[in] nameNew The new name.
             *
             * @return An error code.
             */
            static DWORD Move( PCTSTR nameOld, PCTSTR nameNew );

            /**
             * @brief A default constructor.
             */
            File();
            /**
             * @brief A primary constructor, opens a file.
             *
             * @param[in] name   A name of the file.
             * @param[in] mode   Desired access to the file.
             * @param[in] share  Share mode flags.
             * @param[in] create Creation flags.
             */
            File( PCTSTR name, DWORD mode, DWORD share, DWORD create );

            /**
             * @brief Obtains size of the file.
             *
             * @param[out] size Where to store the size.
             *
             * @return An error code.
             */
            DWORD GetSize( LARGE_INTEGER& size ) const;

            /**
             * @brief Opens a file.
             *
             * @param[in] name   A name of the file.
             * @param[in] mode   Desired access to the file.
             * @param[in] share  Share mode flags.
             * @param[in] create Creation flags.
             *
             * @return An error code.
             */
            DWORD Open( PCTSTR name, DWORD mode, DWORD share, DWORD create );

            /**
             * @brief Sets the internal file pointer.
             *
             * @param[in]  dist   The distance to move.
             * @param[in]  method The starting point.
             * @param[out] result The result of the move.
             *
             * @return An error code.
             */
            DWORD SetPointer( LARGE_INTEGER dist, DWORD method, PLARGE_INTEGER result = NULL );
            /**
             * @brief Flushes buffers of the file.
             *
             * @return An error code.
             */
            DWORD FlushBuffers();
        };
    }
}

#endif /* !__COMMON__WIN__FILE_H__INCL__ */
