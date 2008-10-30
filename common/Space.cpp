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

initialiseSingleton( Space );

Space::Space() : mAcceptedConnections(0), mAuthorizedConnections(0), mStartTime(0)
{

}

EveClientSession* Space::FindSession(uint32 id)
{
	m_sessionlock.AcquireReadLock();
	EveClientSession * ret = 0;
	SessionMapConstItr itr = m_sessions.find(id);

	if(itr != m_sessions.end())
		ret = itr->second;

	m_sessionlock.ReleaseReadLock();
	return ret;
}

void Space::RemoveSession(uint32 id)
{
	m_sessionlock.AcquireWriteLock();

	SessionMapItr itr = m_sessions.find(id);

	if(itr != m_sessions.end())
	{
		delete itr->second;
		m_sessions.erase(itr);
	}

	m_sessionlock.ReleaseWriteLock();
}

void Space::AddSession(EveClientSession* session)
{
	if(session == NULL)
		return;

	m_sessionlock.AcquireWriteLock();

	ASSERT(session);
	m_sessions[session->GetUserId()] = session;

	//if(m_sessions.size() >  PeakSessionCount)
	//	PeakSessionCount = (uint32)m_sessions.size();

	m_sessionlock.ReleaseWriteLock();
}

void Space::AddGlobalSession(EveClientSession *session)
{
	if(session == NULL)
		return;

	SessionsMutex.Acquire();
	Sessions.insert(session);
	SessionsMutex.Release();
}

void Space::RemoveGlobalSession(EveClientSession *session)
{
	SessionsMutex.Acquire();
	Sessions.erase(session);
	SessionsMutex.Release();
}

void Space::Update(time_t diff)
{

}

void Space::UpdateSessions(time_t diff)
{
	
	if (Sessions.empty() == true)
		return;

	SessionSet::iterator itr = Sessions.begin();//, it2;
	EveClientSession *session;
	//int result;
	for(; itr != Sessions.end();)
	{
		session = (*itr);
		++itr;
		/*it2 = itr;
		++itr;
		if(!session || session->GetInstance() != 0)
		{
			Sessions.erase(it2);
			continue;
		}*/

		session->Update();

		/*if((result = session->Update()))
		{
			if(result == 1)
			{
				// complete deletion
				DeleteSession(session);
			}
			Sessions.erase(it2);
		}*/
	}
}