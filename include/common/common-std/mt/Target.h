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

#ifndef __COMMON__MT__TARGET_H__INCL__
#define __COMMON__MT__TARGET_H__INCL__

namespace common
{
    namespace mt
    {
        /**
         * @brief An execution target.
         *
         * @author Bloody.Rabbit
         */
        class Target
        {
        public:
            /**
             * @brief Runs the given Target.
             *
             * Static to avoid <code>delete this</code> in the code.
             *
             * @param[in] target The Target to run.
             */
            static void Run( Target* target );

            /**
             * @brief Must be virtual if proper destructor shall be invoked.
             */
            virtual ~Target();

        protected:
            /**
             * @brief Determines whether the Target should be deleted when Run() returns.
             *
             * @retval true  Target should be deleted.
             * @retval false Target should not be deleted.
             */
            virtual bool deleteOnExit() = 0;

            /**
             * @brief Runs the target.
             */
            virtual void Run() = 0;
        };

        /**
         * @brief A Target with an ability to stop.
         *
         * @author Bloody.Rabbit
         */
        class TargetEx
        : public Target
        {
        public:
            /**
             * @brief Calling this should make Run() return as soon as possible.
             */
            virtual void Stop() = 0;
        };
    }
}

#endif /* !__COMMON__MT__TARGET_H__INCL__ */
