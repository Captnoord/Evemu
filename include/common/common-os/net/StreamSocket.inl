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

/*************************************************************************/
/* common::net::StreamSocket                                             */
/*************************************************************************/
template< typename L3 >
StreamSocket< L3 >::StreamSocket()
{
}

template< typename L3 >
StreamSocket< L3 >::StreamSocket( int prot )
: Base( SOCK_STREAM, prot )
{
}

template< typename L3 >
int StreamSocket< L3 >::Create( int prot )
{
    return Base::Create( SOCK_STREAM, prot );
}

template< typename L3 >
int StreamSocket< L3 >::Connect( const Address& address )
{
    if( 0 != ::connect( Base::mSocket,
                        reinterpret_cast< const sockaddr* >( &address ),
                        sizeof( address ) ) )
        return NET_ERRNO;

    Base::mAddress = address;
    return 0;
}

template< typename L3 >
stream::Error StreamSocket< L3 >::Read( util::Data& data, size_t* bytesRead )
{
    int code = ::recv( Base::mSocket, &data[0], data.size(), 0 );
    if( 0 < code )
    {
        if( NULL != bytesRead )
            *bytesRead = code;

        return stream::ERROR_OK;
    }
    else if( 0 == code )
        return stream::ERROR_EOS;
    else if( EWOULDBLOCK == NET_ERRNO )
        return stream::ERROR_TRYLATER;
    else
        return stream::ERROR_READ;
}

template< typename L3 >
stream::Error StreamSocket< L3 >::Write( const util::Data& data, size_t* bytesWritten )
{
    int code = ::send( Base::mSocket, &data[0], data.size(), 0 );
    if( 0 <= code )
    {
        if( NULL != bytesWritten )
            *bytesWritten = code;

        return stream::ERROR_OK;
    }
    else if( EWOULDBLOCK == NET_ERRNO )
        return stream::ERROR_TRYLATER;
    else
        return stream::ERROR_WRITE;
}

template< typename L3 >
int StreamSocket< L3 >::Listen( int backlog )
{
    if( 0 != ::listen( Base::mSocket, backlog ) )
        return NET_ERRNO;

    return 0;
}

template< typename L3 >
int StreamSocket< L3 >::Listen( const Address& address, int backlog )
{
    int code = Bind( address );
    if( 0 != code )
        return code;

    code = Listen( backlog );
    if( 0 != code )
        return code;

    return 0;
}

template< typename L3 >
int StreamSocket< L3 >::Accept( StreamSocket& into )
{
    // Close the socket first
    int code = into.Close();
    if( 0 != code )
        return code;

    // Obtain the new socket
    Address address;
    socklen_t len = sizeof( address );

    SOCKET socket = ::accept( Base::mSocket,
                              reinterpret_cast< sockaddr* >( &address ),
                              &len );
    if( INVALID_SOCKET == socket )
        return NET_ERRNO;

    // Assign it
    code = into.Assign( socket, address );
    if( 0 != code )
        return code;

    return 0;
}
