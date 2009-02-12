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

#ifndef PY_STRING_TABLE_H
#define PY_STRING_TABLE_H

#include "ascent.h"
#include "PyObjects.h"

/**
 * \class PyMarshalStringTable
 *
 * @brief a singleton data container for communication string lookup.
 *
 * this class is a data container for communication string lookup.
 * eventually this class should be available to every thread the unmarshal's.
 * so only until we have solved the entire mess.. this is a singleton with mutex locks.
 * @todo when the object puzzle is solved this class should be available to every thread.
 * it doesn't take much mem. So it means the mutexes can be removed when every thread has its own resource.
 *
 * @author Captnoord
 * @date December 2008
 */
class PyMarshalStringTable : public Singleton<PyMarshalStringTable>
{
public:
	PyMarshalStringTable();
	~PyMarshalStringTable();

	/**
	 * @brief lookup a index nr using a string
	 *
	 * no long description
	 *
	 * @param[in] string that needs a lookup for a index nr.
	 * @return the index number of the string that was given, returns -1 if string is not found.
	 */
	size_t LookupIndex(std::string &str);

	/**
	* @brief lookup a index nr using a string
	*
	* no long description
	*
	* @param[in] string that needs a lookup for a index nr.
	* @return the index number of the string that was given, returns -1 if string is not found.
	*/
	size_t LookupIndex(const char* str);

	/**
	 * @brief lookup a string using a index
	 *
	 * no long description
	 *
	 * @param[in] index is the index of the string that needs to be looked up.
	 * @param[out] str is the string that we have looked up if successful.
	 * @return true if the index is valid and false if its beyond the range.
	 */
	bool LookupString(uint8 index, std::string &str);

	/**
	 * @brief lookup a PyString object using a index
	 *
	 * no long description
	 *
	 * @param[in] index is the index of the string that needs to be looked up.
	 * @param[out] str is the PyString that we have looked up if successful.
	 * @return true if the index is valid and false if its beyond the range.
	 */
	bool LookupPyString(uint8 index, PyString *&str);

#define STRING_TABLE_ERROR -1
	
private:
	/**
	 * @brief djb2 algorithm taken from http://www.cse.yorku.ca/~oz/hash.html slightly modified
	 *
	 * 
	 *
	 * @param[in] oStr string that needs to be hashed.
	 * @return djb2 has of the string.
	 */
	uint32 hash(const std::string& oStr)
	{
		const char * str = oStr.c_str();

		uint32 hash = 5381;
		int c;

		while ((c = *str++))
			hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

		return hash;
	}

private:
	typedef std::tr1::unordered_map<uint32, uint8>	StringMap;
	typedef StringMap::iterator						StringMapItr;
	typedef StringMap::const_iterator				StringMapConstItr;

	StringMap	mStringTable;
	Mutex		mLock; // only for now..
	PyString	mPyStringTable[195];
};

/* the current string count of the string table */
static size_t StringTableSize = 195;

