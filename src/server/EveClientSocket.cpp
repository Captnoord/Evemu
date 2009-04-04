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

#include "EvemuPCH.h"
#include "ByteBuffer.h"
#include "PasswordModule.h"
#include "BinAsciiModule.h"
#include "EVEVersion.h"

#ifdef OBJECT_DUMPER_SUPPORT
#include "PyObjectDumper.h"
#endif//OBJECT_DUMPER_SUPPORT

/* unused utility functions */
static std::string CaseFold(std::string & str)
{
	std::string s2 = str;
	Utils::Strings::toUpperCase(s2);
	Utils::Strings::toLowerCase(s2);
	if (s2 != str)
	{
		return CaseFold(s2);
	}
	return s2;
}

static std::wstring CaseFold(std::wstring & str)
{
	std::wstring s2 = str;
	Utils::Strings::toUpperCase(s2);
	Utils::Strings::toLowerCase(s2);
	if (s2 != str)
	{
		return CaseFold(s2);
	}
	return s2;
}

// Dummy authorization handshake function
static const uint8 handShakeInitialFunctionBlob[] = {
	0x74, 0x04, 0x00, 0x00, 0x00, 0x4E, 0x6F, 0x6E, 0x65	//marshaled Python string "None"
};
static const uint8 handShakeInitialFunctionBlobSize = 9;

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

	PyTuple tulpe(6);
	tulpe[0] = EveBirthday;
	tulpe[1] = MachoNetVersion;
	tulpe[2] = authCount;
	tulpe[3] = EveVersionNumber;
	tulpe[4] = EveBuildVersion;
	tulpe[5] = EveProjectVersion;

	send(tulpe);
}

// 'auth' commands
//OK = Reading Client 'Crypto' Context OR Queue Check
//OK CC = 'Crypto' Context Complete
//CC = 'Crypto' Context

/* send handshake accept */
void EveClientSocket::_sendAccept()
{
	PyString out("OK CC");
	send(out);
}

void EveClientSocket::OnConnect()
{
	sSpace.OnClientConnect();
	
	/* atm we don't track socket latency*/
	//_latency = GetTickCount();

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
			/* check if we at least have a header */
			if(GetReadBuffer().GetSize() < 4)
				return;

			/* copy the packet size from the buffer */
			uint32 packetSize;
			GetReadBuffer().Read((void*)&packetSize, 4);
			mRemaining = packetSize;
		}

		ReadStream * packet = NULL;
		if(mRemaining > 0)
		{
			/* check if we have a fragmented packet, if so wait until its completed */
			if( GetReadBuffer().GetSize() < mRemaining )
				return;

			/*
			 * @todo merge the object deserialization and feeding it directly from the read buffer
			 * this requires to add some kind of wrapper mode to ReadStream.
			 */
			packet = new ReadStream(mRemaining);

			if ( !GetReadBuffer().Read(packet->content(), mRemaining) )
			{
				Log.Error("ClientSocket","buffer read went wrong");
				SafeDelete(packet);
				mRemaining = 0;
				return;
			}

			mRemaining = 0;
		}

		PyObject* object = mMarshal.load(*packet);
#ifdef OBJECT_DUMPER_SUPPORT
		printf("client packet:\n");
		DumpObject(stdout, object);
		printf("\n");
#endif//OBJECT_DUMPER_SUPPORT
		if (object == NULL)
		{
			Log.Error("ClientSocket","unable to unmarshal object");
			SafeDelete(packet);
			return;
		}

		// the state machine magic
		(this->*mCurrentStateMachine)(object);
		object->DecRef();
		if (packet)
			SafeDelete(packet);
	}
}

/* the client sends back its server info...
 * we should compare this with our own to make sure we can block unsupported clients.
 * I guess that would never happen unless you have a modified client. And even than its possible.
 */
