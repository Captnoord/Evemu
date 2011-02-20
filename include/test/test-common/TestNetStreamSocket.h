/**
 * @file TestNetStreamSocket.h
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

#ifndef __TEST__TEST_NET_STREAM_SOCKET_H__INCL__
#define __TEST__TEST_NET_STREAM_SOCKET_H__INCL__

namespace test
{
    /**
     * @brief Tests net::StreamSocket.
     *
     * @author Bloody.Rabbit
     */
    template< typename L3 >
    class TestNetStreamSocket
    : public CppUnit::TestCase
    {
    public:
        /// A test message.
        static const char MESSAGE[];

        /**
         * @brief A primary constructor.
         */
        TestNetStreamSocket()
        : CppUnit::TestCase( "TestNetStreamSocket" )
        {
        }

        /**
         * @brief Runs a throughput test.
         */
        void runTest()
        {
            // Prepare socket address
            typename L3::SocketAddress socketAddress;

            CPPUNIT_ASSERT( socketAddress.setHostname( "localhost" ) );
            socketAddress.setPort( 43250 );

            // Create and setup the sockets
            CPPUNIT_ASSERT_EQUAL( 0, mClient.Create( IPPROTO_TCP ) );
            CPPUNIT_ASSERT_EQUAL( 0, mServer.Create( IPPROTO_TCP ) );

#       ifdef WIN32
            CPPUNIT_ASSERT_EQUAL( 0, mClient.Ioctl( FIONBIO, 1 ) );
#       else /* !WIN32 */
            CPPUNIT_ASSERT_EQUAL( 0, mClient.Fcntl( F_SETFL, O_NONBLOCK ) );
#       endif /* !WIN32 */

            CPPUNIT_ASSERT_EQUAL( 0, mServer.SetOption( SOL_SOCKET, SO_REUSEADDR, 1 ) );

            // Try to connect
            net::StreamSocket< L3 > acceptedClient;

            CPPUNIT_ASSERT_EQUAL( 0, mServer.Listen( socketAddress ) );
            CPPUNIT_ASSERT_EQUAL( EINPROGRESS, mClient.Connect( socketAddress ) );
            CPPUNIT_ASSERT_EQUAL( 0, mServer.Accept( acceptedClient ) );

            // Transfer and verify the data
            util::Buffer buf( MESSAGE, MESSAGE + sizeof( MESSAGE ) );

            CPPUNIT_ASSERT_EQUAL( stream::ERROR_OK, mClient.Write( buf ) );
            CPPUNIT_ASSERT_EQUAL( stream::ERROR_OK, acceptedClient.Read( buf ) );
            CPPUNIT_ASSERT( std::equal( MESSAGE, MESSAGE + sizeof( MESSAGE ),
                                        buf.begin< char >() ) );

            // Close the sockets.
            CPPUNIT_ASSERT_EQUAL( 0, acceptedClient.Close() );
            CPPUNIT_ASSERT_EQUAL( 0, mClient.Close() );
            CPPUNIT_ASSERT_EQUAL( 0, mServer.Close() );
        }

    protected:
        /// A client socket.
        net::StreamSocket< L3 > mClient;
        /// A server socket.
        net::StreamSocket< L3 > mServer;
    };

    template< typename L3 >
    const char TestNetStreamSocket< L3 >::MESSAGE[] =
        "This is a test message of TestNetStreamSocket.";
}

#endif /* !__TEST__TEST_NET_STREAM_SOCKET_H__INCL__ */
