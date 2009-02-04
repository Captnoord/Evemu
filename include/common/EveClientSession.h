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

#ifndef __EVECLIENTSESSION_H
#define __EVECLIENTSESSION_H

typedef void (EveClientSession::*packetHandler)(PyReadStream&);

/**
 * \class EveClientSession
 *
 * @brief this class represents the client session layer.
 *
 * This class represents the session layer between the game play code and the socket.
 * the main idea is to split the object into specific classes that all do a diff part of the stuff needed.
 *
 * @author Captnoord.
 * @date January 2009
 */
class SERVER_DECL EveClientSession
{
	friend class EveClientSocket;
public:
	EveClientSession(EveClientSession* session);

	EveClientSession(uint32 userId, EveClientSocket *sock );
	~EveClientSession();

	void Init(uint32 userId)
	{
		ASCENT_ASSERT(_userId == 0); // only initializing a 'EveClientSession' once

		_userId = userId;
	}

	ASCENT_INLINE EveClientSocket* GetSocket();
	void SetSocket(EveClientSocket* sock);

	void Disconnect();

	void Delete();

	int Update();

	void _ProcessNone(PyReadStream& packet);
	void _ProcessCallRequest(PyReadStream& packet);
	void _ProcessNotification(PyReadStream& packet);
	void _ProcessPingRequest(PyReadStream& packet);
	void _ProcessPingResponce(PyReadStream& packet);

	uint32 GetUserId() { return _userId;}

	void _HandleMessage(PyReadStream& packet);

	//void _CheckSessionChange();

private:

	EveSession *SessionInfo;

	bool	bDeleted;
	uint32	_userId;
	Mutex	_deleteMutex;

	EveClientSocket *_socket;
	
	FastQueue<PyReadStream*, Mutex> _recvQueue;

	// this of course will result in a lookup table for every player..... 84 bytes on a 32 bits machine 168 byts on a 64 bits machine..
	packetHandler Handlers[MACHONETMSG_TYPE_MAX];
};

#endif//__EVECLIENTSESSION_H
