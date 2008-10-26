/*
	------------------------------------------------------------------------------------
	LICENSE:
	------------------------------------------------------------------------------------
	This file is part of EVEmu: EVE Online Server Emulator
	Copyright 2003 - 2008 The EVEmu Team
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

#include "EvemuPCH.h"

#include "ByteBuffer.h"

#define GetConnectedAddress GetRemoteIP

// lol at handshake function....
static const byte handshakeFunc[] = {
	0x74, 0x04, 0x00, 0x00, 0x00, 0x4E, 0x6F, 0x6E, 0x65	//marshaled Python string "None"
};

EveClientSocket::EveClientSocket(SOCKET fd) : Socket(fd, CLIENTSOCKET_SENDBUF_SIZE, CLIENTOCKET_RECVBUF_SIZE)
{
	Authed = false;
	//mSize = mOpcode = 
	mRemaining = 0;
	mRequest = NULL;
	//_latency = 0;
	//mSession = NULL;
	//mSeed = rand() % 0xFFFFFFF0 + 10;
	//pAuthenticationPacket = NULL;
	//mQueued = false;
	//mRequestID = 0;
	m_nagleEanbled = false;
	//m_fullAccountName = NULL;

	currentStateMachine = &EveClientSocket::_authStateHandshake;
}

EveClientSocket::~EveClientSocket()
{
	//WorldPacket * pck;
	//while((pck = _queue.Pop()))
	//	delete pck;

	//if(pAuthenticationPacket)
	//	delete pAuthenticationPacket;

	//if(mSession)
	//{
	//	mSession->SetSocket(NULL);
	//	mSession=NULL;
	//}

	//if( m_fullAccountName != NULL )
	//{
	//	delete m_fullAccountName;
	//	m_fullAccountName = NULL;
	//}
}

void EveClientSocket::OnDisconnect()
{
	Log.Debug("EVE network","OnDisconnect");
	sSpace.mAcceptedConnections--;
	/*if(mSession)
	{
		mSession->SetSocket(0);
		mSession=NULL;
	}

	if(mRequestID != 0)
	{
		sLogonCommHandler.UnauthedSocketClose(mRequestID);
		mRequestID = 0;
	}*/
}

// sends initial handshake containing the version info
void EveClientSocket::_sendHandShake()
{
	//uint32 connectionCount = sSpaceCore.GetConnectionCount();

	VersionExchange version;
	version.birthday = EVEBirthday;
	version.macho_version = MachoNetVersion;
	version.user_count = 1;//connectionCount;
	version.version_number = EVEVersionNumber;
	version.build_version = EVEBuildVersion;
	version.project_version = EVEProjectVersion;

	//_log(NET__PRES_REP_OUT, "%s: Sending Low Level Version Exchange:", GetConnectedAddress().c_str());
	//version.Dump(NET__PRES_REP_OUT, "    ");

	PyRep *rep = version.Encode();

	EVENetPacket *packet = new EVENetPacket;
	packet->data = Marshal(rep, packet->length);

	_outPacket(packet);

	delete packet;
}

//send position in queue
void EveClientSocket::_sendQueuePos( int queuePos )
{
	// send a python integer back as a response on the queue query
	/*PyRepInteger CurrentQueuePos( queuePos );

	EVENetPacket * packet = new EVENetPacket;
	packet->data = Marshal(&CurrentQueuePos, packet->length); // mem leak?
	
	_outPacket(packet);

	delete packet;*/

	PyRepInteger packet(queuePos);
	OutPacket(&packet);

}

// send handshake accept
void EveClientSocket::_sendAccept()
{
	PyRepString packet("OK CC");

	OutPacket(&packet);
}

void EveClientSocket::_sendRequirePasswordType(int passwordType)
{
	PyRepInteger packet(passwordType);
	OutPacket(&packet);
}

OUTPACKET_RESULT EveClientSocket::_outPacket(EVENetPacket * packet)
{
	uint32 len = packet->length;

	bool rv;
	if(!IsConnected())
		return OUTPACKET_RESULT_NOT_CONNECTED;

	BurstBegin();
	if( GetWriteBuffer().GetSpace() < (len+4) )
	{
		BurstEnd();
		return OUTPACKET_RESULT_NO_ROOM_IN_BUFFER;
	}

	// Packet logger :)
	//sWorldLog.LogPacket((uint32)len, opcode, (const uint8*)data, 1);

	// Pass the size of the packet to our send buffer
	rv = BurstSend((const uint8*)&len, 4);

	// Pass the rest of the packet to our send buffer (if there is any)
	if(len > 0 && rv == true)
	{
		rv = BurstSend((const uint8*)packet->data, (uint32)len);
	}

	if(rv == true)
	{
		BurstPush();
	}
	BurstEnd();
	return rv ? OUTPACKET_RESULT_SUCCESS : OUTPACKET_RESULT_SOCKET_ERROR;
}

