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

#include "posix/Fd.h"

/*************************************************************************/
/* Posix::Fd                                                             */
/*************************************************************************/
Posix::Fd::Fd( int fd )
: mFd( fd )
{
}

Posix::Fd::~Fd()
{
    int code = Close();
    assert( 0 == code );
}

int Posix::Fd::Close()
{
    if( isValid() )
    {
        if( 0 != ::close( mFd ) )
            return errno;
    }

    mFd = -1;
    return 0;
}

/*************************************************************************/
/* Posix::ReadableFd                                                     */
/*************************************************************************/
Posix::ReadableFd::ReadableFd( int fd )
: Posix::Fd( fd )
{
}

Posix::ReadableFd::Error Posix::ReadableFd::Read( Util::Data& data, size_t* bytesRead )
{
    ssize_t res = ::read( mFd, &data[0], data.size() );
    if( 0 > res )
        return ERROR_READ;

    if( NULL != bytesRead )
        *bytesRead = res;

    return data.size() == res ? ERROR_OK : ERROR_EOS;
}

/*************************************************************************/
/* Posix::WritableFd                                                     */
/*************************************************************************/
Posix::WritableFd::WritableFd( int fd )
: Posix::Fd( fd )
{
}

Posix::WritableFd::Error Posix::WritableFd::Write( const Util::Data& data, size_t* bytesWritten )
{
    ssize_t res = ::write( mFd, &data[0], data.size() );
    if( 0 > res )
        return ERROR_WRITE;

    if( NULL != bytesWritten )
        *bytesWritten = res;

    return ERROR_OK;
}
