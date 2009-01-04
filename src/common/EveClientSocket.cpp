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

#include "EvemuPCH.h"
#include "ByteBuffer.h"
//#include "ShaModule.h"
#include "PasswordModule.h"

// Dummy authorization handshake function
static const uint8 handshakeFunc[] = {
	0x74, 0x04, 0x00, 0x00, 0x00, 0x4E, 0x6F, 0x6E, 0x65	//marshaled Python string "None"
};

EveClientSocket::EveClientSocket(SOCKET fd) : Socket(fd, CLIENTSOCKET_SENDBUF_SIZE, CLIENTOCKET_RECVBUF_SIZE)
{
	mAuthed = false;
	mQueued = false;			// don't support authorization queue for now
	mNagleEanbled = false;		// don't enable 'nagle' for now

	mRemaining = 0;

	mSession = NULL;

	mCurrentStateMachine = &EveClientSocket::_authStateHandshake;
}

EveClientSocket::~EveClientSocket()
{
	/* delete the buffers in the send buffer queue */
	// disabled for now as we don't have a send queue yet
	//WorldPacket * pck;
	//while((pck = _queue.Pop()))
	//	delete pck;

	/* delete the 'auth' request packet if it still exists */
	//if (mRequest != NULL)
	//	delete mRequest;

	/* remove the link between the current socket and its client session */
	if (mSession)
	{
		mSession->SetSocket(NULL);
		mSession = NULL;
	}
}

void EveClientSocket::OnDisconnect()
{
	sSpace.OnClientDisconnect();
	
	if(mSession != NULL)
	{
		mSession->SetSocket(0);
		mSession = NULL;
	}
}

// sends initial handshake containing the version info
void EveClientSocket::_sendHandShake()
{
	uint32 authCount = (uint32)sSpace.GetAuthorizedCount();

	/* version response packet */
	PyTupleStream packet(45);
	packet << EveBirthday;
	packet << MachoNetVersion;
	packet << authCount;
	packet << EveVersionNumber;
	packet << EveBuildVersion;
	packet << EveProjectVersion;

	SendStream(packet);
}

//send position in queue
void EveClientSocket::_sendQueuePos( int queuePos )
{
	// send a python integer back as a response on the queue query
	PyIntStream out(queuePos);
	SendStream(out);
}

// 'auth' commands
//OK = Reading Client 'Crypto' Context OR Queue Check
//OK CC = 'Crypto' Context Complete
//CC = 'Crypto' Context

/* send handshake accept */
void EveClientSocket::_sendAccept()
{
	PyStringStream out("OK CC"); // doesn't seem to matter what we send... (at this point)
	SendStream(out);
}

/* send a python integer as a response on the password type query */
void EveClientSocket::_sendRequirePasswordType(int passwordType)
{
	PyIntStream out(passwordType);
	SendStream(out);
}

/* low level packet sending, uses burst */
OUTPACKET_RESULT EveClientSocket::_outPacket(EVENetPacket * packet)
{
	/*uint32 len = packet->length;

	//ByteBuffer packetcheck;
	//packetcheck.append(packet->data, packet->length);

	//printf("\nsend:\n");
	//packetcheck.LogBuffer();

	if(IsConnected() == false)
		return OUTPACKET_RESULT_NOT_CONNECTED;

	BurstBegin();
	if( GetWriteBuffer().GetSpace() < (len+4) )
	{
		BurstEnd();
		return OUTPACKET_RESULT_NO_ROOM_IN_BUFFER;
	}

	// Packet logger :)
	//sSpaceLog.LogBuffer((uint32)len, opcode, (const uint8*)data, 1);

	// Pass the size of the packet to our send buffer
	bool rv = BurstSend((const uint8*)&len, 4);

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
	return rv ? OUTPACKET_RESULT_SUCCESS : OUTPACKET_RESULT_SOCKET_ERROR;*/
	return OUTPACKET_RESULT_SUCCESS; // hack
}

// TODO loose the new packet and delete packet stuff..
void EveClientSocket::OutPacket(PyRep * packet )
{
	/*EVENetPacket * netData = new EVENetPacket;
	netData->data = Marshal(packet, netData->length);
	_outPacket(netData);
	SafeDelete(netData);*/
}

