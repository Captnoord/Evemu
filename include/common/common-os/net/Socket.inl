/**
 * @file net/Socket.inl
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
 * @author Bloody.Rabbit
 */

/*************************************************************************/
/* common::net::Socket                                                   */
/*************************************************************************/
template< typename L3 >
Socket< L3 >::Socket()
: mSocket( INVALID_SOCKET )
{
}

template< typename L3 >
Socket< L3 >::Socket( int type, int prot )
: mSocket( INVALID_SOCKET )
{
    int code = Create( type, prot );
    assert( 0 == code );
}

template< typename L3 >
Socket< L3 >::~Socket()
{
    int code = Close();
    assert( 0 == code );
}

template< typename L3 >
int Socket< L3 >::Create( int type, int prot )
{
    // Close the socket first
    int code = Close();
    if( 0 != code )
        return code;

    // Create the new socket
    SOCKET socket = ::socket( L3::ADDRESS_FAMILY, type, prot );
    if( INVALID_SOCKET == socket )
        return NET_ERRNO;

    // Assign it
    code = Assign( socket, Address() );
    if( 0 != code )
        return code;

    return 0;
}

template< typename L3 >
int Socket< L3 >::Close()
{
    return Assign( INVALID_SOCKET, Address() );
}

template< typename L3 >
int Socket< L3 >::Bind( const Address& address )
{
    if( 0 != ::bind( mSocket,
                     reinterpret_cast< const sockaddr* >( &address.mSocketAddress ),
                     sizeof( address.mSocketAddress ) ) )
        return NET_ERRNO;

    mAddress = address;
    return 0;
}

template< typename L3 >
template< typename T >
int Socket< L3 >::SetOption( int level, int name, const T& val )
{
    if( 0 != ::setsockopt( mSocket, level, name, &val, sizeof( val ) ) )
        return NET_ERRNO;

    return 0;
}

#ifdef WIN32
template< typename L3 >
int Socket< L3 >::Ioctl( long cmd, unsigned long arg )
{
    if( 0 != ::ioctlsocket( mSocket, cmd, &arg ) )
        return NET_ERRNO;

    return 0;
}
#else /* !WIN32 */
template< typename L3 >
int Socket< L3 >::Fcntl( int cmd, long arg )
{
    if( 0 != ::fcntl( mSocket, cmd, arg ) )
        return NET_ERRNO;

    return 0;
}
#endif /* !WIN32 */

template< typename L3 >
int Socket< L3 >::Assign( SOCKET socket, const Address& address )
{
    if( isValid() )
    {
        /* There is a problem that we may be shutting down
           a listening socket, for which the shutdown will
           fail. Thus we ignore ENOTCONN error. */
        if( 0 != ::shutdown( mSocket, SHUT_WR ) )
            if( ENOTCONN != NET_ERRNO )
                return NET_ERRNO;

        if( 0 != ::shutdown( mSocket, SHUT_RD ) )
            if( ENOTCONN != NET_ERRNO )
                return NET_ERRNO;

        if( 0 != ::close( mSocket ) )
            return NET_ERRNO;
    }

    mSocket = socket;
    mAddress = address;
    return 0;
}
