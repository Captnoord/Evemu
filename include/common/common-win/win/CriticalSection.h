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

#ifndef __COMMON__WIN__CRITICAL_SECTION_H__INCL__
#define __COMMON__WIN__CRITICAL_SECTION_H__INCL__

namespace common
{
    namespace win
    {
        /**
         * @brief Wrapper around Windows critical section.
         *
         * @author Bloody.Rabbit
         */
        class CriticalSection
        {
        public:
            /**
             * @brief Primary constructor.
             */
            CriticalSection();
            /**
             * @brief A destructor.
             */
            ~CriticalSection();

            /**
             * @brief Enters the critical section.
             *
             * This method blocks until the section
             * has been entered.
             */
            VOID Enter();
            /**
             * @brief Attempts to enter the critical section.
             *
             * This method returns immediately; the returned value
             * indicates whether the critical section has been entered.
             *
             * @retval TRUE  Successfully entered the section.
             * @retval FALSE Coudn't enter, another thread is in.
             */
            BOOL TryEnter();

            /**
             * @brief Leaves the critical section.
             */
            VOID Leave();

        protected:
            /// The critical section itself.
            CRITICAL_SECTION mCriticalSection;
        };
    }
}

#endif /* !__COMMON__WIN__CRITICAL_SECTION_H__INCL__ */