void EveClientSocket::OutPacket(PyRep * packet )
{
	EVENetPacket * netData = new EVENetPacket;
	netData->data = Marshal(packet, netData->length); // mem leak? in returned stuff from marshal?

	_outPacket(netData);

	delete netData;
}

void EveClientSocket::OnConnect()
{
	sSpace.mAcceptedConnections++;
	//_latency = getMSTime();

	Log.Debug("EVE network","OnConnect");
	_sendHandShake();
}

void EveClientSocket::OnRead()
{
	for(;;)
	{
		uint32 packetSize = 0;
		//Check for the header if we don't have any bytes to wait for.
		if(mRemaining == 0)
		{
			if(GetReadBuffer().GetSize() < 4)
			{
				// No header in the packet, let's wait.
				return;
			}

			// copy the packet size from the buffer
			GetReadBuffer().Read((uint8*)&packetSize, 4);

			mRemaining = packetSize;
		}

		//WorldPacket * Packet;

		if(mRemaining > 0)
		{
			if( GetReadBuffer().GetSize() < mRemaining )
			{
				// We have a fragmented packet. Wait for the complete one before proceeding.
				return;
			}
		}

		ByteBuffer * packet = NULL;

		//Packet = new WorldPacket(mOpcode, mSize);
		//Packet->resize(mSize);

		if(mRemaining > 0)
		{
			packet = new ByteBuffer;
			packet->resize(mRemaining);

			// Copy from packet buffer into our actual buffer.
			//GetReadBuffer().Read((uint8*)packet->contents(), mRemaining);
			GetReadBuffer().Read(packet->data(), mRemaining);
			mRemaining = 0;
		}

		// packet log
		//sWorldLog.LogPacket(mSize, mOpcode, mSize ? Packet->contents() : NULL, 0);

		printf("\nRecv:\n");
		//packet->hexlike();
		packet->LogPacket();

		PyRep *r = InflateAndUnmarshal(packet->contents(), packet->size());

		// whoo state machine magic
		(this->*currentStateMachine)(*r);

		// this is the end of the road
		if ( packet != NULL)
		{
			delete packet;
		}
	}
}

// the client sends back its server info...
// we should compare this with our own to make sure we can block unsupported clients
void EveClientSocket::_authStateHandshake(PyRep& packet)
{
	VersionExchange ve;

	PyRep *fp = &packet;
	if(!ve.Decode(&fp)) {
		//_log(NET__PRES_ERROR, "%s: Received invalid version exchange!", GetConnectedAddress().c_str());
		return;
	}

	Log.Debug("ClientSocket","%s: Received Low Level Version Exchange:\n", GetRemoteIP().c_str());

	//_log(NET__PRES_REP, "%s: Received Low Level Version Exchange:\n", GetConnectedAddress().c_str());
	//ve.Dump(NET__PRES_REP, "    ");

	if(ve.birthday != EVEBirthday) {
		//_log(NET__PRES_ERROR, "%s: Client's birthday does not match ours!", GetConnectedAddress().c_str());
	}
	if(ve.macho_version != MachoNetVersion) {
		//_log(NET__PRES_ERROR, "%s: Client's macho_version not match ours!", GetConnectedAddress().c_str());
	}
	if(ve.version_number != EVEVersionNumber) {
		//_log(NET__PRES_ERROR, "%s: Client's version_number not match ours!", GetConnectedAddress().c_str());
	}
	if(ve.build_version != EVEBuildVersion) {
		//_log(NET__PRES_ERROR, "%s: Client's build_version not match ours!", GetConnectedAddress().c_str());
	}
	if(ve.project_version != EVEProjectVersion) {
		//_log(NET__PRES_ERROR, "%s: Client's project_version not match ours!", GetConnectedAddress().c_str());
	}

	Log.Debug("AuthStateMachine","State changed into StateQueueCommand");
	currentStateMachine = &EveClientSocket::_authStateQueueCommand;
}

