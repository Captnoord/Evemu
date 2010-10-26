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

#ifndef __WIN__HANDLE_H__INCL__
#define __WIN__HANDLE_H__INCL__

#include "time/Msec.h"

namespace Win
{
    /**
     * @brief Wrapper around Windows <code>HANDLE</code>.
     *
     * @author Bloody.Rabbit
     */
    class Handle
    {
    public:
        /**
         * @brief Primary constructor.
         *
         * This constructor takes ownership of the handle,
         * i.e. closes it on destruction.
         *
         * @param[in] handle The handle.
         */
        Handle( HANDLE handle = INVALID_HANDLE_VALUE );
        /**
         * @brief Copy constructor, duplicates the handle.
         *
         * @param[in] oth The handle to duplicate.
         */
        Handle( const Handle& oth );
        /**
         * @brief A destructor, closes the handle.
         */
        ~Handle();

        /**
         * @brief Checks if the handle is valid.
         *
         * @retval TRUE  The handle is valid.
         * @retval FALSE The handle is invalid.
         */
        BOOL isValid() const { return ( INVALID_HANDLE_VALUE == mHandle ? FALSE : TRUE ); }

        /**
         * @brief Convenience cast to bool, calls isValid().
         *
         * @return A value returned by isValid().
         */
        operator BOOL() const { return isValid(); }
        /**
         * @brief Compares two handles for equality.
         *
         * @param[in] oth The other handle in comparison.
         *
         * @retval TRUE  The handles are equal.
         * @retval FALSE The handles are not equal.
         */
        BOOL operator==( const Handle& oth ) const { return ( oth.mHandle == mHandle ? TRUE : FALSE ); }

        /**
         * @brief Closes the handle.
         *
         * @return A value returned by <code>CloseHandle</code>.
         */
        BOOL Close();
        /**
         * @brief Closes the handle if necessary.
         *
         * This method closes the handle only if necessary.
         * In other words, call this whenever you need to
         * make sure the handle is closed.
         *
         * @return TRUE if already closed; a value returned
         *         by <code>CloseHandle</code> otherwise.
         */
        BOOL CloseEx();

        /**
         * @brief Copy operator, duplicates the handle.
         *
         * @param[in] oth The handle to duplicate.
         *
         * @return Itself.
         */
        Handle& operator=( const Handle& oth );

    protected:
        /// The handle itself.
        HANDLE mHandle;
    };

    /**
     * @brief A Handle which you can wait for.
     *
     * @author Bloody.Rabbit
     */
    class WaitableHandle
    : virtual public Handle
    {
    public:
        /**
         * @brief A primary constructor.
         *
         * @param[in] handle The handle.
         */
        WaitableHandle( HANDLE handle = INVALID_HANDLE_VALUE );

        /**
         * @brief Waits for the handle.
         *
         * @param[in] timeout The wait timeout.
         *
         * @return A value returned by <code>WaitForSingleObject</code>.
         */
        DWORD Wait( const Time::Msec& timeout = INFINITE ) const;
    };
}

#endif /* !__WIN__HANDLE_H__INCL__ */
