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

#ifndef __SPACE_H
#define __SPACE_H

typedef std::list<EveClientSession*>	SessionList;
typedef SessionList::iterator			SessionListItr;
typedef SessionList::const_iterator		SessionListConstItr;

typedef std::set<EveClientSession*>		SessionSet;
typedef SessionSet::iterator			SessionSetItr;
typedef SessionSet::const_iterator		SessionSetConstItr;

// keep track of the space info stuff related to this server
class SERVER_DECL Space : public Singleton<Space>
{
public:
	Space();

	size_t GetConnectionCount()
	{
		return mAcceptedConnections;
	}

	size_t GetAuthorizedCount()
	{
		return mAuthorizedConnections;
	}

	EveClientSession* FindSession(uint32 userid);
	EveClientSession* FindSessionByName(const char *);

	void AddSession(EveClientSession *);
	void RemoveSession(uint32 userid);

	void AddGlobalSession(EveClientSession *session);
	void RemoveGlobalSession(EveClientSession *session);
	void DeleteSession(EveClientSession *session);

	ASCENT_INLINE size_t GetSessionCount() const { return m_sessions.size(); }

	size_t mAcceptedConnections;
	size_t mAuthorizedConnections;
};

#endif//__SPACE_H

#define sSpace Space::getSingleton()

