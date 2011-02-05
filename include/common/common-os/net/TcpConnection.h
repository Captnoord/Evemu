/**
 * @file net/TcpConnection.h
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
 * @author Zhur, Bloody.Rabbit
 */

#ifndef __COMMON__NET__TCP_CONNECTION_H__INCL__
#define __COMMON__NET__TCP_CONNECTION_H__INCL__

#include "mt/Mutex.h"
#include "net/Socket.h"
#include "util/Buffer.h"

namespace common
{
    namespace net
    {
        /**
         * @brief Generic class for TCP connections.
         *
         * @author Zhur, Bloody.Rabbit
         */
        class TcpConnection
        {
        public:
            /// Size of error buffer TcpConnection uses.
            static const size_t ERRBUF_SIZE = 1024;
            /// Size of receive buffer TcpConnection uses.
            static const size_t RECVBUF_SIZE;
            /// Time (in milliseconds) between periodical process for incoming/outgoing data.
            static const size_t LOOP_GRANULARITY;

            /// Describes all states this object may be in.
            enum State
            {
                STATE_DISCONNECTED, ///< No connection.
                STATE_CONNECTING,   ///< Connection pending (asynchronous connection).
                STATE_CONNECTED,    ///< Connection established, transferring data.
                STATE_DISCONNECTING ///< Disconnect pending, waiting for all data to be sent.
            };

            /// Creates new connection in STATE_DISCONNECTED.
            TcpConnection();
            /// Cleans connection up.
            virtual ~TcpConnection();

            /// @return Remote IP.
            uint32 GetrIP() const     { return mrIP; }
            /// @return Remote port.
            uint16 GetrPort() const { return mrPort; }
            /// @return String in format "<remote_address>:<remote_port>".
            std::string GetAddress();
            /// @return Current state of connection.
            State GetState() const { return mSockState; }

            /**
             * @brief Connects to specified address.
             *
             * This function does synchronous connect, ie. blocks calling thread
             * until connect either succeeds or fails. Benefit is that outcome
             * of connect is known immediately.
             *
             * @param[in]  rIP    Target remote IP address.
             * @param[in]  rPort  Target remote TCP port.
             * @param[out] errbuf String buffer which receives error desription.
             *
             * @retval true  Connection succeeded.
             * @retval false Connection failed.
             */
            bool Connect( uint32 rIP, uint16 rPort, char* errbuf = NULL );
            /**
             * @brief Schedules asynchronous connect to specified address.
             *
             * This function does asynchronous connect, ie. does not block
             * calling thread at all. However, result of connect is not
             * known immediately.
             *
             * @param[in] rIP   Target remote IP address.
             * @param[in] rPort Target remote TCP port.
             */
            void AsyncConnect( uint32 rIP, uint16 rPort );
            /**
             * @brief Schedules disconnect of current connection.
             *
             * Connection will be closed as soon as possible. Note that
             * this may take some time since we wait for emptying send
             * queue before actually disconnecting.
             */
            void Disconnect();

            /**
             * @brief Enqueues data to be sent.
             *
             * @param[in] data A buffer with data; the pointer
             *                 is invalidated by the function.
             *
             * @retval true  Data successfully enqueued.
             * @retval false Failed to enqueue data.
             */
            bool Send( util::Buffer** data );

        protected:
            /**
             * @brief Creates connection from an existing socket.
             *
             * @param[in] sock  Socket to be used for connection.
             * @param[in] rIP   Remote IP socket is connected to.
             * @param[in] rPort Remote TCP port socket is connected to.
             */
            TcpConnection( Socket* sock, uint32 rIP, uint16 rPort );

            /**
             * @brief Starts working thread.
             *
             * This function just starts a thread, does not check
             * whether there is already one running!
             */
            void StartLoop();
            /// Blocks calling thread until working thread terminates.
            void WaitLoop();

            /**
             * @brief Does all stuff that needs to be periodically done to keep connection alive.
             *
             * @retval true  Process more.
             * @retval false Don't process anymore (eg. disconnected).
             */
            virtual bool Process();
            /**
             * @brief Processes received data.
             *
             * This function must be overloaded by children to process received data.
             * Called every time a chunk of new data is received. Please note that
             * receive buffer is overwritten every time data is received.
             *
             * @param[out] errbuf Buffer which receives description of error.
             *
             * @retval true  Processing succeeded.
             * @retval false Failed to process data.
             */
            virtual bool ProcessReceivedData( char* errbuf = NULL ) = 0;

            /**
             * @brief Sends data in send queue.
             *
             * @param[out] errbuf Buffer which receives desription of error.
             *
             * @retval true  Data sent successfully.
             * @retval false Failed to send data.
             */
            virtual bool SendData( char* errbuf = NULL );
            /**
             * @brief Receives data and puts them into receive queue.
             *
             * @param[out] errbuf Buffer which receives description of error.
             *
             * @retval true  Receive OK.
             * @retval false Failure during receiving.
             */
            virtual bool RecvData( char* errbuf = NULL );
            /// Disconnects socket.
            void DoDisconnect();

            /// Clears send and receive buffers.
            virtual void ClearBuffers();

            /**
             * @brief Loop for worker threads.
             *
             * This function just casts given arg into TcpConnection and calls
             * member TcpConnectionLoop.
             *
             * @param[in] arg Pointer to TcpConnection.
             */
            static thread_return_t TcpConnectionLoop( void* arg );
            /// Loop for worker threads.
            thread_return_t TcpConnectionLoop();

            /// Protection of socket and associated variables.
            mutable mt::Mutex mMSock;
            /// Socket for connection.
            Socket* mSock;
            /// State the socket is in.
            State mSockState;
            /// Remote IP the socket is connected to.
            uint32 mrIP;
            /// Remote TCP port the socket is connected to; is in host byte order.
            uint16 mrPort;

            /// When a thread is running TcpConnectionLoop, it acquires this mutex first; used for synchronization.
            mutable mt::Mutex mMLoopRunning;

            /// Mutex protecting send queue.
            mutable mt::Mutex mMSendQueue;
            /// Send queue.
            std::deque< util::Buffer* > mSendQueue;

            /// Receive buffer.
            util::Buffer* mRecvBuf;
        };
    }
}

#endif /* !__COMMON__NET__TCP_CONNECTION_H__INCL__ */
