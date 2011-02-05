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
    Author:     Zhur, Bloody.Rabbit
*/

#ifndef __NET__TCP_SERVER_H__INCL__
#define __NET__TCP_SERVER_H__INCL__

#include "mt/Mutex.h"
#include "net/Socket.h"

namespace Net
{
    /**
     * @brief Generic class for TCP server.
     *
     * @author Zhur, Bloody.Rabbit
     */
    class TcpServerBase
    {
    public:
        /// Size of error buffer BaseTcpServer uses.
        static const size_t ERRBUF_SIZE;
        /// Time (in milliseconds) between periodical checks of socket for new connections.
        static const size_t LOOP_GRANULARITY;

        /// Creates empty TCP server.
        TcpServerBase();
        /// Cleans server up.
        virtual ~TcpServerBase();

        /// @return TCP port the server listens on.
        uint16 GetPort() const { return mPort; }
        /// @return True if listening has been opened, false if not.
        bool IsOpen() const;

        /**
         * @brief Start listening on specified port.
         *
         * @param[in]  port   Port on which listening should be started.
         * @param[out] errbuf Error buffer which receives description of error.
         *
         * @retval true  The port has been opened successfully.
         * @retval false Failed to open the port.
         */
        bool Open( uint16 port, char* errbuf = 0 );
        /// Stops started listening.
        void Close();

    protected:
        /**
         * @brief Starts worker thread.
         *
         * This function just starts worker thread; it doesn't check
         * whether there already is one running!
         */
        void StartLoop();
        /// Waits for worker thread to terminate.
        void WaitLoop();

        /**
         * @brief Does periodical stuff to keep the server alive.
         *
         * @retval true  Process more.
         * @retval false Don't process anymore (eg. an error occurred).
         */
        virtual bool Process();
        /// Accepts all new connections.
        void ListenNewConnections();

        /**
         * @brief Processes new connection.
         *
         * This function must be overloaded by children to process new connections.
         * Called every time a new connection is accepted.
         */
        virtual void CreateNewConnection( Socket* sock, uint32 rIP, uint16 rPort ) = 0;

        /**
         * @brief Loop for worker threads.
         *
         * This function only casts arg to BaseTcpServer and calls
         * member TcpServerLoop().
         *
         * @param[in] arg Pointer to BaseTcpServer.
         */
        static thread_return_t TcpServerLoop( void* arg );
        /// Loop for worker threads.
        thread_return_t TcpServerLoop();

        /// Mutex to protect socket and associated variables.
        mutable Mt::Mutex mMSock;
        /// Socket used for listening.
        Socket* mSock;
        /// Port the socket is listening on.
        uint16 mPort;

        /// Worker thread acquires this mutex before it starts processing; used for thread synchronization.
        mutable Mt::Mutex mMLoopRunning;
    };

    /**
     * @brief Connection-type-dependent version of TCP server.
     *
     * @author Zhur, Bloody.Rabbit
     */
    template< typename X >
    class TcpServer
    : public TcpServerBase
    {
    public:
        /// Deletes all stored connections.
        ~TcpServer()
        {
            Mt::MutexLock lock( mMQueue );

            X* conn;
            while( ( conn = PopConnection() ) )
                SafeDelete( conn );
        }

        /**
         * @brief Pops connection from queue.
         *
         * @return Popped connection.
         */
        X* PopConnection()
        {
            Mt::MutexLock lock( mMQueue );

            X* ret = NULL;
            if( !mQueue.empty() )
            {
                ret = mQueue.front();
                mQueue.pop();
            }

            return ret;
        }

    protected:
        /**
         * @brief Adds connection to the queue.
         *
         * @param[in] con Connection to be added to the queue.
         */
        void AddConnection( X* con )
        {
            Mt::MutexLock lock( mMQueue );

            mQueue.push( con );
        }

        /// Mutex to protect connection queue.
        Mt::Mutex mMQueue;
        /// Connection queue.
        std::queue< X* > mQueue;
    };
}

#endif /* !__NET__TCP_SERVER_H__INCL__ */