void EveClientSocket::_authStateQueueCommand(PyRep& packet)
{
	//check if it actually is tuple
	if(!packet.CheckType(PyRep::Tuple)) {
		_log(NET__PRES_ERROR, "%s: Invalid packet during waiting for command (tuple expected).", GetConnectedAddress().c_str());
		return;
	}
	PyRepTuple *t = (PyRepTuple *)&packet;
	
	//decode
	if(t->items.size() == 2)
	{
		//QC = Queue Check
		NetCommand_QC cmd;
		if(!cmd.Decode(&t)) {
			_log(NET__PRES_ERROR, "%s: Failed to decode 2-arg command.", GetConnectedAddress().c_str());
			return;//break;
		}
		if(cmd.command != "QC") {
			_log(NET__PRES_ERROR, "%s: Unknown 2-arg command '%s'.", GetConnectedAddress().c_str(), cmd.command.c_str());
			return;//break;
		}
		_log(NET__PRES_DEBUG, "%s: Got Queue Check command.", GetConnectedAddress().c_str());
		

		//return position in queue, which is 1 for now until we implemented a real login queue
		_sendQueuePos(1);

		//now act like client just connected
		//send out handshake again
		_sendHandShake();

		//and set proper state
		Log.Debug("AuthStateMachine","State changed into StateHandshake");
		currentStateMachine = &EveClientSocket::_authStateHandshake;
		return;
	} 
	else if(t->items.size() == 3) 
	{
		//this is sent when client is logging in
		NetCommand_VK cmd;
		if(!cmd.Decode(&t)) {
			_log(NET__PRES_ERROR, "%s: Failed to decode 3-arg command.", GetConnectedAddress().c_str());
			return;//break;
		}
		if(cmd.command != "VK") {
			_log(NET__PRES_ERROR, "%s: Unknown 3-arg command '%s'.", GetConnectedAddress().c_str(), cmd.command.c_str());
			return;//break;
		}
		_log(NET__PRES_DEBUG, "%s: Got VK command, vipKey=%s.", GetConnectedAddress().c_str(), cmd.vipKey.c_str());
		//m_state = CryptoRequestNotReceived;
		Log.Debug("AuthStateMachine","State changed into StateStateNoCrypto");
		currentStateMachine = &EveClientSocket::_authStateNoCrypto;
		return;
	}
	else 
	{
		Log.Debug("EVE Socket","StateQueueCommand received a invalid packet");
		_log(NET__PRES_ERROR, "%s: Received invalid command packet:", GetConnectedAddress().c_str());
		packet.Dump(NET__PRES_ERROR, "	");
	}
}

void EveClientSocket::_authStateNoCrypto(PyRep& packet)
{
	CryptoRequestPacket cr;

	PyRep * data = &packet;

	if(!cr.Decode(&data)) {
		_log(NET__PRES_ERROR, "%S: Received invalid crypto request!", GetConnectedAddress().c_str());
		return;//break;
	}

	if(cr.keyVersion == "placebo")
	{
		_log(NET__PRES_DEBUG, "%s: Received Placebo crypto request, accepting.", GetConnectedAddress().c_str());
		//m_state = CryptoRequestReceived_ChallengeWait;
		Log.Debug("AuthStateMachine","State changed into StateCryptoChallenge");
		currentStateMachine = &EveClientSocket::_authStateCryptoChallenge;

		_sendAccept();
	}
	else
	{
		//im sure cr.keyVersion can specify either CryptoAPI or PyCrypto, but its all binary so im not sure how.
		PyRep *params = cr.keyParams.Clone();
		CryptoAPIRequestParams car;
		if(!car.Decode(&params)) {
			_log(NET__PRES_ERROR, "%s: Received invalid CryptoAPI request!", GetConnectedAddress().c_str());
			return;
			//break;
		}

		_log(NET__PRES_ERROR, "%s: Unhandled CryptoAPI request: hashmethod=%s sessionkeylength=%d provider=%s sessionkeymethod=%s", GetConnectedAddress().c_str(), car.hashmethod.c_str(), car.sessionkeylength, car.provider.c_str(), car.sessionkeymethod.c_str());
		_log(NET__PRES_ERROR, "%s: You must change your client to use Placebo crypto in common.ini to talk to this server!\n", GetConnectedAddress().c_str());
	}
}

