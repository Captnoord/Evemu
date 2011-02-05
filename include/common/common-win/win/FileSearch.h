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

#ifndef __WIN__FILE_SEARCH_H__INCL__
#define __WIN__FILE_SEARCH_H__INCL__

#include "win/Handle.h"

namespace Win
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

#endif /* !__WIN__FILE_SEARCH_H__INCL__ */
