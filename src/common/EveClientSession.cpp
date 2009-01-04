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

EveClientSession::EveClientSession(uint32 userId, std::string name, EveClientSocket *sock ) : _socket(sock), _userId(userId), _accountName(name), bDeleted(false)
{
	Handlers[0] = &EveClientSession::_ProcessNone;
	Handlers[1] = &EveClientSession::_ProcessNone;
	Handlers[2] = &EveClientSession::_ProcessNone;
	Handlers[3] = &EveClientSession::_ProcessNone;
	Handlers[4] = &EveClientSession::_ProcessNone;
	Handlers[5] = &EveClientSession::_ProcessNone;
	Handlers[6] = &EveClientSession::_ProcessCallRequest;
	Handlers[7] = &EveClientSession::_ProcessNone;
	Handlers[8] = &EveClientSession::_ProcessNone;
	Handlers[9] = &EveClientSession::_ProcessNone;
	Handlers[10] = &EveClientSession::_ProcessNone;
	Handlers[11] = &EveClientSession::_ProcessNone;
	Handlers[12] = &EveClientSession::_ProcessNotification;
	Handlers[13] = &EveClientSession::_ProcessNone;
	Handlers[14] = &EveClientSession::_ProcessNone;
	Handlers[15] = &EveClientSession::_ProcessNone;
	Handlers[16] = &EveClientSession::_ProcessNone;
	Handlers[17] = &EveClientSession::_ProcessNone;
	Handlers[18] = &EveClientSession::_ProcessNone;
	Handlers[19] = &EveClientSession::_ProcessNone;
	Handlers[20] = &EveClientSession::_ProcessPingRequest;
	Handlers[21] = &EveClientSession::_ProcessPingResponce;

	//_client = new Client(this);

	// crap.... check comments in the header..
	/*Handlers = {
		&EveClientSession::_ProcessNone, //0
		&EveClientSession::_ProcessNone, //1
		&EveClientSession::_ProcessNone, //2
		&EveClientSession::_ProcessNone, //3
		&EveClientSession::_ProcessNone, //4
		&EveClientSession::_ProcessNone, //5
		&EveClientSession::_ProcessCallRequest, //6
		&EveClientSession::_ProcessNone, //7
		&EveClientSession::_ProcessNone, //8
		&EveClientSession::_ProcessNone, //9
		&EveClientSession::_ProcessNone, //10
		&EveClientSession::_ProcessNone, //11
		&EveClientSession::_ProcessNotification, //12
		&EveClientSession::_ProcessNone, //13
		&EveClientSession::_ProcessNone, //14
		&EveClientSession::_ProcessNone, //15
		&EveClientSession::_ProcessNone, //16
		&EveClientSession::_ProcessNone, //17
		&EveClientSession::_ProcessNone, //18
		&EveClientSession::_ProcessNone, //19
		&EveClientSession::_ProcessPingRequest, //20
		&EveClientSession::_ProcessPingResponce, //21
	};*/

}

EveClientSession::~EveClientSession()
{
	_deleteMutex.Acquire();

	PyNetworkStream *packet;
	while((packet = _recvQueue.Pop()))
	{
		SafeDelete(packet);
	}

	if(_socket != NULL)
		_socket->SetSession(NULL);
	_deleteMutex.Release();
}

void EveClientSession::Delete()
{
	_deleteMutex.Acquire();
	delete this;
}

/* enqueue a packet to be processed in the packet dispatcher */
/*void EveClientSession::QueuePacket(PyNetworkStream* packet)
{
	//m_lastPing = (uint32)UNIXTIME;
	_recvQueue.Push(packet);
}*/

/* returns the socket */
EveClientSocket* EveClientSession::GetSocket()
{
	return _socket;
}

/* sets the sessions socket */
void EveClientSession::SetSocket(EveClientSocket* sock)
{
	_socket = sock;
}

void EveClientSession::Disconnect()
{
	if(_socket && _socket->IsConnected())
		_socket->Disconnect();
}

