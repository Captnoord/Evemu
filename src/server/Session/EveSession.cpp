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

static int		EvePersistVarsCount = 30;
static const char * EvePersistVars[] = 
{
	"userid",
	"languageID",
	"regionid",
	"constellationid",
	"role",
	"charid",
	"allianceid",
	"corpid",
	"gangid",
	"gangrole",
	"gangbooster",
	"wingid",
	"squadid",
	"shipid",
	"stationid",
	"solarsystemid",
	"solarsystemid2",
	"address",
	"userType",
	"maxSessionTime",
	"hqID",
	"baseID",
	"rolesAtAll",
	"rolesAtHQ",
	"rolesAtBase",
	"rolesAtOther",
	"genderID",
	"bloodlineID",
	"raceID",
	"corpAccountKey"
};

static int		EveNonPersistVarsCount = 6;
static const char * EveNonpersistvars[] = {
	"sid",
	"c2ooid",
	"connectedObjects",
	"clientID",
	"locationid",
	"corprole"
};

/* eve session is a container for often used client info... */

void EveSession::init(uint32 sid, uint32 role)
{
	for (int i = 0; i < EvePersistVarsCount; i++)
	{
		mSessionDict[EvePersistVars[i]] = GetDefaultValueOfAttribute(EvePersistVars[i]);
	}
	mSessionDict["locationid"] = GetDefaultValueOfAttribute("locationid");
	mSessionDict["corprole"] = GetDefaultValueOfAttribute("corprole");

	/* fill the rest of the session info */

	/* small note: its unclear to me if are going to use all of this... */
	mSessionDict["version"] = 1;
	mSessionDict["sid"] = sid;
	mSessionDict["role"] = role;
	mSessionDict["c2ooid"] = 1;
	mSessionDict["connectedObjects"] = 1;
	mSessionDict["calltimes"] = 1;
	mSessionDict["notificationID"] = 0;
	mSessionDict["machoObjectsByID"] = 1;
	mSessionDict["machoObjectConnectionsByObjectID"] = 1;
	mSessionDict["sessionVariables"] = 1;
	mSessionDict["lastRemoteCall"] = getMSTime();
	mSessionDict["nextSessionChange"] = PyStreamNone;
	mSessionDict["sessionChangeReason"] = "Initial State";
	mSessionDict["callback"] = PyStreamNone;
	//mSessionDict["semaphore"] = 1;//uthread.SquidgySemaphore(('sessions', sid))
	//mSessionDict["sessionhist"] = 1;
	mSessionDict["hasproblems"] = 0;
	mSessionDict["mutating"] = 0;
	mSessionDict["changing"] = PyStreamNone;

	ChangeAttribute("role", role);
	//self.LogSessionHistory('Session created')
}

PySessionAttribute EveSession::GetDefaultValueOfAttribute(const char *attribute)
{
	PySessionAttribute ret;

	// no operator for PySessionAttribute = ROLE_LOGIN
	/*if (!strncmp(attribute, "role", 4))
	{
		ret = ROLE_LOGIN;
		return ret;
	}
	
	if (!strncmp(attribute, "corprole", 8) || !strncmp(attribute, "rolesAtAll", 10) || !strncmp(attribute, "rolesAtHQ", 9) || !strncmp(attribute, "rolesAtBase", 11) || !strncmp(attribute, "rolesAtOther", 12))
	{
		ret = ROLE_LOGIN;
		return ret;
	}*/

	ret = PyStreamNone;
	return ret;
}
