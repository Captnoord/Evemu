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

EveClientSession::EveClientSession(uint32 userId, std::string name, EveClientSocket *sock ) : _socket(sock), _userId(userId), _accountName(name)
{

}

EveClientSession::~EveClientSession()
{
	PyPacket *packet;
	while((packet = _recvQueue.Pop()))
		delete packet;

	if(_socket)
		_socket->SetSession(NULL);
}

void EveClientSession::Update()
{
	/*if(!m_net.Connected())
		return(false);

	if(m_pingTimer.Check()) {
		_log(CLIENT__TRACE, "%s: Sending ping request.", GetName());
		_SendPingRequest();
	}*/

	PyPacket *packet;
	while (packet = _recvQueue.Pop())
	{
		ASSERT(packet);

		switch(p->type) {
			case MACHONETMSG_TYPE_CALL_REQ:
				//_ProcessCallRequest(p);
				break;
			case NOTIFICATION:
				//_ProcessNotification(p);
				break;
			case PING_REQ:
				_log(CLIENT__TRACE, "%s: Unhandled ping request.", GetName());
				break;
			case PING_RSP:
				_log(CLIENT__TRACE, "%s: Received ping response.", GetName());
				break;
			default:
				_log(CLIENT__ERROR, "%s: Unhandled message type %d", GetName(), p->type);
		}

		delete p;
		p = NULL;
	}

	//return(true);
}