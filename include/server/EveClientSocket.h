/*
	------------------------------------------------------------------------------------
	LICENSE:
	------------------------------------------------------------------------------------
	This file is part of EVEmu: EVE Online Server Emulator
	Copyright 2006 - 2009 The EVEmu Team
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
	Author:		Captnoord
*/

#ifndef __EVECLIENTSOCKET_H
#define __EVECLIENTSOCKET_H

#define CLIENTSOCKET_SENDBUF_SIZE 0xF4240
#define CLIENTOCKET_RECVBUF_SIZE 0xF4240

/* captnoord personal object debug support */
//#define OBJECT_DUMPER_SUPPORT

/* outPacket queue system, part of a reliable packet sending system */
enum OUTPACKET_RESULT
{
	OUTPACKET_RESULT_SUCCESS				= 1,
	OUTPACKET_RESULT_NO_ROOM_IN_BUFFER		= 2,
	OUTPACKET_RESULT_NOT_CONNECTED			= 3,
	OUTPACKET_RESULT_SOCKET_ERROR			= 4,
};

/* Authorization 'statemachine', based on the old one
 * Currently not used.
 */
enum EVE_AUTH_STATE
{
	EVE_AUTH_STATE_HANDSHAKE,			// old VersionNotReceived					| version handshake
	EVE_AUTH_STATE_QUEUE_COMMAND,		// old WaitingForCommand					| expect queue commands
	EVE_AUTH_STATE_NO_CRYPTO,			// old CryptoRequestNotReceived				| expecting 'crypto' stuff
	EVE_AUTH_STATE_CRYPTO_CHALLENGE,	// old CryptoRequestReceived_ChallengeWait	| 
	EVE_AUTH_STATE_HANDSHAKE_SEND,		// old CryptoHandshakeSent					| waiting for handshake result
	EVE_AUTH_STATE_DONE,				// old AcceptingPackets						| finished authorization
};

class EveClientSession;
class EveClientSocket : public Socket
{
public:
	// state machine function pointer typedef
	typedef void (EveClientSocket::*stateProc)(PyObject* object);

	EveClientSocket(SOCKET fd);
	~EveClientSocket();

	/* socket event handlers */
	void OnRead();
	void OnConnect();
	void OnDisconnect();

	bool sendInt(int number);
	bool sendPyNone();

	/**
	 * \brief sends a object to the client
	 *
	 * this functions marshals and sends the python object to the client.
	 *
	 * @param[in] object is a reference to a PyObject.
	 * @return true if successful and false if not.
	 */
	template<typename T>
	bool send(T &object)
	{
		PyObject * sendObject = (PyObject*)&object;

/* captnoord personal debug stuff */
#ifdef OBJECT_DUMPER_SUPPORT
		printf("server sends:\n");
		DumpObject(stdout, sendObject);
		printf("\n");
#endif//OBJECT_DUMPER_SUPPORT

		WriteStream stream(0x80);
		if(!mMarshal.save(sendObject, stream))
		{
			Log.Error("ClientSocket","send: unable to marshal object");
			return false;
		}

		OUTPACKET_RESULT sendResult = SendRaw(stream.content(), stream.size());
		if (sendResult != OUTPACKET_RESULT_SUCCESS)
		{
			/* @todo add resend and other hacky solutions to a crowded socket system */
			Log.Error("ClientSocket","send: unable to send marshaled object");
			return false;
		}

		return true;
	}

