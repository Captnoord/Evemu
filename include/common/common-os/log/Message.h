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

#ifndef __COMMON__LOG__MESSAGE_H__INCL__
#define __COMMON__LOG__MESSAGE_H__INCL__

#include "time/Tm.h"

namespace common
{
    namespace log
    {
        /**
         * @brief A log message.
         *
         * @author Bloody.Rabbit
         */
        class Message
        {
        public:
            /**
             * @brief Type of the message.
             *
             * @author Bloody.Rabbit
             */
            enum Type
            {
                TYPE_NOTICE,  ///< A common message.
                TYPE_ERROR,   ///< An error message.
                TYPE_WARNING, ///< A warning message.
                TYPE_SUCCESS, ///< A success message,
                TYPE_DEBUG,   ///< A debug message.
                TYPE_DUMP,    ///< A dump.

                TYPE_COUNT    ///< Count of the types.
            };

            /**
             * @brief A primary constructor.
             *
             * @param[in] type   Type of the message.
             * @param[in] source Source of the message.
             * @param[in] format Format of the message.
             * @param[in] ...    Additional arguments.
             */
            Message( Type type, const char* source,
                     const char* format, ... );
            /**
             * @brief A primary constructor.
             *
             * @param[in] type   Type of the message.
             * @param[in] source Source of the message.
             * @param[in] format Format of the message.
             * @param[in] ap     Additional arguments.
             */
            Message( Type type, const char* source,
                     const char* format, va_list ap );

            /**
             * @brief Obtains type of the message.
             *
             * @return The type of the message.
             */
            Type type() const { return mType; }
            /**
             * @brief Obtains prefix of the message.
             *
             * @return The prefix of the message.
             */
            char prefix() const;
            /**
             * @brief Obtains time of the message.
             *
             * @return The time.
             */
            const time::Tm& time() const { return mTime; }
            /**
             * @brief Obtains source of the message.
             *
             * @return Source of the message.
             */
            const std::string& source() const { return mSource; }
            /**
             * @brief Obtains the message itself.
             *
             * @return The message.
             */
            const std::string& message() const { return mMessage; }

        protected:
            /// The type of the message.
            Type mType;
            /// The time of the message.
            time::Tm mTime;
            /// The source of the message.
            std::string mSource;
            /// The message.
            std::string mMessage;

            /// An array of type prefixes.
            static const char TYPE_PREFIXES[ TYPE_COUNT ];
        };
    }
}

#endif /* !__COMMON__LOG__MESSAGE_H__INCL__ */
