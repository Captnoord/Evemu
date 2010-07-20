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

#ifndef __POSIX__POSIX_CONDITION_ATTRIBUTE_H__INCL__
#define __POSIX__POSIX_CONDITION_ATTRIBUTE_H__INCL__

#include "posix/PosixCondition.h"

/**
 * @brief A wrapper around @c pthread_condattr_t.
 *
 * @author Bloody.Rabbit
 */
class PosixCondition::Attribute
{
    friend class PosixCondition;

public:
    /**
     * @brief A default constructor.
     */
    Attribute();
    /**
     * @brief A primary constructor.
     *
     * @param[in] processShared The value for process-shared attribute.
     */
    Attribute( int processShared );
    /**
     * @brief A destructor.
     */
    ~Attribute();

    /**
     * @brief Obtains a value of process-shared attribute.
     *
     * @param[out] processShared A variable which receives the value.
     *
     * @return A value returned by @c pthread_condattr_getpshared.
     */
    int GetProcessShared( int* processShared ) const;
    /**
     * @brief Sets the value of process-shared attribute.
     *
     * @param[in] processShared A value of process-shared attribute to set.
     *
     * @return A value returned by @c pthread_condattr_setpshared.
     */
    int SetProcessShared( int processShared );

protected:
    /// The condition attribute itself.
    pthread_condattr_t mAttribute;
};

#endif /* !__POSIX__POSIX_CONDITION_ATTRIBUTE_H__INCL__ */