void EveClientSocket::OnConnect()
{
	sSpace.OnClientConnect();
	//_latency = getMSTime();

	/* send initial 'auth' handshake */
	_sendHandShake();
}

void EveClientSocket::OnRead()
{
	for(;;)
	{
		//Check for the header if we don't have any bytes to wait for.
		if(mRemaining == 0)
		{
			if(GetReadBuffer().GetSize() < 4)
			{
				// No header in the packet, let's wait.
				return;
			}

			// copy the packet size from the buffer
			uint32 packetSize;
			GetReadBuffer().Read((void*)&packetSize, 4);
			mRemaining = packetSize;
		}

		if(mRemaining > 0)
		{
			if( GetReadBuffer().GetSize() < mRemaining )
			{
				// We have a fragmented packet. Wait for the complete one before proceeding.
				return;
			}
		}

		//ByteBuffer * packet = NULL;
		PyNetworkStream * packet = NULL;

		/* TODO we can skip this new ByteBuffer by feeding the circular buffer into the 'unmarshal' function
		 * requires to redesign the 'unmarshal' function
		 */
		if(mRemaining > 0)
		{
			packet = new PyNetworkStream(mRemaining);

			// Copy from packet buffer into our actual buffer.
			GetReadBuffer().Read(packet->content(), mRemaining);
			mRemaining = 0;
		}

		// packet log
		sFileLogger.logPacket(packet->content(), packet->size(), 0);

		/* IMPORTANT TODO, InflateAndUnmarshal should not be in the socket, as when receiving big packets this could block the
		 * network core. So move it somehow to the session class, also as its recursive it can run forever.
		 */
		//PyRep *recvPyPacket = InflateAndUnmarshal(packet->contents(), (uint32)packet->size());

		// printf("\nrecv packet with opcode:%d and Type:%s and size:%d\n", ((PyPacket*)recvPyPacket)->type, recvPyPacket->TypeString(), packet->size());
		// packet->LogBuffer();

		// the state machine magic
		//(this->*mCurrentStateMachine)(recvPyPacket);
		(this->*mCurrentStateMachine)(*packet);

		// this is the end of the road for the used read buffer
		if ( packet != NULL)
		{
			SafeDelete(packet);
		}
	}
}


/* the client sends back its server info...
 * we should compare this with our own to make sure we can block unsupported clients.
 * I guess that would never happen unless you have a modified client. And even than its possible.
 */
void EveClientSocket::_authStateHandshake(PyNetworkStream& packet)
{
	Log.Debug(__FUNCTION__, "hand shaking...");

	PyTupleNetStream data(packet);

	uint16 MachoVersion;
	double VersionNr;
	uint32 BuildVersion;
	std::string Projectversion;
	uint32 Birthday;
	uint32 usercount; // what we send the client....

	data >> Birthday;
	data >> MachoVersion;
	data >> usercount;
	data >> VersionNr;
	data >> BuildVersion;
	data >> Projectversion;
	
	/*VersionExchange data;
	if(data.Decode(&packet) == false)
	{
		//Log.Debug("%s: Received invalid version exchange!", GetRemoteIP().c_str());
		Disconnect();
		return;
	}

	if(data.birthday != EveBirthday)
	{
		//sLog.Debug("%s: Client's birthday does not match ours!", GetRemoteIP().c_str());
		Disconnect();
	}

	if(data.macho_version != MachoNetVersion)
	{
		//sLog.Debug("%s: Client's macho_version not match ours!", GetRemoteIP().c_str());
		Disconnect();
	}

	if(data.version_number != EveVersionNumber)
	{
		//sLog.Debug("%s: Client's version_number not match ours!", GetRemoteIP().c_str());
		Disconnect();
	}

	if(data.build_version != EveBuildVersion)
	{
		//sLog.Debug("%s: Client's build_version not match ours!", GetRemoteIP().c_str());
		Disconnect();
	}

	if(data.project_version != EveProjectVersion)
	{
		//sLog.Debug("%s: Client's project_version not match ours!", GetRemoteIP().c_str());
		Disconnect();
	}*/

	Log.Debug("AuthStateMachine","State changed into StateQueueCommand");
	mCurrentStateMachine = &EveClientSocket::_authStateQueueCommand;
}

