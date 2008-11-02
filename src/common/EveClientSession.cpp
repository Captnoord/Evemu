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
	deleteMutex.Acquire();

	PyPacket *packet;
	while((packet = _recvQueue.Pop()))
	{
		SafeDelete(packet);
	}

	if(_socket)
		_socket->SetSession(NULL);
	deleteMutex.Release();
}

void EveClientSession::Delete()
{
	deleteMutex.Acquire();
	delete this;
}

/* enqueue a packet to be processed in the packet dispatcher */
void EveClientSession::QueuePacket(PyPacket* packet)
{
	//m_lastPing = (uint32)UNIXTIME;
	_recvQueue.Push(packet);
}

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

	PyPacket *packet;
	while (packet = _recvQueue.Pop())
	{
		ASSERT(packet && "EveClientSession packet dispatcher crash");

		MACHONETMSG_TYPE _type = packet->type;
		if ( _type < MACHONETMSG_TYPE_MAX )
		{
			(this->*Handlers[_type])(*packet);
			packet = NULL;
		}
		else
		{
			Log.Error("SessionPacketDispatcher","'Unhandled' packet with wrong opcode received, opcode:%d", _type);
		}

		//Log.Notice("SessionPacketDispatcher","packet processed, deleting the packet");

		SafeDelete(packet);
	}
	return 0;
}

void EveClientSession::_ProcessNone(PyPacket& packet)
{
	Log.Debug("SessionPacketDispatcher", "'Unhandled' packet received, opcode:%d", packet.type);
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

/* not working atm... */
void EveClientSession::_sendLoginFailed()
{
	PyRepPackedObject1 *e = new PyRepPackedObject1("exceptions.GPSTransportClosed");
	e->args = new PyRepTuple(1);
	e->args->items[0] = new PyRepString("LoginAuthFailed");

	//throw(PyException(e));
	//PyException* exp = new PyException(e);
	//OutPacket(exp->ssException.get());
}