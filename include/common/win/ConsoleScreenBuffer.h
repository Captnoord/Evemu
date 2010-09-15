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

#ifndef __WIN__CONSOLE_SCREEN_BUFFER_H__INCL__
#define __WIN__CONSOLE_SCREEN_BUFFER_H__INCL__

#include "win/Handle.h"

namespace Win
{
    /**
     * @brief A handle to a console screen buffer.
     *
     * @author Bloody.Rabbit
     */
    class ConsoleScreenBuffer
    : public Handle
    {
    public:
        /// A handle to the default console output screen.
        static const ConsoleScreenBuffer DEFAULT_OUTPUT_SCREEN;

        /**
         * @brief A primary constructor, creates a new console screen buffer.
         *
         * @param[in] desiredAccess A desired access to the buffer.
         * @param[in] shareMode     A share mode for the buffer.
         */
        ConsoleScreenBuffer( DWORD desiredAccess = GENERIC_READ | GENERIC_WRITE,
                             DWORD shareMode = FILE_SHARE_READ );

        /**
         * @brief Sets this buffer as active.
         *
         * An active console screen buffer is the one
         * which is currently displayed.
         *
         * @return A value returned by <code>SetConsoleActiveScreenBuffer</code>.
         */
        BOOL SetActive();
        /**
         * @brief Sets attributes of console text.
         *
         * @param[in] attributes The new attributes.
         *
         * @return A value returned by <code>SetConsoleTextAttribute</code>.
         */
        BOOL SetTextAttributes( WORD attributes );

    protected:
        /**
         * @brief A protected constructor, takes a handle directly.
         *
         * @param[in] handle The handle.
         */
        ConsoleScreenBuffer( HANDLE handle );
    };
}

#endif /* !__WIN__CONSOLE_SCREEN_BUFFER_H__INCL__ */
