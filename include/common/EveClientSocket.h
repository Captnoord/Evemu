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
	Author:		Captnoord
*/

#ifndef __EVECLIENTSOCKET_H
#define __EVECLIENTSOCKET_H

#define CLIENTSOCKET_SENDBUF_SIZE 0xF4240
#define CLIENTOCKET_RECVBUF_SIZE 0xF4240

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
	typedef void (EveClientSocket::*stateProc)(PyRep*);

	EveClientSocket(SOCKET fd);
	~EveClientSocket();

	/* socket event handlers */
	void OnRead();
	void OnConnect();
	void OnDisconnect();

	/* Basic PyRep send function */
	ASCENT_INLINE void OutPacket(PyRep * packet);

	/* set the sessions socket */
	void SetSession(EveClientSession* session) { mSession = session; }

protected:
	/************************************************************************/
	/* Python packet send wrapper functions make the code readable          */
	/************************************************************************/
	ASCENT_INLINE void _sendHandShake();
	ASCENT_INLINE void _sendQueuePos( int queuePos );
	ASCENT_INLINE void _sendAccept();
	ASCENT_INLINE void _sendRequirePasswordType(int passwordType);

	/************************************************************************/
	/* raw Python packet send function                                      */
	/************************************************************************/
	ASCENT_INLINE OUTPACKET_RESULT _outPacket(EVENetPacket* packet);

	/************************************************************************/
	/* Authorization state machine                                          */
	/************************************************************************/
	void _authStateHandshake(PyRep* packet);
	void _authStateQueueCommand(PyRep* packet);
	void _authStateNoCrypto(PyRep* packet);
	void _authStateCryptoChallenge(PyRep* packet);
	void _authStateHandshakeSend(PyRep* packet);
	void _authStateDone(PyRep* packet);
	void _authStateException(PyRep* packet);

	// current state function pointer
	stateProc mCurrentStateMachine;

private:
	/* Crypt challenge cache */
	CryptoChallengePacket* mRequest;

	/* Session of this socket */
	EveClientSession* mSession;

	/* connection/login is queued */
	bool mQueued;

	/* connection is authorized */
	bool mAuthed;

	/* 'TCP nagle' enabled? */
	bool mNagleEanbled;

	/* User name cache */
	std::string mUserName;

	/* User id cache */
	uint32 mUserid;

	/* OnRead remaining size */
	uint32 mRemaining;

	/* send queue
	 * currently not used
	 * TODO implement this, would be easy
	 */
	//FastQueue<WorldPacket*, DummyLock> _queue; // send packet queue not used atm
	//Mutex queueLock;
};

#endif//__EVECLIENTSOCKET_H
