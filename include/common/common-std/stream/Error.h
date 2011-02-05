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

#ifndef __COMMON__STREAM__ERROR_H__INCL__
#define __COMMON__STREAM__ERROR_H__INCL__

namespace common
{
    namespace stream
    {
        /**
         * @brief An enum for all possible stream errors.
         *
         * @author Bloody.Rabbit
         */
        enum Error
        {
            ERROR_OK   = 0, ///< No error.
            ERROR_READ,     ///< An error occurred during reading.
            ERROR_WRITE,    ///< An error occurred during writing.
            ERROR_TRYLATER, ///< Could not read/write all (requested) data, try again later.
            ERROR_EOS       ///< End of stream encountered.
        };
    }
}

#endif /* !__COMMON__STREAM__ERROR_H__INCL__ */
