/**
 * @file win/InitWinsock.h
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
 * @author Zhur, Bloody.Rabbit
 */

#ifndef __COMMON__WIN__INIT_WINSOCK_H__INCL__
#define __COMMON__WIN__INIT_WINSOCK_H__INCL__

namespace common
{
    namespace win
    {
        /**
         * @brief Winsock initialization class.
         *
         * Utility class to initialize Winsock. Winsock is
         * initialized as long as there is at least one
         * instance of this class.
         *
         * @author Zhur
         */
        class InitWinsock
        {
        public:
            /**
             * @brief A primary constructor.
             */
            InitWinsock();
            /**
             * @brief A destructor.
             */
            ~InitWinsock();
        };
    }
}

#endif /* !__COMMON__WIN__INIT_WINSOCK_H__INCL__ */
