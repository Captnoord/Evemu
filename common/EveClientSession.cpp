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

typedef enum {
	PyInteger,
	PyReal,
	PyBoolean,
	PyBuffer,
	PyString,
	PyTuple,
	PyList,
	PyDict,
	PyNone,
	PySubStruct,
	PySubStream,
	PyChecksumedStream,
	PyObject,
	PyPackedRow,
	PyPackedObject1,
	PyPackedObject2
} Type;

EveClientSession::~EveClientSession()
{
	deleteMutex.Acquire();
	// dono if this is correct but will do for now....
	//sSpace.RemoveSession(this->GetUserId());
	//sSpace.RemoveGlobalSession(this);

	PyRep *packet;
	while((packet = _recvQueue.Pop()))
	{
		PyRep * pack = packet;

		/* HACKED SOLLUTION TO FIX PACKET DELETION */
		switch (pack->GetType())
		{
		case PyInteger:
		case PyReal:
		case PyBoolean:
		case PyBuffer:
		case PyString:
		case PyTuple:
		case PyList:
		case PyDict:
		case PyNone:
		case PySubStruct:
		case PySubStream:
		case PyChecksumedStream:
		case PyObject:
		case PyPackedRow:
		case PyPackedObject1:
		case PyPackedObject2:
			{
				delete packet;

			}break;
		}
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
void EveClientSession::QueuePacket(PyRep* packet)
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

	PyRep *packet;
	while (packet = _recvQueue.Pop())
	{
		ASSERT(packet && "EveClientSession packet dispatcher crash");

		/*if ( packet&& packet->type < MACHONETMSG_TYPE_MAX )
		{
			(this->*Handlers[packet->type])(*packet);
			packet = NULL;
		}*/
		
		//delete packet;
	}
	return 0;
}

void EveClientSession::_ProcessNone(PyRep& packet)
{
	
	//Log.Debug("SessionPacketDispatcher", "'Unhandled' packet received, opcode:%d", packet.type);
}

void EveClientSession::_ProcessCallRequest(PyRep& packet)
{
	Log.Debug("SessionPacketDispatcher", "ProcessCallRequest");
}

void EveClientSession::_ProcessNotification(PyRep& packet)
{
	Log.Debug("SessionPacketDispatcher", "ProcessNotification");
}

void EveClientSession::_ProcessPingRequest(PyRep& packet)
{
	Log.Debug("SessionPacketDispatcher", /*"%s:*/ "'Unhandled' ping request.");//, GetName());
}

void EveClientSession::_ProcessPingResponce(PyRep& packet)
{
	Log.Debug("SessionPacketDispatcher", /*"%s:*/ "Received ping response.");//, GetName());
}