void EveClientSocket::_authStateHandshake(PyObject* object)
{
	Log.Debug(__FUNCTION__, "hand shaking...");

	if (object->gettype() != PyTypeTuple)
	{
		Log.Error("ClientSocket","initial packet isn't a tuple");
		return;
	}

	PyTuple * tuple = (PyTuple*)object;
	// not sure if we need this check becaus of the get_smart function below
	if (tuple->size() != 6)
	{
		Log.Error("ClientSocket","initial packet isn't a tuple size 6");
		return;
	}

	int32 birthDay;
	int32 machoVersion;
	int32 userCount;
	double versionNr;
	int32 buildVersion;
	std::string projectVersion;	

	// note for mmcs look what I did :P
	if(!tuple->get_smart("iiifis", &birthDay, &machoVersion, &userCount, &versionNr, &buildVersion, &projectVersion))
	{
		Log.Error("ClientSocket","unable to read info from tuple");
		return;
	}


	printf("EveClientSocket 'auth' handshake version: \n\tbirthDay:%d\n\tmachoVersion:%d\n\tuserCount:%d\n\tversionNr:%f\n\tbuildVersion:%d\n\tprojectVersion:%s\n",
		birthDay, machoVersion, userCount, versionNr, buildVersion, projectVersion.c_str());

	/* do version checking here
	   the client also does the version check but hey... hackers....:P */

	/* send fake queue position */
	PyInt queuePos(1);
	send(queuePos);
	
	Log.Debug("AuthStateMachine","State changed into StateQueueCommand");
	mCurrentStateMachine = &EveClientSocket::_authStateQueueCommand;
}

