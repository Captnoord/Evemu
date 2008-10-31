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

	PyRep *packet;
	while((packet = _recvQueue.Pop()))
	{
		/* HACKED SOLLUTION TO FIX PACKET DELETION 
		 * if we changed the 'unmarshaling' of the packets from the socket to the packet dispatcher in the Update() function, we don't need todo this.
		 * so the only good solution for this hack is to change all this
		 */
		if (_DeletePyPacket(packet) == false)
		{
			/* Even this would actually never happen, I do add these debug console messages.
			 * as we still are unable to grasp all the stuff that are send to us.
			 *
			 * if after testing, we can say that it would never happen, only the assert will stay
			 */

			if (packet != NULL)
			{
				// this should never happen as it can result in a very very very big stream of error messages
				Log.Error("~Session packet", "unable to delete packet with PyOpcode: %d", packet->GetType());
			}
			else
			{
				// this should absolutely never exist
				assert(true && "Session packet deleting exception catch.. unable to delete NULL pointer packet"); // crash here because if we hit this one something realy fishy is going on
				Log.Error("~Session packet", "unable to delete a NULL pointer packet");
			}
		}
	}

	if(_socket)
		_socket->SetSession(NULL);
	deleteMutex.Release();
}

// wrapper function for hack fix the packet deleting, related to the above comments in the deconstructor
bool EveClientSession::_DeletePyPacket(PyRep* packet)
{
	switch (packet->GetType())
	{
	case PyInteger:
		delete ((PyRepInteger*)packet);
		break;
	case PyReal:
		delete ((PyRepReal*)packet);
		break;
	case PyBoolean:
		delete ((PyRepBoolean*)packet);
		break;
	case PyBuffer:
		delete ((PyRepBuffer*)packet);
		break;
	case PyString:
		delete ((PyRepString*)packet);
		break;
	case PyTuple:
		delete ((PyRepTuple*)packet);
		break;
	case PyList:
		delete ((PyRepList*)packet);
		break;
	case PyDict:
		delete ((PyRepDict*)packet);
		break;
	case PyNone:
		delete ((PyRepNone*)packet);
		break;
	case PySubStruct:
		delete ((PyRepSubStruct*)packet);
		break;
	case PySubStream:
		delete ((PyRepSubStream*)packet);
		break;
	case PyChecksumedStream:
		delete ((PyRepChecksumedStream*)packet);
		break;
	case PyObject:
		delete ((PyRepObject*)packet);
		break;
	case PyPackedRow:
		delete ((PyRepPackedRow*)packet);
		break;
	case PyPackedObject1:
		delete ((PyRepPackedObject1*)packet);
		break;
	case PyPackedObject2:
		delete ((PyRepPackedObject2*)packet);
		break;
	default:
		// return false when we are unable to delete the Packet, because we simply do not know its type
		// Note: this of course would never happen, but better safe than sorry
		return false;
		break;
	}
	// return true when we successfully deleted the Packet
	return true;
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

		MACHONETMSG_TYPE _type = ((PyPacket*)packet)->type;
		if ( _type < MACHONETMSG_TYPE_MAX )
		{
			(this->*Handlers[_type])(*packet);
			packet = NULL;
		}
		else
		{
			Log.Error("SessionPacketDispatcher","'Unhandled' packet with wrong opcode received, opcode:%d", _type);
		}

		Log.Notice("SessionPacketDispatcher","packet processed, deleting the packet");

		/* same comments on line 59, in the deconstructor applies to this code */
		if (_DeletePyPacket(packet) == false)
		{
			if (packet != NULL)
			{
				Log.Error("SessionPacketDispatcher", "unable to delete packet with PyOpcode: %d", packet->GetType());
			}
			else
			{
				assert(true && "Session packet deleting exception catch.. unable to delete NULL pointer packet"); // crash here because if we hit this one something realy fishy is going on
				Log.Error("SessionPacketDispatcher", "unable to delete a NULL pointer packet");
			}
		}
	}
	return 0;
}

void EveClientSession::_ProcessNone(PyRep& packet)
{
	MACHONETMSG_TYPE _type = ((PyPacket*)&packet)->type;
	Log.Debug("SessionPacketDispatcher", "'Unhandled' packet received, opcode:%d", _type);
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