void EveClientSocket::_authStateQueueCommand(PyNetworkStream& packet)
{
	PyTupleNetStream data(packet);

//	loginprogress::connecting
//	loginprogress::authenticating

	if (data.size() == 3)
	{
		Log.Debug("AuthStateMachine","State changed into StateStateNoCrypto");
		mCurrentStateMachine = &EveClientSocket::_authStateNoCrypto;
		return;
	}
	else if (data.size() == 2) //GetLogonQueuePosition
	{
		std::string unk;
		std::string command;

		data >> unk;		// in normal situations this should be a None object
		data >> command;	// in normal situations this should be a text command.

		ASCENT_ASSERT(command == "QC");

		// a check just to make sure
		if (command == "QC")
		{
			// Send the position 1 or 0  the queue, which is 1 for now until we implemented a real login queue
			// Send 100 and the client tells you that the queue is 100
			_sendQueuePos(1);

			//now act like client just connected
			//send out handshake again
			_sendHandShake();

			//and set proper state
			Log.Debug("AuthStateMachine","State changed into StateHandshake");
			mCurrentStateMachine = &EveClientSocket::_authStateHandshake;
		}
	}
	else 
	{
		//Log.Debug("EVE Socket","StateQueueCommand received a invalid packet");
		sLog.Debug("%s: Received invalid command packet:", GetRemoteIP().c_str());
		Disconnect();
	}
	/* If we get here.... it means that the authorization failed in some way, also we didn't handle the exceptions yet.
	 *
	 */
}

void EveClientSocket::_authStateNoCrypto(PyNetworkStream& packet)
{
	PyTupleNetStream data(packet);

	std::string keyVersion;
	data >> keyVersion;

	if(keyVersion == "placebo")
	{
		//sLog.Debug("%s: Received Placebo 'crypto' request, accepting.", GetRemoteIP().c_str());
		Log.Debug("AuthStateMachine","State changed into StateCryptoChallenge");
		mCurrentStateMachine = &EveClientSocket::_authStateCryptoChallenge;

		_sendAccept();
	}
	else
	{
		sLog.Debug("Received invalid 'crypto' request!");
		Disconnect();
	}

	/*CryptoRequestPacket cryptoRequest;

	PyRep * data = packet;

	if(cryptoRequest.Decode(&data) == false)
	{
		//sLog.Debug("%S: Received invalid 'crypto' request!", GetRemoteIP().c_str());
		Disconnect();
		return;
	}

	if(cryptoRequest.keyVersion == "placebo")
	{
		//sLog.Debug("%s: Received Placebo 'crypto' request, accepting.", GetRemoteIP().c_str());
		Log.Debug("AuthStateMachine","State changed into StateCryptoChallenge");
		mCurrentStateMachine = &EveClientSocket::_authStateCryptoChallenge;

		_sendAccept();
	}
	else
	{
		//i'm sure cryptoRequest.keyVersion can specify either CryptoAPI or PyCrypto, but its all binary so im not sure how.
		PyRep *params = cryptoRequest.keyParams.Clone();
		CryptoAPIRequestParams car;
		if(car.Decode(&params) == false)
		{
			//sLog.Debug("%s: Received invalid CryptoAPI request!", GetRemoteIP().c_str());
			Disconnect();
			return;
		}

		sLog.Debug("%s: 'Unhandled' CryptoAPI request: hashmethod=%s sessionkeylength=%d provider=%s sessionkeymethod=%s", GetRemoteIP().c_str(), car.hashmethod.c_str(), car.sessionkeylength, car.provider.c_str(), car.sessionkeymethod.c_str());
		sLog.Debug("%s: You must change your client to use Placebo 'crypto' in common.ini to talk to this server!\n", GetRemoteIP().c_str());
	}*/
}

