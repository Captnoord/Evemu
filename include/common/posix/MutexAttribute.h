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

#ifndef __POSIX__MUTEX_ATTRIBUTE_H__INCL__
#define __POSIX__MUTEX_ATTRIBUTE_H__INCL__

#include "posix/Mutex.h"

namespace Posix
{
    /**
     * @brief Wrapper around <code>pthread_mutexattr_t</code>.
     *
     * @author Bloody.Rabbit
     */
    class Mutex::Attribute
    {
        friend class Mutex;

    public:
        /// The default constructor.
        Attribute();
        /**
         * @brief The primary constructor.
         *
         * @param[in] type Type to be set.
         */
        Attribute( int type );
        /// A destructor.
        ~Attribute();

        /**
         * @brief Obtains the type.
         *
         * @param[out] type A variable which receives the type.
         *
         * @return A value returned by <code>pthread_mutexattr_gettype</code>.
         */
        int GetType( int* type ) const;
        /**
         * @brief Sets the type.
         *
         * @param[in] type Type to be set.
         *
         * @return A value returned by <code>pthread_mutexattr_settype</code>.
         */
        int SetType( int type );

    protected:
        /// The attribute itself.
        pthread_mutexattr_t mAttribute;
    };
}

#endif /* !__POSIX__MUTEX_ATTRIBUTE_H__INCL__ */
