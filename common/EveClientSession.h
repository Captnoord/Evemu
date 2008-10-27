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

typedef void (EveClientSession::*packetHandler)(PyPacket&t);

static packetHandler Handlers[MACHONETMSG_TYPE_MAX] = {
	&AuthSocket::HandleChallenge,			// 0
	&AuthSocket::HandleProof,				// 1
	&AuthSocket::HandleReconnectChallenge,	// 2
	&AuthSocket::HandleReconnectProof,		// 3
	NULL,									// 4
	NULL,									// 5
	NULL,									// 6
	NULL,									// 7
	NULL,									// 8
	NULL,									// 9
	NULL,									// 10
	NULL,									// 11
	NULL,									// 12
	NULL,									// 13
	NULL,									// 14
	NULL,									// 15
	&AuthSocket::HandleRealmlist,			// 16
	NULL,									// 17
	NULL,									// 18
	NULL,									// 19
	NULL,									// 20
	NULL,									// 21
	NULL,									// 22
	NULL,									// 23
	NULL,									// 24
	NULL,									// 25
	NULL,									// 26
	NULL,									// 27
	NULL,									// 28
	NULL,									// 29
	NULL,									// 30
	NULL,									// 31
	NULL,									// 32
	NULL,									// 33
	NULL,									// 34
	NULL,									// 35
	NULL,									// 36
	NULL,									// 37
	NULL,									// 38
	NULL,									// 39
	NULL,									// 40
	NULL,									// 41
	NULL,									// 42
	NULL,									// 43
	NULL,									// 44
	NULL,									// 45
	NULL,									// 46
	NULL,									// 47
	NULL,									// 48
	NULL,									// 49
	&AuthSocket::HandleTransferAccept,		// 50
	&AuthSocket::HandleTransferResume,		// 51
	&AuthSocket::HandleTransferCancel,		// 52
};

class SERVER_DECL EveClientSession
{
	friend class EveClientSocket;
public:
	EveClientSession(uint32 userId, string name, EveClientSocket *sock );
	~EveClientSession();

	ASCENT_INLINE void QueuePacket(PyPacket* packet)
	{
		printf("client session queue packet\n");
		//m_lastPing = (uint32)UNIXTIME;
		_recvQueue.Push(packet);
	}

	ASCENT_INLINE EveClientSocket* GetSocket() { return _socket; }

	void SetSocket(EveClientSocket* sock)
	{
		_socket = sock;
	}

	void Disconnect()
	{
		if(_socket && _socket->IsConnected())
			_socket->Disconnect();
	}

	int Update();

private:
	EveClientSocket *_socket;

	uint32 _userId;
	//uint32 _accountFlags;
	std::string _accountName;

	FastQueue<PyPacket*, Mutex> _recvQueue;
};

#endif//__EVECLIENTSESSION_H