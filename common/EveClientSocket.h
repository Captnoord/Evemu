/*
	------------------------------------------------------------------------------------
	LICENSE:
	------------------------------------------------------------------------------------
	This file is part of EVEmu: EVE Online Server Emulator
	Copyright 2007 - 2008 The EVEmu Team
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

// some lame buffer size stuff
#define CLIENTSOCKET_SENDBUF_SIZE 131078
#define CLIENTOCKET_RECVBUF_SIZE 16384

// 'outpacket' queue system, part of a reliable packet sending system
enum OUTPACKET_RESULT
{
	OUTPACKET_RESULT_SUCCESS				= 1,
	OUTPACKET_RESULT_NO_ROOM_IN_BUFFER		= 2,
	OUTPACKET_RESULT_NOT_CONNECTED			= 3,
	OUTPACKET_RESULT_SOCKET_ERROR			= 4,
};

//authorize state machine
enum EVE_AUTH_STATE
{
	EVE_AUTH_STATE_HANDSHAKE,			// old VersionNotReceived					| version handshake
	EVE_AUTH_STATE_QUEUE_COMMAND,		// old WaitingForCommand					| expect queue commands
	EVE_AUTH_STATE_NO_CRYPTO,			// old CryptoRequestNotReceived				| expecting 'crypto' stuff
	EVE_AUTH_STATE_CRYPTO_CHALLENGE,	// old CryptoRequestReceived_ChallengeWait	| 
	EVE_AUTH_STATE_HANDSHAKE_SEND,		// old CryptoHandshakeSent					| waiting for handshake result
	EVE_AUTH_STATE_DONE,				// old AcceptingPackets						| finished authorization
};

class EveClientSocket : public Socket
{
public:
	// state machine function pointer typedef
	typedef void (EveClientSocket::*stateProc)(PyRep&);

	EveClientSocket(SOCKET fd);
	~EveClientSocket();

	ASCENT_INLINE void HandleHandShake();

	// basic PyRep send function
	ASCENT_INLINE void OutPacket(PyRep * packet );

	void OnRead();
	void OnConnect();
	void OnDisconnect();

	bool m_Authed;

protected:
	// Internal functions

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
	ASCENT_INLINE OUTPACKET_RESULT _outPacket(EVENetPacket * packet);

	/************************************************************************/
	/* Authorization state machine                                          */
	/************************************************************************/
	void _authStateHandshake(PyRep& packet);
	void _authStateQueueCommand(PyRep& packet);
	void _authStateNoCrypto(PyRep& packet);
	void _authStateCryptoChallenge(PyRep& packet);
	void _authStateHandshakeSend(PyRep& packet);
	void _authStateDone(PyRep& packet);

	
	stateProc m_currentStateMachine;

private:

	uint32 mRemaining;
	CryptoChallengePacket *mRequest;

	/*uint32 mOpcode;
	
	uint32 mSize;
	uint32 mSeed;
	uint32 mClientSeed;
	uint32 mClientBuild;
	uint32 mRequestID;*/

	//WorldSession *mSession;
	//WorldPacket * pAuthenticationPacket;
	//FastQueue<WorldPacket*, DummyLock> _queue;
	Mutex queueLock;

//	uint32 _latency;
	bool m_nagleEanbled;
//	string * m_fullAccountName;
};