int EveClientSession::Update()
{
	if( _socket == NULL )
	{
		bDeleted = true;
		return 1;
	}
	if (_recvQueue.GetSize() == 0)
		return 0;

	PyNetworkStream *packet;
	while (packet = _recvQueue.Pop())
	{
		ASSERT(packet && "EveClientSession packet dispatcher crash");
		
		// this fucked... hack.....
		MACHONETMSG_TYPE _type = MACHONETMSG_TYPE_AUTHENTICATION_REQ;//packet->type;
		if ( _type < MACHONETMSG_TYPE_MAX )
		{
			(this->*Handlers[_type])(*packet);
			packet = NULL;
		}
		else
		{
			Log.Error("SessionPacketDispatcher","'Unhandled' packet with wrong opcode received, opcode:%d", _type);
		}

		SafeDelete(packet);
	}
	return 0;
}

void EveClientSession::_ProcessNone(PyNetworkStream& packet)
{
	//Log.Debug("SessionPacketDispatcher", "'Unhandled' packet received, opcode:%d", packet.type);
}

void EveClientSession::_ProcessCallRequest(PyNetworkStream& packet)
{
	//Log.Debug("SessionPacketDispatcher", "ProcessCallRequest");
	//_client->_ProcessCallRequest(&packet);
}

void EveClientSession::_ProcessNotification(PyNetworkStream& packet)
{
	//Log.Debug("SessionPacketDispatcher", "ProcessNotification");
	//_client->_ProcessNotification(&packet);
}

void EveClientSession::_ProcessPingRequest(PyNetworkStream& packet)
{
	Log.Debug("SessionPacketDispatcher", /*"%s:*/ "'Unhandled' ping request.");//, GetName());

	//PyLogsysDump dumper(SERVICE__ERROR);
	//packet.Dump(SERVICE__ERROR,&dumper);

	//PyRepChecksumedStream * pingPacket = new PyRepChecksumedStream;
	//bool success = packet.Decode(pingPacket); // kinda assuming this returns true

	//return(new PyRepInteger(Win32TimeNow()));

	/*PyNetworkStream ping_req(MACHONETMSG_TYPE_PING_RSP, "macho.PingRsp");
	ping_req.source.type = PyAddress::Node;
	ping_req.source.typeID = sPyServiceMgr.GetNodeID();
	//ping_req.source.callID

	ping_req.dest.type = PyAddress::Client;
	ping_req.dest.typeID = GetUserId();//GetAccountID();
	ping_req.dest.callID = 0; //ping_req.dest.callID = 0;

	ping_req.userid = GetUserId();

	ping_req.payload = new PyRepTuple(1);

	PyRepList* list = new PyRepList();

	ping_req.payload->items[0] = list;	//times

	PyRepTuple* container = new PyRepTuple(3);
	PyRepTuple* container2 = new PyRepTuple(3);
	list->add(container);
	list->add(container2);

	(*container)[0] = new PyRepInteger(Win32TimeNow());
	(*container)[1] = new PyRepInteger(Win32TimeNow());
	(*container)[2] = new PyRepString("server::handle_message");
	
	(*container2)[0] = new PyRepInteger(Win32TimeNow());
	(*container2)[1] = new PyRepInteger(Win32TimeNow());
	(*container2)[2] = new PyRepString("server::turnaround");

	//ping_req.named_payload = new PyRepDict();

	Send(&ping_req);*/

	/*PyNetworkStream *ping_req = new PyPacket(MACHONETMSG_TYPE_PING_REQ, "macho.PingReq");

	ping_req->source.type = PyAddress::Node;
	ping_req->source.typeID = m_services->GetNodeID();
	ping_req->source.service = "ping";
	ping_req->source.callID = 0;

	ping_req->dest.type = PyAddress::Client;
	ping_req->dest.typeID = GetAccountID();
	ping_req->dest.callID = 0;

	ping_req->userid = m_accountID;

	ping_req->payload = new PyRepTuple(1);
	ping_req->payload->items[0] = new PyRepList();	//times
	ping_req->named_payload = new PyRepDict();

	FastQueuePacket(&ping_req);*/
}

void EveClientSession::_ProcessPingResponce(PyNetworkStream& packet)
{
	Log.Debug("SessionPacketDispatcher", /*"%s:*/ "Received ping response.");//, GetName());
}