void EveClientSocket::_authStateCryptoChallenge(PyNetworkStream& packet)
{
	PyTupleNetStream data(packet);
	PyDictNetStream testDict;

	std::string unkHashOrSalt;
	data >> unkHashOrSalt;
	data >> testDict;

	uint16 machoTest;
	testDict["macho_version"] >> machoTest;

	std::wstring UserName;
	std::wstring UserPass;
	std::string UserPasswordHash;
	int UserAffiliateId;
	testDict["user_name"] >> UserName;
	testDict["user_password"] >> UserPass;
	testDict["user_password_hash"] >> UserPasswordHash;
	testDict["user_affiliateid"] >> UserAffiliateId;

	/* now do the account lookup and get the password from it. */

	std::string calcPassHash;
	std::wstring tempPass = L"syst33m";
	PasswordModule::GeneratePassHash(UserName, tempPass, calcPassHash);



	
	// just to be sure
	/*if(mRequest != NULL)
		delete mRequest;

	

	mRequest = new CryptoChallengePacket;

	if(mRequest->Decode(&packet) == false)
	{
		//sLog.Debug("%s: Received invalid 'crypto' challenge!", GetRemoteIP().c_str());
		Disconnect();
		return;
	}

	sLog.Debug("%s: Received Client Challenge.", GetRemoteIP().c_str(), mRequest->user_name.c_str());

	if(mRequest->user_password->CheckType(PyRep::None) == true)
	{
		//this is little wrong because on live they send password version always, but only once,
		//but we send password version when we get request with hashed password ...
		sLog.Debug("%s: Got hashed password, requesting plain.", GetRemoteIP().c_str());
		delete mRequest;
		mRequest = NULL;

		//send passwordVersion required: 1=plain, 2=hashed
		_sendRequirePasswordType(1);
		return;
	}

	sLog.Debug("%s:		username='%s'", GetRemoteIP().c_str(), mRequest->user_name.c_str());

	//send our handshake
	CryptoServerHandshake server_shake;
	server_shake.serverChallenge = "hi";
	server_shake.func_marshaled_code = new PyRepBuffer(handshakeFunc, sizeof(handshakeFunc));
	server_shake.macho_version = MachoNetVersion;
	server_shake.boot_version = EveVersionNumber;
	server_shake.boot_build = EveBuildVersion;
	server_shake.boot_codename = EveProjectCodename;
	server_shake.boot_region = EveProjectRegion;
	server_shake.cluster_usercount = 100;//m_userCount;
	server_shake.proxy_nodeid = 0xFFAA;
	server_shake.user_logonqueueposition = 1;
	server_shake.challenge_responsehash = "654"; // binascii.crc_hqx of 64 zero bytes in a string, in a single element tuple, marshaled

	// whoo recycle the incomming pointer
	packet = server_shake.Encode();
	OutPacket(packet);
	SafeDelete(packet);*/
	
	Log.Debug("AuthStateMachine","State changed into HandservershakeSend");
	mCurrentStateMachine = &EveClientSocket::_authStateHandshakeSend;
}

