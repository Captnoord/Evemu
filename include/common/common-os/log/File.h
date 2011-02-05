/**
 * @file log/File.h
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

#ifndef __COMMON__LOG__FILE_H__INCL__
#define __COMMON__LOG__FILE_H__INCL__

#include "log/LogMgr.h"
#include "stdx/File.h"

namespace common
{
    namespace log
    {
        /**
         * @brief A file log.
         *
         * @author Bloody.Rabbit
         */
        class File
        : public Log
        {
        public:
            /**
             * @brief A primary constructor.
             *
             * @param[in] name The name of the output file.
             */
            File( const char* name );

            /**
             * @brief Writes a message into the log file.
             *
             * @param[in] m The message to be written.
             *
             * @return An error code.
             */
            stream::Error Write( const Message& m );
            /**
             * @brief Writes messages into the log file.
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

        protected:
            /// The output file.
            stdx::File mFile;
        };
    }
}

#endif /* !__COMMON__LOG__FILE_H__INCL__ */