void EveClientSocket::_authStateQueueCommand(PyObject* object)
{
	Log.Debug(__FUNCTION__, "_authStateQueueCommand");

	/* we only want to be fed tuple's at this point */
	if (object->gettype() != PyTypeTuple)
	{
		Log.Error("ClientSocket","received object isn't a tuple");
		return;
	}

	PyTuple * tuple = (PyTuple*)object;

	//std::string unk = tuple->GetString(0); /* return str(binascii.crc_hqx(blue.marshal.Save(args), 0)) */
	std::string queueCommand = tuple->GetString(1);

	/* check if the user has special rights */
	if (queueCommand == "VK") // vip key
	{
		Log.Debug("AuthStateMachine","State changed into StateStateNoCrypto");
		mCurrentStateMachine = &EveClientSocket::_authStateNoCrypto;
		return;
	}
	else if (queueCommand == "QC") // queue command
	{
		/* send login queue */
		SendInt(1);

		_sendHandShake();

		Log.Debug("AuthStateMachine","State changed into StateHandshake");
		mCurrentStateMachine = &EveClientSocket::_authStateHandshake;
		return;
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

void EveClientSocket::_authStateNoCrypto(PyObject* object)
{
	Log.Debug(__FUNCTION__, "_authStateNoCrypto");
	if (object->gettype() != PyTypeTuple)
		return;

	PyTuple * tuple = (PyTuple*)object;
	std::string keyVersion = tuple->GetString(0);

	if (keyVersion == "placebo")
	{
		//sLog.Debug("%s: Received Placebo 'crypto' request, accepting.", GetRemoteIP().c_str());
		Log.Debug("AuthStateMachine","State changed into StateCryptoChallenge");
		mCurrentStateMachine = &EveClientSocket::_authStateCryptoChallenge;

		//SendInt(2);
		_sendAccept();
		return;
	}
	else
	{
		sLog.Debug("Received invalid 'crypto' request!");
		Disconnect();
	}
}

void EveClientSocket::_authStateCryptoChallenge(PyObject* object)
{
	Log.Debug(__FUNCTION__, "_authStateCryptoChallenge");
	if (object->gettype() != PyTypeTuple)
		return;

	PyTuple & tuple = *(PyTuple*)object;
	if (tuple.size() != 2)
		return;

	PyString *clientResponceHash = (PyString *)tuple[0].getPyObject(); // currently we are not using this.
	PyDict &dict = *(PyDict *)tuple[1].getPyObject();

	std::wstring UserName;
	char UserPasswordHash[20];

	/* the only 2 things we actually need to know for now */
	dict.get_smart("user_name", "u", &UserName);
	dict.get_buffer("user_password_hash", (char*)UserPasswordHash, 20);

	/* send password version, I think... */
	//SendInt(2);

	mAccountInfo = sAccountMgr.lookupAccount(UserName);

	// check if the password matches the saved password.
	if(memcmp(UserPasswordHash, mAccountInfo->AccountShaHash, 20) == TRUE)
	{
		// pass doesn't seem to match... send exception...
		Disconnect();
		return;
	}
	else
	{
		PyTuple syncTuple(4);
		
		/* initial function blob */
		PyTuple & initialFunctionBlob = *new PyTuple(2);
		  initialFunctionBlob[0] = new PyString((char*)handShakeInitialFunctionBlob, handShakeInitialFunctionBlobSize);
		  initialFunctionBlob[1] = new PyBool(false);
		
		  /* sync data dict */
		PyDict* syncDict = new PyDict();
		  syncDict->set_double("boot_version", EveVersionNumber);
		  syncDict->set_str("boot_region", EveProjectRegion);
		  syncDict->set_str("challenge_responsehash", "654");
		  syncDict->set_int("cluster_usercount", 10);
		  syncDict->set_int("user_logonqueueposition", 1);
		  syncDict->set_int("macho_version", MachoNetVersion);
		  syncDict->set_str("boot_codename", EveProjectCodename);
		  syncDict->set_int("boot_build", EveBuildVersion);
		  syncDict->set_int("proxy_nodeid", 0xFFAA);					// doesn't really matter which proxy node id we send

		/* the main tuple */
		syncTuple[0] = "hi";					/* we can send everything here as its related to the handShakeInitialFunctionBlob */
		syncTuple[1] = &initialFunctionBlob;
		syncTuple[2] = new PyDict();
		syncTuple[3] = syncDict;

		send(syncTuple);
	}

	Log.Debug("AuthStateMachine","State changed into HandservershakeSend");
	mCurrentStateMachine = &EveClientSocket::_authStateHandshakeSend;
}

void EveClientSocket::_authStateHandshakeSend(PyObject* object)
{
	uint32 userid = sSpace.GenerateUserId();
	uint32 userClientId = mAccountInfo->AccountId;

	/* Server challenge response ack */
	PyDict * dict = new PyDict();
	PyDict * session_init = new PyDict();
	
	/* @todo clean this as its not as fast as it can be */
	std::string remoteip = GetRemoteIP();
	remoteip += ":"; remoteip += Utils::Strings::toString(GetRemotePort());

	session_init->set_str ("languageID", "EN");
	session_init->set_int ("userid", userid);
	session_init->set_item("maxSessionTime", mMarshal.GetPyNone());
	session_init->set_int ("userType", 1);	/* userType: trial:23, steam:? normal:1? */
	session_init->set_int ("role", mAccountInfo->AccountRole);
	session_init->set_str ("address", remoteip.c_str());
	session_init->set_bool("inDetention", false);

	dict->set_item("live_updates", new PyList());
	dict->set_item("session_init", session_init);
	dict->set_item("client_hashes" , new PyList());
	dict->set_int ("user_clientid", userClientId);
	
	MarshalSend(*dict);

	Log.Debug("AuthStateMachine","State changed into StateDone");
	mCurrentStateMachine = &EveClientSocket::_authStateDone;

	mAuthed = true;
}

/* 'ingame' packet dispatcher */

/* Packet fact list
   - in game packet aren't 'PackedObject1' as it seems ( assumption based on the old code )

*/
void EveClientSocket::_authStateDone(PyObject* object)
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

bool EveClientSocket::SendInt( int number )
{
	PyInt num(number);
	return send(num);
}

bool EveClientSocket::SendPyNone()
{
	/* quite tricky because we don't increase and decrease the ref counter */
	return send(mMarshal.PyNone);
}
