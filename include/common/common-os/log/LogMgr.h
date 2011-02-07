/**
 * @file log/LogMgr.h
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

#ifndef __COMMON__LOG__LOG_MGR_H__INCL__
#define __COMMON__LOG__LOG_MGR_H__INCL__

#include "log/Message.h"
#include "stream/OutputHub.h"
#include "util/Singleton.h"

namespace common
{
    /**
     * @brief Contains utilities for logging.
     */
    namespace log
    {
        /**
         * @brief Manages all active log modules.
         *
         * @author Bloody.Rabbit
         */
        class LogMgr
        : public stream::OutputHub< Message >,
          public util::Singleton< LogMgr >
        {
        public:
            /**
             * @brief Logs a common message.
             *
             * @param[in] source The source from where the message is printed.
             * @param[in] format The message itself.
             */
            void notice( const char* source, const char* format, ... );
            /**
             * @brief Logs an error message.
             *
             * @param[in] source The source from where the message is printed.
             * @param[in] format The error message itself.
             */
            void error( const char* source, const char* format, ... );
            /**
             * @brief Logs a warning message.
             *
             * @param[in] source The source from where the message is printed.
             * @param[in] format The warning message itself.
             */
            void warning( const char* source, const char* format, ... );
            /**
             * @brief Logs a success message.
             *
             * @param[in] source The source from where the message is printed.
             * @param[in] format The success message itself.
             */
            void success( const char* source, const char* format, ... );
            /**
             * @brief Logs a debug message.
             *
             * @param[in] source The source from where the message is printed.
             * @param[in] format The debug message itself.
             */
            void debug( const char* source, const char* format, ... );
            /**
             * @brief Logs a hex dump with a custom message.
             *
             * @param[in] source The source from where the message is printed.
             * @param[in] data   The data to be dumped.
             * @param[in] length The length of data.
             * @param[in] format The custom message.
             */
            void dump( const char* source, const void* data, size_t length,
                       const char* format, ... );

        protected:
            /**
             * @brief Logs a single line of a hex dump.
             *
             * @param[in] source The source from where the message is printed.
             * @param[in] data   The data to be dumped.
             * @param[in] length The length of data.
             * @param[in] offset Where the dump line begins.
             */
            void dumpLine( const char* source, const void* data, size_t length,
                           size_t offset );
        };

        /// Base of all logs.
        typedef LogMgr::Target Log;
    }
}

/// Convenience macro.
#define sLog \
    ( common::log::LogMgr::get() )

#endif /* !__COMMON__LOG__LOG_MGR_H__INCL__ */