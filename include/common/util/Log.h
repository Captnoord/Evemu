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
    Author:     Captnoord
*/

#ifndef __UTIL__LOG_H__INCL__
#define __UTIL__LOG_H__INCL__

#include "mt/Mutex.h"
#include "util/Singleton.h"

#ifdef WIN32
#   include "win/ConsoleScreenBuffer.h"
#endif /* WIN32 */

namespace Util
{
    /**
     * @brief a small and simple logging system.
     *
     * This class is designed to be a simple logging system that both logs to file
     * and console regarding the settings.
     *
     * @author Captnoord.
     * @date August 2009
     */
    class Log
    : public Singleton< Log >
    {
    public:
        /// Primary constructor, initializes logging.
        Log();
        /// Destructor, closes the logfile.
        ~Log();

        /// Checks if we have an opened logfile.
        bool hasLogfile() const { return NULL != mLogfile; }

        /**
         * @brief Logs a message to file.
         *
         * @param[in] source is the source from where the message is printed.
         * @param[in] format is the message itself.
         */
        void Message( const char* source, const char* format, ... );
        /**
         * @brief Logs error message to console and file.
         *
         * @param[in] source is the source from where the message is printed.
         * @param[in] format is the error message itself.
         */
        void Error( const char* source, const char* format, ... );
        /**
         * @brief Logs a warning message to file.
         *
         * @param[in] source is the source from where the message is printed.
         * @param[in] format is the message itself.
         */
        void Warning( const char* source, const char* format, ... );
        /**
         * @brief Logs a success message to file.
         *
         * @param[in] source is the source from where the message is printed.
         * @param[in] format is the message itself.
         */
        void Success( const char* source, const char* format, ... );
        /**
         * @brief Logs a debug message to file and console.
         *
         * Optimized out on a release build.
         *
         * @param[in] source is the source from where the message is printed.
         * @param[in] format is the message itself.
         */
        void Debug( const char* source, const char* format, ... );
        /**
         * @brief Print a hex dump with custom message to file and console.
         *
         * @param[in] source is the source from where the message is printed.
         * @param[in] data   is the data to be dumped.
         * @param[in] length is the length of data.
         * @param[in] format is the custom message.
         */
        void Dump( const char* source, const void* data, size_t length,
                   const char* format, ... );

        /**
         * @brief Opens a logfile.
         *
         * @param[in] filename A name of the logfile.
         *
         * @retval true  The new logfile was successfully opened.
         * @retval false Failed to open the new logfile.
         */
        bool OpenLogfile( const char* filename );
        /**
         * @brief Sets the logfile to be used.
         *
         * Passed <var>file</var> is closed during destruction.
         *
         * @param[in] file A handle to file.
         *
         * @retval true  The new logfile was successfully opened.
         * @retval false Failed to open the new logfile.
         */
        bool SetLogfile( FILE* file );

    protected:
        /// A convenience color enum.
        enum Color
        {
            COLOR_DEFAULT, ///< A default color.
            COLOR_BLACK,   ///< Black color.
            COLOR_RED,     ///< Red color.
            COLOR_GREEN,   ///< Green color.
            COLOR_YELLOW,  ///< Yellow color.
            COLOR_BLUE,    ///< Blue color.
            COLOR_MAGENTA, ///< Magenta color.
            COLOR_CYAN,    ///< Cyan color.
            COLOR_WHITE,   ///< White color.

            COLOR_COUNT    ///< Number of colors.
        };

        /**
         * @brief Prints a hexadecimal dump.
         *
         * @param[in] color  Color of the message.
         * @param[in] prefix Single-character prefix/identificator.
         * @param[in] source Origin of message.
         * @param[in] data   Data to be dumped.
         * @param[in] length Length of <var>data</var> (in bytes).
         */
        void PrintDump( Color color, char prefix, const char* source,
                        const void* data, size_t length );
        /**
         * @brief Prints a line of a hexadecimal dump.
         *
         * @param[in] color  Color of the message.
         * @param[in] prefix Single-character prefix/identificator.
         * @param[in] source Origin of message.
         * @param[in] data   Data to be dumped.
         * @param[in] length Length of <var>data</var> (in bytes).
         * @param[in] offset An offset in <var>data</var> (in bytes).
         */
        void PrintDumpLine( Color color, char prefix, const char* source,
                            const void* data, size_t length, size_t offset );

        /**
         * @brief Prints a message.
         *
         * This prints a generic message.
         *
         * @param[in] color  Color of the message.
         * @param[in] prefix Single-character prefix/identificator.
         * @param[in] source Origin of message.
         * @param[in] format The format string.
         * @param[in] ...    The arguments.
         */
        void PrintMsg( Color color, char prefix, const char* source,
                       const char* format, ... );
        /**
         * @brief Prints a message.
         *
         * This prints a generic message.
         *
         * @param[in] color  Color of the message.
         * @param[in] prefix Single-character prefix/identificator.
         * @param[in] source Origin of message.
         * @param[in] format The format string.
         * @param[in] ap     The arguments.
         */
        void PrintMsgVa( Color color, char prefix, const char* source,
                         const char* format, va_list ap );
        /// Prints time.
        void PrintTime();

        /**
         * @brief Prints a raw message.
         *
         * This method only handles printing to all desired
         * destinations (standard output and logfile at the moment).
         *
         * @param[in] format The format string.
         * @param[in] ...    The arguments.
         */
        void Print( const char* format, ... );
        /**
         * @brief Prints a raw message.
         *
         * This method only handles printing to all desired
         * destinations (standard output and logfile at the moment).
         *
         * @param[in] format The format string.
         * @param[in] ap     The arguments.
         */
        void PrintVa( const char* format, va_list ap );

        /**
         * @brief Sets the color of the output text.
         *
         * @param[in] color The new color of output text.
         */
        void SetColor( Color color );

        /// The active logfile.
        FILE* mLogfile;
        /// Protection against concurrent log messages
        Mt::Mutex mMutex;

#   ifdef WIN32
        /// A handle to the output screen buffer.
        Win::ConsoleScreenBuffer mOutputScreen;

        /// Color translation table.
        static const WORD COLOR_TABLE[ COLOR_COUNT ];
#   else
        /// Color translation table.
        static const char* const COLOR_TABLE[ COLOR_COUNT ];
#   endif /* !WIN32 */
    };
}

/// Evaluates to a Util::Log instance.
#define sLog \
    ( Util::Log::get() )

#endif /* !__UTIL__LOG_H__INCL__ */