	/**
	 * Function for sending easy packets
	 */
	template <typename T>
	int MarshalSend(T& object)
	{
		PyObject * sendObject = (PyObject*)&object;

		/* some error checking.... but its not good enough */
		if (sendObject == NULL)
		{
			Log.Error("ClientSocket","can't send a NULL object");
			return OUTPACKET_RESULT_SOCKET_ERROR;
		}

		if (sendObject->gettype() >= PyTypeDeleted)
		{
			Log.Error("ClientSocket","object seems to be corrupt.");
			return OUTPACKET_RESULT_SOCKET_ERROR;
		}

		WriteStream stream(0x80);
		if(!mMarshal.save(sendObject, stream))
		{
			Log.Error("ClientSocket","MarshalSend: unable to marshal object");
			return false;
		}

		if(IsConnected() == false)
			return OUTPACKET_RESULT_NOT_CONNECTED;

		size_t len = stream.size();
		unsigned char* data = stream.content();

		BurstBegin();
		if( GetWriteBuffer().GetSpace() < (len + 4) )
		{
			BurstEnd();
			return OUTPACKET_RESULT_NO_ROOM_IN_BUFFER;
		}

		// packet logger :D
		sFileLogger.logPacket(data, len, 1);

		// Pass the size of the packet to our send buffer
		bool rv = BurstSend((const uint8*)&len, 4);

		// Pass the rest of the packet to our send buffer (if there is any)
		if(len > 0 && rv == true)
		{
			rv = BurstSend(data, (uint32)len);
		}

		if(rv == true)
		{
			BurstPush();
		}
		BurstEnd();

		return rv ? OUTPACKET_RESULT_SUCCESS : OUTPACKET_RESULT_SOCKET_ERROR;
	}

	/* function to send RAW data */
	OUTPACKET_RESULT SendRaw(const uint8* data, const size_t len)
	{
		if(IsConnected() == false)
			return OUTPACKET_RESULT_NOT_CONNECTED;

		BurstBegin();
		if( GetWriteBuffer().GetSpace() < (len + 4) )
		{
			BurstEnd();
			return OUTPACKET_RESULT_NO_ROOM_IN_BUFFER;
		}

		// packet logger :D
		sFileLogger.logPacket(data, len, 1);

		// Pass the size of the packet to our send buffer
		bool rv = BurstSend((const uint8*)&len, 4);

		// Pass the rest of the packet to our send buffer (if there is any)
		if(len > 0 && rv == true)
		{
			rv = BurstSend(data, (uint32)len);
		}

		if(rv == true)
		{
			BurstPush();
		}
		BurstEnd();
		return rv ? OUTPACKET_RESULT_SUCCESS : OUTPACKET_RESULT_SOCKET_ERROR;
	}

	/* set the sessions socket */
	void SetSession(EveClientSession* session) { mSession = session; }

	/**
	 * \brief Utility function that generates a PyString containing "ip:port"
	 *
	 * GetAddress converts the remote address ( read server IP + Port ) to a PyString.
	 * The implementation is based on the fact that the snprintf formatting will
	 * maximum use 21 characters. 
	 * strlen("255.255.255.255:65535")
	 * Which translates into a memory chunk of 22 bytes including the '\0'.
	 *
	 * @return PyString if success and NULL if its not.
	 */
	PyString * GetAddress();

protected:
	/************************************************************************/
	/* Python packet send wrapper functions make the code readable          */
	/************************************************************************/
	ASCENT_INLINE void sendConnectionHandShake();
	ASCENT_INLINE void sendCryptoContextAccept();

	/************************************************************************/
	/* Authorization state machine                                          */
	/************************************************************************/
	void _authStateHandshake(PyObject* object);
	void _authStateQueueCommand(PyObject* object);
	void _authStateNoCrypto(PyObject* object);
	void _authStateCryptoChallenge(PyObject* object);
	void _authStateHandshakeSend(PyObject* object);
	void packetHandler(PyObject* object);

	// current state function pointer
	stateProc mCurrentStateMachine;

	/**
	 * resets the connection when we are in authorization mode.
	 */
	void ResetConnection();

private:

	/* Session of this socket */
	EveClientSession* mSession;

	/* User account info */
	AccountInfo* mAccountInfo;

	/* connection/login is queued */
	bool mQueued;

	/* connection is authorized */
	bool mAuthed;

	/* 'TCP nagle' enabled? */
	bool mNagleEanbled;

	/* User name cache */
	std::string mUserName;

	/* OnRead remaining size */
	uint32 mRemaining;

	/* marshal wrapper */
	MarshalStream mMarshal;

	/* send queue
	 * currently not used
	 * TODO implement this, would be easy
	 */
	//FastQueue<WorldPacket*, DummyLock> _queue; // send packet queue not used atm
	//Mutex queueLock;
};

#endif//__EVECLIENTSOCKET_H