void EveClientSocket::_authStateHandshakeSend(PyNetworkStream& packet)
{
	/*CryptoHandshakeResult handshakeResult;
	PyRep * data = packet;
	if(handshakeResult.Decode(&data) == false)
	{
		sLog.Debug("%s: Received invalid 'crypto' handshake result!", GetRemoteIP().c_str());
		Disconnect();
		return;
	}

	//mSession = new EveClientSession(mUserid, mUserName, this);
	mSession = new EveClientSession(0, "", this);

	mSession->_client->Login(mRequest);

	

	mSession->Init(mSession->_client->GetAccountID(), mSession->_client->GetName());
*/

	//this is a bit crappy ...
	//client->Login(m_request);

	/* now comes code that should be in the client class, but as we are hacking anyway why care at all */
	/*sLog.Debug("Login with %s", mRequest->user_name.c_str());

	if(mRequest->user_password->CheckType(PyRep::PackedObject2) == false)
	{
		//sLog.Debug("Failed to get password: user password is not PackedObject2.");
		Disconnect();
		return;
	}
	PyRepPackedObject2 *obj = (PyRepPackedObject2 *)mRequest->user_password;
	//we can check type, should be "util.PasswordString"

	Call_SingleStringArg passString;
	if(!passString.Decode(&obj->args1))
	{
		Disconnect();
		//sLog.Debug("Failed to decode password.");
		return;
	}*/

	// db check stuff
	/*if(!sPyServiceMgr.GetServiceDB()->DoLogin(mRequest->user_name.c_str(), pass.arg.c_str(), m_accountID, m_role)) {
		_log(CLIENT__MESSAGE, "%s: Login rejected by DB", mRequest->user_name.c_str());

		PyRepPackedObject1 *e = new PyRepPackedObject1("exceptions.GPSMACHONETMSG_TYPE_TRANSPORTCLOSED");
		e->args = new PyRepTuple(1);
		e->args->items[0] = new PyRepString("LoginAuthFailed");

		//throw(PyException(e));
	}*/

	// this is needed so if we exit before selecting a character, the account 'online' flag would switch back to 0
	//m_char.charid = 0;
	//sPyServiceMgr.GetServiceDB()->SetAccountOnlineStatus(m_accountID, true);

	/*mUserName =  mRequest->user_name;
	mUserid = 42;


	//send this before session change
	CryptoHandshakeAck cryptoHandshakeAck;
	cryptoHandshakeAck.connectionLogID = 1;	//TODO: what is this??
	cryptoHandshakeAck.jit = mRequest->user_languageid;
	cryptoHandshakeAck.userid = mUserid;//m_accountID;
	cryptoHandshakeAck.maxSessionTime = new PyRepNone;
	cryptoHandshakeAck.userType = 1;	//TODO: what is this??*/

	/*
	1 = I think normal user
	2 = rooky
	23 = I think trial account (unable to verify atm)
	*/

	/*cryptoHandshakeAck.role = 1;//m_role;
	cryptoHandshakeAck.address = GetRemoteIP();
	cryptoHandshakeAck.inDetention = new PyRepNone;
	cryptoHandshakeAck.user_clientid = 42;//m_accountID;
	
	PyRep * ectpack = cryptoHandshakeAck.Encode();
	OutPacket(ectpack);
	delete ectpack;*/

	/* end of the evil hacking */

	//delete mRequest;
	//mRequest = NULL;

	/* assumed that authorization is completed */
/*	if (mSession == NULL)
	{
		mSession = new EveClientSession(mUserid, mUserName, this);
	}*/

	sSpace.AddSession(mSession);
	sSpace.AddGlobalSession(mSession);

	Log.Debug("AuthStateMachine","State changed into StateDone");
	mCurrentStateMachine = &EveClientSocket::_authStateDone;
	mAuthed = true;
}

/* 'ingame' packet dispatcher */

/* Packet fact list
   - in game packet aren't 'PackedObject1' as it seems ( assumption based on the old code )

*/
void EveClientSocket::_authStateDone(PyNetworkStream& packet)
{
	Log.Debug("ClientSocket","received packet 'whooo' we passed authorization");

	// small hack to manage to handle the unexpected stuff..
	// and of course is this not the correct way 'todo' this
	/*if ( packet->CheckType(PyRep::PackedObject1) == true )
	{
		//Log.Debug("AuthStateMachine","Exception debug string:%s", ((PyRepPackedObject1*)packet)->type.c_str());
		

		//Log.Debug("AuthStateMachine","State changed into StateException");
		//mCurrentStateMachine = &EveClientSocket::_authStateException;
		//(this->*mCurrentStateMachine)(recvPyPacket);

		//Log.Debug("AuthStateMachine","Exception noticed");
		//mCurrentStateMachine(packet);
		_authStateException(packet);
		return;		
	}

	//take the PyRep and turn it into a PyPacket
	PyPacket *pyPacket = new PyPacket;
	if(pyPacket->Decode(packet) == false) 	//packet is consumed here, as in deleted....hehehehe kinda inefficient.. but its ok for now
	{
		Log.Error("ClientSocket","%s: Failed to decode packet rep", GetRemoteIP().c_str());
		return;
	}

	mSession->QueuePacket(pyPacket);*/
}

void EveClientSocket::_authStateException(PyNetworkStream& packet)
{
	Log.Debug("AuthStateMachine","Processing Exception");

	/*PyRepPackedObject1* obj = (PyRepPackedObject1*)packet;
	//obj->

	//ByteBuffer data;
	//data.append(packet->)

	// whoo delete if for now
	//SafeDelete(packet);
	SafeDelete(obj);
	obj = NULL;*/
}
