/**
 * @file win/FileSearch.h
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

#ifndef __COMMON__WIN__FILE_SEARCH_H__INCL__
#define __COMMON__WIN__FILE_SEARCH_H__INCL__

#include "win/Handle.h"

namespace common
{
    namespace win
    {
        /**
         * @brief Searches a directory.
         *
         * @author Bloody.Rabbit
         */
        class FileSearch
        : public Handle
        {
        public:
            /**
             * @brief A default constructor.
             */
            FileSearch();
            /**
             * @brief Closes the search.
             */
            ~FileSearch();

            /**
             * @brief Finds a file according to the given name.
             *
             * @param[in]  name A full name of a file to find.
             * @param[out] data Where to store the result.
             *
             * @return An error code.
             */
            DWORD Find( PCTSTR name, PWIN32_FIND_DATA data );
            /**
             * @brief Finds another file.
             *
             * Reuses a filename passed to the last call to Find.
             *
             * @param[out] data Where to store the result.
             *
             * @return An error code.
             */
            DWORD FindNext( PWIN32_FIND_DATA data );

            /**
             * @brief Closes the search.
             *
             * Does not fail even if the handle is already invalid.
             *
             * @return An error code.
             */
            DWORD Close();
        };
    }
}

#endif /* !__COMMON__WIN__FILE_SEARCH_H__INCL__ */