/* not working atm... */
void EveClientSession::_sendLoginFailed()
{
	//PyRepPackedObject1 *e = new PyRepPackedObject1("exceptions.GPSTransportClosed");
	//e->args = new PyRepTuple(1);
	//e->args->items[0] = new PyRepString("LoginAuthFailed");

	//throw(PyException(e));
	//PyException* exp = new PyException(e);
	//OutPacket(exp->ssException.get());
}

//void EveClientSession::_SendCallReturn(PyNetworkStream *req, PyRep **return_value, const char *channel)
//{
	//build the packet:
	/*PyPacket packet(MACHONETMSG_TYPE_CALL_RSP, "macho.CallRsp");

	packet.source = req->dest;

	if(GetUserId() == 0)
	{
		Log.Error("EveClientSession","***************************User ID = 0");
	}

	packet.dest.type = PyAddress::Client;
	packet.dest.typeID = GetUserId();//GetAccountID();    // when destination type is client this is the account id
	packet.dest.callID = req->source.callID;

	packet.userid = GetUserId();//m_accountID;

	packet.payload = new PyRepTuple(1);
	packet.payload->items[0] = new PyRepSubStream(*return_value);
	*return_value = NULL;	//consumed

	if(channel != NULL)
	{
		packet.named_payload = new PyRepDict();
		packet.named_payload->add("channel", new PyRepString(channel));
	}

	
	Send(&packet);*/
//}

