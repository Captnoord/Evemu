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
	// dono if this is correct but will do for now....
	sSpace.RemoveSession(this->GetUserId());
	sSpace.RemoveGlobalSession(this);

	PyPacket *packet;
	while((packet = _recvQueue.Pop()))
		delete packet;

	if(_socket)
		_socket->SetSession(NULL);
}

void EveClientSession::Update()
{
	PyPacket *packet;
	while (packet = _recvQueue.Pop())
	{
		ASSERT(packet && "EveClientSession packet dispatcher crash");

		if ( packet->type < MACHONETMSG_TYPE_MAX )
		{
			(this->*Handlers[packet->type])(*packet);
		}
		//delete packet;
	}
}

void EveClientSession::_ProcessNone(PyPacket& packet)
{
	Log.Debug("SessionPacketDispatcher", "'Unhandled' packet received");
}

void EveClientSession::_ProcessCallRequest(PyPacket& packet)
{
	Log.Debug("SessionPacketDispatcher", "ProcessCallRequest");
}

void EveClientSession::_ProcessNotification(PyPacket& packet)
{
	Log.Debug("SessionPacketDispatcher", "ProcessNotification");
}

void EveClientSession::_ProcessPingRequest(PyPacket& packet)
{
	Log.Debug("SessionPacketDispatcher", /*"%s:*/ "'Unhandled' ping request.");//, GetName());
}

void EveClientSession::_ProcessPingResponce(PyPacket& packet)
{
	Log.Debug("SessionPacketDispatcher", /*"%s:*/ "Received ping response.");//, GetName());
}