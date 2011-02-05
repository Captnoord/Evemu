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

#ifndef __TEST_NET_STREAM_SOCKET_H__INCL__
#define __TEST_NET_STREAM_SOCKET_H__INCL__

namespace Util
{
    /**
     * @brief Tests Net::StreamSocket.
     *
     * @author Bloody.Rabbit
     */
    template< typename L3 >
    class Test< Net::StreamSocket< L3 > >
    : public CppUnit::TestCase
    {
    public:
        /// A test socket address.
        static const typename L3::SocketAddress SOCKET_ADDRESS;
        /// A test message.
        static const char MESSAGE[];

        /**
         * @brief A primary constructor.
         */
        Test()
        : CppUnit::TestCase( "TestNetStreamSocket" )
        {
        }

        /**
         * @brief Runs a throughput test.
         */
        void runTest()
        {
            // Create the sockets
            CPPUNIT_ASSERT_EQUAL( 0, mClient.Create( IPPROTO_TCP ) );
            CPPUNIT_ASSERT_EQUAL( 0, mServer.Create( IPPROTO_TCP ) );

            // Set a nonblocking mode on the client socket
#       ifdef WIN32
            CPPUNIT_ASSERT_EQUAL( 0, mClient.Ioctl( FIONBIO, 1 ) );
#       else /* !WIN32 */
            CPPUNIT_ASSERT_EQUAL( 0, mClient.Fcntl( F_SETFL, O_NONBLOCK ) );
#       endif /* !WIN32 */

            // Set SO_REUSEADDR on the server socket
            CPPUNIT_ASSERT_EQUAL( 0, mServer.SetOption( SOL_SOCKET, SO_REUSEADDR, 1 ) );

            // Start listening
            CPPUNIT_ASSERT_EQUAL( 0, mServer.Listen( SOCKET_ADDRESS ) );

            // Try to connect (asynchronously)
            CPPUNIT_ASSERT_EQUAL( EINPROGRESS, mClient.Connect( SOCKET_ADDRESS ) );

            // Accept the connection (blocking)
            Net::StreamSocket< L3 > acceptedClient;
            CPPUNIT_ASSERT_EQUAL( 0, mServer.Accept( acceptedClient ) );

            // Send the data (asynchronously)
            Util::Buffer buf( MESSAGE, MESSAGE + sizeof( MESSAGE ) );
            CPPUNIT_ASSERT_EQUAL( Stream::ERROR_OK, mClient.Write( buf ) );

            // Receive the data (blocking)
            CPPUNIT_ASSERT_EQUAL( Stream::ERROR_OK, acceptedClient.Read( buf ) );

            // Compare the data
            CPPUNIT_ASSERT( std::equal( MESSAGE, MESSAGE + sizeof( MESSAGE ),
                                        buf.begin< char >() ) );

            // Close the sockets.
            CPPUNIT_ASSERT_EQUAL( 0, acceptedClient.Close() );
            CPPUNIT_ASSERT_EQUAL( 0, mClient.Close() );
            CPPUNIT_ASSERT_EQUAL( 0, mServer.Close() );
        }

    protected:
        /// A client socket.
        Net::StreamSocket< L3 > mClient;
        /// A server socket.
        Net::StreamSocket< L3 > mServer;
    };

    template< typename L3 >
    const typename L3::SocketAddress Test< Net::StreamSocket< L3 > >::SOCKET_ADDRESS =
        L3::GetSocketAddress( L3::GetAddressByHostname( "localhost" ),
                              L3::GetPort( 43250 ) );
    template< typename L3 >
    const char Test< Net::StreamSocket< L3 > >::MESSAGE[] =
        "This is a test message of TestNetStreamSocket.";
}

#endif /* !__TEST_NET_STREAM_SOCKET_H__INCL__ */