/*	Note: this function needs some love.....

*/
bool EveClientSession::DoLogin(CryptoChallengePacket& requestPack)
{
	//_client->Login(&requestPack);


	// just a debug message
	//Log.Notice("ClientSession", "User:%s logging in", requestPack.user_name.c_str());

	/*if(requestPack.user_password->CheckType(PyRep::PackedObject2) == false)
	{
		//sLog.outDebug("Failed to get password: user password is not PackedObject2.");
		Disconnect();
		return false;
	}

	PyRepPackedObject2 *obj = (PyRepPackedObject2 *)requestPack.user_password;
	//we can check type, should be "util.PasswordString"

	Call_SingleStringArg passString;
	if(!passString.Decode(&obj->args1))
	{
		Disconnect();
		//sLog.outDebug("Failed to decode password.");
		return false;
	}

	// Note: most of these password checks needs to have a feedback towards the client

	// password checking: even that its not possible (client side limitations) don't process passwords with no data.
	if (passString.arg.size() == 0)
	{
		Disconnect();
		//sLog.outDebug("Password not valid as it contains no data");
		return false;
	}

	// password checking: lets say we can make this a configurable limit, a password has to be a certain length
	//if (passString.arg.size() < 5)
	//{
	//Disconnect();
	//sLog.outDebug("Password is to short");
	//return false;
	}

	// shouldn't be even possible because of the client
	if (passString.arg.size() > 100)
	{
		Disconnect();
		//sLog.outDebug("Password is to long");
		return false;
	}

	// password checking: and of course this is a useless check (old code) but if it happens....
	if (passString.arg[0] == '\0')
	{
		Disconnect();
		//sLog.outDebug("Password is not valid");
		return false;
	}

	QueryResult * result = GenericDatabase.Query("SELECT accountID, accountName, password, role, online FROM account WHERE accountName='%s'", GenericDatabase.EscapeString(requestPack.user_name).c_str());

	// this statement is quite bold, if the user doesn't exists OR if the password is not correct....
	if (result == NULL)
	{
		Disconnect();
		//sLog.outDebug("user isn't in the database");
		// query failed
		return false;
	}

	// do password checks and stuff

	Field * queryField = result->Fetch();
	if (queryField == NULL)
	{
		// ok something is wrong whooo exit (shouldn't happen on a good system)
		Disconnect();
		//sLog.outDebug("query error unable to read query result");
		SafeDelete(result);
		return false;
	}

	// store the result
	uint32 accountId = queryField[0].GetUInt32();
	const char* accountName = queryField[1].GetString();
	const char* accountPass = queryField[2].GetString();
	uint32 role = queryField[3].GetUInt32();
	bool online = queryField[4].GetBool(); // 0 == false any other value is true, no text... numbers only

	// compare pass

	// Note: add py exception message 
	if (strncmp(passString.arg.c_str(), accountPass, 0xFF) != 0 )
	{
		// ok wrong password
		Disconnect();
		//sLog.outDebug("password: wrong password");
		SafeDelete(result);
		return false;
	}

	SafeDelete(result);



	// db check stuff
	
	//PyRepPackedObject1 *e = new PyRepPackedObject1("exceptions.GPSMACHONETMSG_TYPE_TRANSPORTCLOSED");
	//e->args = new PyRepTuple(1);
	//e->args->items[0] = new PyRepString("LoginAuthFailed");
	}

	uint32 mUserid = accountId; // because we don't have or need a dynamic userid generator ( mainly because we aren't clustered yet ) use static account id.
	Init(mUserid, requestPack.user_name);

	//send this before session change
	CryptoHandshakeAck cryptoHandshakeAck;
	cryptoHandshakeAck.connectionLogID = 1;	//TODO: what is this??
	cryptoHandshakeAck.jit = requestPack.user_languageid;
	cryptoHandshakeAck.userid = mUserid;
	cryptoHandshakeAck.maxSessionTime = new PyRepInteger(5000);//new PyRepNone;
	cryptoHandshakeAck.userType = 1;	//TODO: what is this??

	
	//1 = I think normal user
	//2 = rooky
	//23 = I think trial account (unable to verify atm)
	

	cryptoHandshakeAck.role = role;
	cryptoHandshakeAck.address = _socket->GetRemoteIP(); // strange one...
	cryptoHandshakeAck.inDetention = new PyRepBoolean(false);//new PyRepNone;
	cryptoHandshakeAck.user_clientid = mUserid;//m_accountID;

	PyRep * ectpack = cryptoHandshakeAck.Encode();
	Send(ectpack);
	SafeDelete(ectpack);

	//if(_sessionInfo == NULL)
	//{
	//	_sessionInfo = new EveClientSessionInfo();
	//}

	//_sessionInfo->Set_userType(1);	//TODO: what is this??
	//_//sessionInfo->Set_userid(mUserid);
	//_sessionInfo->Set_address(_socket->GetRemoteIP());
	//_sessionInfo->Set_role(role);
	//_sessionInfo->Set_languageID(requestPack.user_languageid);

	//_CheckSessionChange();

	_log(CLIENT__MESSAGE, "Login with %s", pack->user_name.c_str());

	if(!pack->user_password->CheckType(PyRep::PackedObject2)) {
	_log(CLIENT__ERROR, "Failed to get password: user password is not PackedObject2.");
	return;
	}
	PyRepPackedObject2 *obj = (PyRepPackedObject2 *)pack->user_password;
	//we can check type, should be "util.PasswordString"

	Call_SingleStringArg pass;
	if(!pass.Decode(&obj->args1)) {
	_log(CLIENT__ERROR, "Failed to decode password.");
	return;
	}

	if(!m_services->GetServiceDB()->DoLogin(pack->user_name.c_str(), pass.arg.c_str(), m_accountID, m_role)) {
	_log(CLIENT__MESSAGE, "%s: Login rejected by DB", pack->user_name.c_str());

	PyRepPackedObject1 *e = new PyRepPackedObject1("exceptions.GPSTransportClosed");
	e->args = new PyRepTuple(1);
	e->args->items[0] = new PyRepString("LoginAuthFailed");

	throw(PyException(e));
	}

	// this is needed so if we exit before selecting a character, the account 'online' flag would switch back to 0
	m_char.charid = 0;
	m_services->GetServiceDB()->SetAccountOnlineStatus(m_accountID, true);

	//send this before session change
	CryptoHandshakeAck ack;
	ack.connectionLogID = 1;	//TODO: what is this??
	ack.jit = pack->user_languageid;
	ack.userid = m_accountID;
	ack.maxSessionTime = new PyRepNone;
	ack.userType = 1;	//TODO: what is this??
	ack.role = m_role;
	ack.address = m_net.GetConnectedAddress();
	ack.inDetention = new PyRepNone;
	ack.user_clientid = m_accountID;

	m_net._QueueRep(ack.Encode());

	session.Set_userType(1);	//TODO: what is this??
	session.Set_userid(m_accountID);
	session.Set_address(m_net.GetConnectedAddress().c_str());
	session.Set_role(m_role);
	session.Set_languageID(pack->user_languageid.c_str());

	_CheckSessionChange();*/
	return true;
}
