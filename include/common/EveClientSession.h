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

#ifndef __EVECLIENTSESSION_H
#define __EVECLIENTSESSION_H

typedef void (EveClientSession::*packetHandler)(PyNetworkStream&);

class SERVER_DECL EveClientSession
{
	friend class EveClientSocket;
public:
	EveClientSession(EveClientSession* session);

	EveClientSession(uint32 userId, string name, EveClientSocket *sock );
	~EveClientSession();

	void Init(uint32 userId, string accountName)
	{
		ASSERT(_userId == 0); // only initializing a 'EveClientSession' once
		ASSERT(_accountName.size() == 0); // again only initializing a 'EveClientSession' once

		_userId = userId;
		_accountName = accountName;
		//bInitialized = true;
	}

	void OutPacket(PyRep* packet) {if(_socket) _socket->OutPacket(packet);}

	// this needs to be optimized.... 'outpacket'.... and ->encode can be combined.... and should be because it doesn't require a delete
	//void Send(PyPacket* packet)
	//{
		/*PyRep* raw = packet->Encode();
		if(_socket)
			_socket->OutPacket(raw);
		SafeDelete(raw);*/
	//}

	void SendPyRep(PyRep* packet)
	{
		//PyRep* raw = packet->Encode();
		//if(_socket)
		//	_socket->OutPacket(raw);
		//SafeDelete(raw);

		if(_socket)
			_socket->OutPacket(packet);
	}

	//ASCENT_INLINE void QueuePacket(PyPacket* packet);

	ASCENT_INLINE EveClientSocket* GetSocket();
	void SetSocket(EveClientSocket* sock);

	void Disconnect();

	void Delete();

	int Update();

	// returns true if successful and false if its not.
	bool DoLogin(CryptoChallengePacket& requestPack);

	void _ProcessNone(PyNetworkStream& packet);
	void _ProcessCallRequest(PyNetworkStream& packet);
	void _ProcessNotification(PyNetworkStream& packet);
	void _ProcessPingRequest(PyNetworkStream& packet);
	void _ProcessPingResponce(PyNetworkStream& packet);

	uint32 GetUserId() { return _userId;}

/************************************************************************/
/* Packet Utility functions                                             */
/************************************************************************/

	void _sendLoginFailed();

	//void _SendCallReturn(PyPacket *req, PyRep **return_value, const char *channel = NULL);
	//void _SendException(PyPacket *req, MACHONETERR_TYPE type, PyRep **payload);
	void _CheckSessionChange();

	//Client* _client;

private:

	bool bDeleted;
	uint32 _userId;
	string _accountName;
	Mutex _deleteMutex;

	EveClientSocket *_socket;
	

	FastQueue<PyNetworkStream*, Mutex> _recvQueue;

	// this of course will result in a lookup table for every player..... 84 bytes on a 32 bits machine 168 byts on a 64 bits machine..
	packetHandler Handlers[MACHONETMSG_TYPE_MAX];
};

//static
/*packetHandler Handlers[MACHONETMSG_TYPE_MAX] = {
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

#endif//__EVECLIENTSESSION_H