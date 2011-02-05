/**
 * @file win/Event.h
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

#ifndef __COMMON__WIN__EVENT_H__INCL__
#define __COMMON__WIN__EVENT_H__INCL__

#include "win/Handle.h"

namespace common
{
    namespace win
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
             * @return An error code.
             */
            DWORD Create( BOOL manualReset = FALSE, BOOL initialState = FALSE );

            /**
             * @brief Sets the event to signaled state.
             *
             * @return An error code.
             */
            DWORD Set();
            /**
             * @brief Resets the event to nonsignaled state.
             *
             * @return An error code.
             */
            DWORD Reset();
        };
    }
}

#endif /* !__COMMON__WIN__EVENT_H__INCL__ */
