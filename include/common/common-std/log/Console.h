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

#ifndef __COMMON__LOG__CONSOLE_H__INCL__
#define __COMMON__LOG__CONSOLE_H__INCL__

#include "log/LogMgr.h"

namespace common
{
    namespace log
    {
        /**
         * @brief Console log.
         *
         * @author Bloody.Rabbit
         */
        class Console
        : public Log
        {
        public:
            /**
             * @brief Writes a message to console.
             *
             * @param[in] m The message to be written.
             *
             * @return An error code.
             */
            stream::Error Write( const Message& m );
            /**
             * @brief Writes messages to console.
             *
             * @param[in]  mp           An array of messages to be written.
             * @param[in]  count        Length of the <var>mp</var> array.
             * @param[out] countWritten Where to store the number
             *                          of written messages.
             *
             * @return An error code.
             */
            stream::Error Write( const Message* mp, size_t count,
                                 size_t* countWritten = NULL );
        };
    }
}

#endif /* !__COMMON__LOG__CONSOLE_H__INCL__ */