void EveClientSocket::_authStateCryptoChallenge(PyRep& packet)
{
	//just to be sure
	if(mRequest != NULL)
		delete mRequest;
	mRequest = new CryptoChallengePacket;

	//CryptoChallengePacket m_request;

	

	PyRep* data = &packet;

	if(!mRequest->Decode(&data)) {
		_log(NET__PRES_ERROR, "%s: Received invalid crypto challenge!", GetConnectedAddress().c_str());
		return;//break;
	}

	_log(NET__PRES_DEBUG, "%s: Received Client Challenge.", GetConnectedAddress().c_str(), mRequest->user_name.c_str());

	if(mRequest->user_password->CheckType(PyRep::None)) {
		//this is little wrong because on live they send password version always, but only once,
		//but we send password version when we get request with hashed password ...
		_log(NET__PRES_DEBUG, "%s: Got hashed password, requesting plain.", GetConnectedAddress().c_str());
		//delete m_request;
		//m_request = NULL;

		//send passwordVersion required: 1=plain, 2=hashed
		//PyRep *r = new PyRepInteger(1);
		//_QueueRep(r);
		//delete r;

		_sendRequirePasswordType(1);

		return;//break;
	}

	_log(NET__PRES_DEBUG, "%s:		username='%s'", GetConnectedAddress().c_str(), mRequest->user_name.c_str());

	//send our handshake
	CryptoServerHandshake server_shake;
	server_shake.serverChallenge = "hi";
	server_shake.func_marshaled_code = new PyRepBuffer(handshakeFunc, sizeof(handshakeFunc));
	server_shake.macho_version = MachoNetVersion;
	server_shake.boot_version = EVEVersionNumber;
	server_shake.boot_build = EVEBuildVersion;
	server_shake.boot_codename = EVEProjectCodename;
	server_shake.boot_region = EVEProjectRegion;
	server_shake.cluster_usercount = 1;//m_userCount;
	server_shake.proxy_nodeid = 0xFFAA;
	server_shake.user_logonqueueposition = 1;
	server_shake.challenge_responsehash = "654"; // binascii.crc_hqx of 64 zero bytes in a string, in a single element tuple, marshaled

	data = server_shake.Encode();
	OutPacket(data);
	delete data;
	
	//m_state = CryptoHandshakeSent;
	Log.Debug("AuthStateMachine","State changed into HandservershakeSend");
	currentStateMachine = &EveClientSocket::_authStateHandshakeSend;
}

void EveClientSocket::_authStateHandshakeSend(PyRep& packet)
{
	CryptoHandshakeResult hr;
	PyRep * data = &packet;
	if(!hr.Decode(&data)) {
		_log(NET__PRES_ERROR, "%s: Received invalid crypto handshake result!", GetConnectedAddress().c_str());
		return;//break;
	}

	//this is a bit crappy ...
	//client->Login(m_request);

	/* now comes code that should be in the client class, but as we are hacking anyway why care at all */
	_log(CLIENT__MESSAGE, "Login with %s", mRequest->user_name.c_str());

	if(!mRequest->user_password->CheckType(PyRep::PackedObject2)) {
		_log(CLIENT__ERROR, "Failed to get password: user password is not PackedObject2.");
		return;
	}
	PyRepPackedObject2 *obj = (PyRepPackedObject2 *)mRequest->user_password;
	//we can check type, should be "util.PasswordString"

	Call_SingleStringArg pass;
	if(!pass.Decode(&obj->args1)) {
		_log(CLIENT__ERROR, "Failed to decode password.");
		return;
	}

	// db check stuff
	/*if(!m_services->GetServiceDB()->DoLogin(mRequest->user_name.c_str(), pass.arg.c_str(), m_accountID, m_role)) {
		_log(CLIENT__MESSAGE, "%s: Login rejected by DB", mRequest->user_name.c_str());

		PyRepPackedObject1 *e = new PyRepPackedObject1("exceptions.GPSTransportClosed");
		e->args = new PyRepTuple(1);
		e->args->items[0] = new PyRepString("LoginAuthFailed");

		//throw(PyException(e));
	}*/

	// this is needed so if we exit before selecting a character, the account online flag would switch back to 0
	//m_char.charid = 0;
	//m_services->GetServiceDB()->SetAccountOnlineStatus(m_accountID, true);

	//send this before session change
	CryptoHandshakeAck ack;
	ack.connectionLogID = 1;	//TODO: what is this??
	ack.jit = mRequest->user_languageid;
	ack.userid = 1337;//m_accountID;
	ack.maxSessionTime = new PyRepNone;
	ack.userType = 1;	//TODO: what is this??
	ack.role = 1;//m_role;
	ack.address = GetConnectedAddress();//m_net.GetConnectedAddress();
	ack.inDetention = new PyRepNone;
	ack.user_clientid = 42;//m_accountID;


	
	PyRep * ectpack = ack.Encode();
	OutPacket(ectpack);
	delete ectpack;

	//m_net._QueueRep(ack.Encode());

	//session.Set_userType(1);	//TODO: what is this??
	//session.Set_userid(m_accountID);
	//session.Set_address(m_net.GetConnectedAddress().c_str());
	//session.Set_role(m_role);
	//session.Set_languageID(pack->user_languageid.c_str());

	//_CheckSessionChange();

	/* end of the evil hacking */



	delete mRequest;
	mRequest = NULL;

	//m_state = AcceptingPackets;
	Log.Debug("AuthStateMachine","State changed into StateDone");
	currentStateMachine = &EveClientSocket::_authStateDone;
}

void EveClientSocket::_authStateDone(PyRep& packet)
{
	Log.Debug("ClientSocket","received packet whooo we passed authorization");
}
