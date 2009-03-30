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

EveClientSession::EveClientSession(uint32 userId, EveClientSocket *sock ) : _socket(sock), _userId(userId), bDeleted(false)
{
/*	Handlers[0] = &EveClientSession::_ProcessNone;
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
	Handlers[21] = &EveClientSession::_ProcessPingResponce;*/
}

EveClientSession::~EveClientSession()
{
	_deleteMutex.Acquire();

	/*MarshalReadStream *packet;
	while((packet = _recvQueue.Pop()))
	{
		//SafeDelete(packet);
	}

	if(_socket != NULL)
		_socket->SetSession(NULL);
	_deleteMutex.Release();*/
}

void EveClientSession::Delete()
{
	_deleteMutex.Acquire();
	delete this;
}

/* enqueue a packet to be processed in the packet dispatcher */
/*void EveClientSession::QueuePacket(PyReadStream* packet)
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
/*	if (_recvQueue.GetSize() == 0)
		return 0;

	MarshalReadStream *packet;
	while (packet = _recvQueue.Pop())
	{
		ASSERT(packet && "EveClientSession packet dispatcher crash");
		*/
		// this fucked... hack.....
		/*MACHONETMSG_TYPE _type = MACHONETMSG_TYPE_AUTHENTICATION_REQ;//packet->type;
		if ( _type < MACHONETMSG_TYPE_MAX )
		{
			(this->*Handlers[_type])(*packet);
			packet = NULL;
		}
		else
		{
			Log.Error("SessionPacketDispatcher","'Unhandled' packet with wrong opcode received, opcode:%d", _type);
		}*/

		//_HandleMessage(*packet);

		//SafeDelete(packet);
	//}
	return 0;
}

#if 0

// whoo gonne act like python code.....
void EveClientSession::_HandleMessage( MarshalReadStream& packet )
{
	MarshalStream stream;
	stream.load(packet);


	//PyObject & packet =
	/** Check if the command is a ping command... if so.. add info to it...
	if (theMessage.command in self.__pingrequestorresponse__):
	theMessage.times.append((blue.os.GetTime(), blue.os.GetTime(1), (macho.mode + '::handle_message')))
	theMessage.Changed()
	*/

}

void EveClientSession::_ProcessNone(MarshalReadStream& packet)
{
	//Log.Debug("SessionPacketDispatcher", "'Unhandled' packet received, opcode:%d", packet.type);
}

void EveClientSession::_ProcessCallRequest(MarshalReadStream& packet)
{
	//Log.Debug("SessionPacketDispatcher", "ProcessCallRequest");
}

void EveClientSession::_ProcessNotification(MarshalReadStream& packet)
{
	//Log.Debug("SessionPacketDispatcher", "ProcessNotification");
}

void EveClientSession::_ProcessPingRequest(MarshalReadStream& packet)
{
	Log.Debug("SessionPacketDispatcher", /*"%s:*/ "'Unhandled' ping request.");//, GetName());
}

void EveClientSession::_ProcessPingResponce(MarshalReadStream& packet)
{
	Log.Debug("SessionPacketDispatcher", /*"%s:*/ "Received ping response.");//, GetName());
}

#endif