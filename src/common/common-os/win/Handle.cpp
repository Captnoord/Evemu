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

#include "CommonWin.h"

#include "win/Handle.h"

using namespace common;
using namespace common::win;

/*************************************************************************/
/* common::win::Handle                                                   */
/*************************************************************************/
Handle::Handle( HANDLE handle )
: mHandle( handle )
{
}

Handle::Handle( const Handle& oth )
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

Handle::~Handle()
{
    DWORD code = Close();
    assert( ERROR_SUCCESS == code );
}

DWORD Handle::Close()
{
    if( TRUE == isValid() )
    {
        if( TRUE != ::CloseHandle( mHandle ) )
            return ::GetLastError();
    }

    mHandle = INVALID_HANDLE_VALUE;
    return ERROR_SUCCESS;
}

Handle& Handle::operator=( const Handle& oth )
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
/* common::win::ReadableHandle                                           */
/*************************************************************************/
ReadableHandle::ReadableHandle( HANDLE handle )
: Handle( handle )
{
}

stream::Error ReadableHandle::Read( util::Data& data, size_t* bytesRead )
{
    DWORD read;

    BOOL success = ::ReadFile( mHandle, &data[0], data.size(), &read, NULL );
    if( TRUE != success )
        return stream::ERROR_READ;

    if( NULL != bytesRead )
        *bytesRead = read;

    return stream::ERROR_OK;
}

/*************************************************************************/
/* common::win::WritableHandle                                           */
/*************************************************************************/
WritableHandle::WritableHandle( HANDLE handle )
: Handle( handle )
{
}

stream::Error WritableHandle::Write( const util::Data& data, size_t* bytesWritten )
{
    DWORD written;

    BOOL success = ::WriteFile( mHandle, &data[0], data.size(), &written, NULL );
    if( TRUE != success )
        return stream::ERROR_WRITE;

    if( NULL != bytesWritten )
        *bytesWritten = written;

    return stream::ERROR_OK;
}

/*************************************************************************/
/* common::win::WaitableHandle                                           */
/*************************************************************************/
WaitableHandle::WaitableHandle( HANDLE handle )
: Handle( handle )
{
}

DWORD WaitableHandle::Wait( const time::Msec& timeout, PDWORD wakeupEvent ) const
{
    DWORD wakeup = ::WaitForSingleObject( mHandle, static_cast< DWORD >( timeout.count() ) );
    if( WAIT_FAILED == wakeup )
        return ::GetLastError();

    if( NULL != wakeupEvent )
        *wakeupEvent = wakeup;

    return ERROR_SUCCESS;
}
