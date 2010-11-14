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

#include "CommonPCH.h"

#include "win/Handle.h"

/*************************************************************************/
/* Win::Handle                                                           */
/*************************************************************************/
Win::Handle::Handle( HANDLE handle )
: mHandle( handle )
{
}

Win::Handle::Handle( const Win::Handle& oth )
: mHandle( INVALID_HANDLE_VALUE )
{
    if( TRUE == oth.isValid() )
    {
        BOOL success = ::DuplicateHandle( ::GetCurrentProcess(), oth.mHandle,
                                          ::GetCurrentProcess(), &mHandle,
                                          0, FALSE, DUPLICATE_SAME_ACCESS );
        assert( TRUE == success );
    }
}

Win::Handle::~Handle()
{
    DWORD code = Close();
    assert( ERROR_SUCCESS == code );
}

DWORD Win::Handle::Close()
{
    if( TRUE == isValid() )
    {
        if( TRUE != ::CloseHandle( mHandle ) )
            return ::GetLastError();
    }

    mHandle = INVALID_HANDLE_VALUE;
    return ERROR_SUCCESS;
}

Win::Handle& Win::Handle::operator=( const Win::Handle& oth )
{
    DWORD code = Close();
    assert( ERROR_SUCCESS == code );

    if( TRUE == oth.isValid() )
    {
        BOOL success = ::DuplicateHandle( ::GetCurrentProcess(), oth.mHandle,
                                          ::GetCurrentProcess(), &mHandle,
                                          0, FALSE, DUPLICATE_SAME_ACCESS );
        assert( TRUE == success );
    }

    return *this;
}

/*************************************************************************/
/* Win::ReadableHandle                                                   */
/*************************************************************************/
Win::ReadableHandle::ReadableHandle( HANDLE handle )
: Win::Handle( handle )
{
}

Win::ReadableHandle::Error Win::ReadableHandle::Read( Util::Data& data, size_t* bytesRead )
{
    DWORD read;

    BOOL success = ::ReadFile( mHandle, &data[0], data.size(), &read, NULL );
    if( TRUE != success )
        return ERROR_READ;

    if( NULL != bytesRead )
        *bytesRead = read;

    return ERROR_OK;
}

/*************************************************************************/
/* Win::WritableHandle                                                   */
/*************************************************************************/
Win::WritableHandle::WritableHandle( HANDLE handle )
: Win::Handle( handle )
{
}

Win::WritableHandle::Error Win::WritableHandle::Write( const Util::Data& data, size_t* bytesWritten )
{
    DWORD written;

    BOOL success = ::WriteFile( mHandle, &data[0], data.size(), &written, NULL );
    if( TRUE != success )
        return ERROR_WRITE;

    if( NULL != bytesWritten )
        *bytesWritten = written;

    return ERROR_OK;
}

/*************************************************************************/
/* Win::WaitableHandle                                                   */
/*************************************************************************/
Win::WaitableHandle::WaitableHandle( HANDLE handle )
: Win::Handle( handle )
{
}

DWORD Win::WaitableHandle::Wait( const Time::Msec& timeout, PDWORD wakeupEvent ) const
{
    DWORD wakeup = ::WaitForSingleObject( mHandle, static_cast< DWORD >( timeout.count() ) );
    if( WAIT_FAILED == wakeup )
        return ::GetLastError();

    if( NULL != wakeupEvent )
        *wakeupEvent = wakeup;

    return ERROR_SUCCESS;
}
