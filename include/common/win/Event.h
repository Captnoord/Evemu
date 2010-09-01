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

#ifndef __WIN__EVENT_H__INCL__
#define __WIN__EVENT_H__INCL__

#include "win/Handle.h"

namespace Win
{
    /**
     * @brief Wrapper for Windows synchronization event handle.
     *
     * @author Bloody.Rabbit
     */
    class Event
    : public WaitableHandle
    {
    public:
        /**
         * @brief A primary constructor.
         *
         * @param[in] manualReset  If TRUE, a manual-reset event is created.
         * @param[in] initialState If TRUE, the event is created signaled.
         */
        Event( BOOL manualReset = FALSE, BOOL initialState = FALSE );

        /**
         * @brief Creates a new event.
         *
         * A handle to the new event is stored in this object;
         * the old one is forgotten.
         *
         * @param[in] manualReset  If TRUE, a manual-reset event is created.
         * @param[in] initialState If TRUE, the event is created signaled.
         *
         * @retval TRUE  Creation successfull.
         * @retval FALSE Creation failed.
         */
        BOOL Create( BOOL manualReset = FALSE, BOOL initialState = FALSE );

        /**
         * @brief Sets the event to signaled state.
         *
         * @return A value returned by <code>SetEvent</code>.
         */
        BOOL Set();
        /**
         * @brief Resets the event to nonsignaled state.
         *
         * @return A value returned by <code>ResetEvent</code>.
         */
        BOOL Reset();
    };
}

#endif /* !__WIN__EVENT_H__INCL__ */