/* we made up this list so we have efficient string communication with the client */
static const char *StringTableData[] =
{
	"*corpid",
	"*locationid",
	"age",
	"Asteroid",
	"authentication",
	"ballID",
	"beyonce",
	"bloodlineID",
	"capacity",
	"categoryID",
	"character",
	"characterID",
	"characterName",
	"characterType",
	"charID",
	"chatx",
	"clientID",
	"config",
	"contraband",
	"corporationDateTime",
	"corporationID",
	"createDateTime",
	"customInfo",
	"description",
	"divisionID",
	"DoDestinyUpdate",
	"dogmaIM",
	"EVE System",
	"flag",
	"foo.SlimItem",
	"gangID",
	"Gemini",
	"gender",
	"graphicID",
	"groupID",
	"header",
	"idName",
	"invbroker",
	"itemID",
	"items",
	"jumps",
	"line",
	"lines",
	"locationID",
	"locationName",
	"macho.CallReq",
	"macho.CallRsp",
	"macho.MachoAddress",
	"macho.Notification",
	"macho.SessionChangeNotification",
	"modules",
	"name",
	"objectCaching",
	"objectCaching.CachedObject",
	"OnChatJoin",
	"OnChatLeave",
	"OnChatSpeak",
	"OnGodmaShipEffect",
	"OnItemChange",
	"OnModuleAttributeChange",
	"OnMultiEvent",
	"orbitID",
	"ownerID",
	"ownerName",
	"quantity",
	"raceID",
	"RowClass",
	"securityStatus",
	"Sentry Gun",
	"sessionchange",
	"singleton",
	"skillEffect",
	"squadronID",
	"typeID",
	"used",
	"userID",
	"util.CachedObject",
	"util.IndexRowset",
	"util.Moniker",
	"util.Row",
	"util.Rowset",
	"*multicastID",
	"AddBalls",
	"AttackHit3",
	"AttackHit3R",
	"AttackHit4R",
	"DoDestinyUpdates",
	"GetLocationsEx",
	"InvalidateCachedObjects",
	"JoinChannel",
	"LSC",
	"LaunchMissile",
	"LeaveChannel",
	"OID+",
	"OID-",
	"OnAggressionChange",
	"OnCharGangChange",
	"OnCharNoLongerInStation",
	"OnCharNowInStation",
	"OnDamageMessage",
	"OnDamageStateChange",
	"OnEffectHit",
	"OnGangDamageStateChange",
	"OnLSC",
	"OnSpecialFX",
	"OnTarget",
	"RemoveBalls",
	"SendMessage",
	"SetMaxSpeed",
	"SetSpeedFraction",
	"TerminalExplosion",
	"address",
	"alert",
	"allianceID",
	"allianceid",
	"bid",
	"bookmark",
	"bounty",
	"channel",
	"charid",
	"constellationid",
	"corpID",
	"corpid",
	"corprole",
	"damage",
	"duration",
	"effects.Laser",
	"gangid",
	"gangrole",
	"hqID",
	"issued",
	"jit",
	"languageID",
	"locationid",
	"machoVersion",
	"marketProxy",
	"minVolume",
	"orderID",
	"price",
	"range",
	"regionID",
	"regionid",
	"role",
	"rolesAtAll",
	"rolesAtBase",
	"rolesAtHQ",
	"rolesAtOther",
	"shipid",
	"sn",
	"solarSystemID",
	"solarsystemid",
	"solarsystemid2",
	"source",
	"splash",
	"stationID",
	"stationid",
	"target",
	"userType",
	"userid",
	"volEntered",
	"volRemaining",
	"weapon",
	"agent.StorylineMissionDetails",
	"agent.ResearchMissionDetails",
	"agent.OfferDetails",
	"agent.StandardMissionDetails",
	"agent.Location",
	"agent.MissionReferral",
	"agent.TimeBonusReward",
	"agent.Reward",
	"agent.TransportObjective",
	"agent.DungeonObjective",
	"agent.EncounterObjective",
	"agent.FetchObjective",
	"agent.Objective",
	"agent.Entity",
	"agent.Item",
	"agent.Credits",
	"agent.ResearchPoints",
	"agent.LoyaltyPoints",
	"agent.offerTemplatizedContent_BasicExchangeOffer_Crafting",
	"agent.offerTemplatizedContent_BasicExchangeOffer_ContrabandDemand",
	"agent.offerTemplatizedContent_BasicExchangeOffer",
	"agent.missionTemplatizedContent_GenericStorylineTradeMission",
	"agent.missionTemplatizedContent_StorylineTradeMission",
	"agent.missionTemplatizedContent_ResearchTradeMission",
	"agent.missionTemplatizedContent_BasicTradeMission",
	"agent.missionTemplatizedContent_GenericStorylineCourierMission",
	"agent.missionTemplatizedContent_StorylineCourierMission",
	"agent.missionTemplatizedContent_ResearchCourierMission",
	"agent.missionTemplatizedContent_BasicCourierMission",
	"agent.missionTemplatizedContent_GenericStorylineKillMission",
	"agent.missionTemplatizedContent_StorylineKillMission",
	"agent.missionTemplatizedContent_ResearchKillMission",
	"agent.missionTemplatizedContent_BasicKillMission"
};

#define sPyStringTable PyMarshalStringTable::getSingleton()

#endif//PY_STRING_TABLE_H
