/*  EVEmu: EVE Online Server Emulator

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; version 2 of the License.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY except by those people which sell it, which
  are required to give you total support for your newly bought product;
  without even the implied warranty of MERCHANTABILITY or FITNESS FOR
  A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
	
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*
Dynamic Bodies:
	- shape
	- `mass`
	- `radius`
	- `volume`???
	- `Inertia`???
	- inertia information
	- position
	- velocity
	- angular velocity
	- collide with things
		- approximate eve "run into and stop/turn around" collisions

Ship: extends Dynamic
	- `maxVelocity`
	- thrust (propulsion + speed ratio)
	- angular thrust
	- "stopping" algorithm
	- "orbit" algorithm
	- "turning" algorithm 

Static Bodies:
	- shape
	- position

detect clients moving into agro radius
*/

#ifndef EVE_CLIENT_H
#define EVE_CLIENT_H

#include "system/SystemEntity.h"
#include "ship/ModuleManager.h"
#include "ClientSession.h"

#include "ascent.h"


/* class name helpers */
class Client;

class PyPacket;
class PyRep;
class PyRepDict;
class PyRepTuple;
class PyRepList;
class PyRepSubStream;
class PyCallStream;
class PyServiceMgr;
class PyAddress;
class EVENotificationStream;
class InventoryItem;
class LSCChannel;
class SystemManager;
class CryptoChallengePacket;
class DynamicSystemEntity;

// struct character info
struct CharacterData
{
	uint32 charid;
	
	std::string name;
	std::string title;
	std::string description;

	double bounty;
	double balance;
	double securityRating;
	uint32 logonMinutes;

	uint32 corporationID;
	uint64 corporationDateTime;
	uint32 allianceID;
	
	uint32 stationID;
	uint32 solarSystemID;
	uint32 constellationID;
	uint32 regionID;

	uint32 typeID;
	uint32 bloodlineID;
	uint32 genderID;
	uint32 ancestryID;
	uint32 careerID;
	uint32 schoolID;
	uint32 careerSpecialityID;
	
	uint32 Intelligence;
	uint32 Charisma;
	uint32 Perception;
	uint32 Memory;
	uint32 Willpower;

	uint64 startDateTime;
	uint64 createDateTime;

	uint8 raceID;	//must correspond to our bloodlineID!

	void ModifyAttributes(uint32 add_int, uint32 add_cha, uint32 add_per, uint32 add_mem, uint32 add_wll) {
		Intelligence += add_int;
		Charisma += add_cha;
		Perception += add_per;
		Memory += add_mem;
		Willpower += add_wll;
	}
};

class CharacterAppearance
{
public:
	CharacterAppearance();
	CharacterAppearance(const CharacterAppearance &from);
	~CharacterAppearance();

	bool IsNull_accessoryID() const { return(accessoryID == 0); } uint32 Get_accessoryID() const { return(IsNull_accessoryID() ? 0 : *accessoryID); } void Set_accessoryID(uint32 val) { Clear_accessoryID(); accessoryID = new uint32(val); } void Clear_accessoryID() { if(!IsNull_accessoryID()) delete accessoryID; accessoryID = 0; }
	bool IsNull_beardID() const { return(beardID == 0); } uint32 Get_beardID() const { return(IsNull_beardID() ? 0 : *beardID); } void Set_beardID(uint32 val) { Clear_beardID(); beardID = new uint32(val); } void Clear_beardID() { if(!IsNull_beardID()) delete beardID; beardID = 0; }
	bool IsNull_decoID() const { return(decoID == 0); } uint32 Get_decoID() const { return(IsNull_decoID() ? 0 : *decoID); } void Set_decoID(uint32 val) { Clear_decoID(); decoID = new uint32(val); } void Clear_decoID() { if(!IsNull_decoID()) delete decoID; decoID = 0; }
	bool IsNull_lipstickID() const { return(lipstickID == 0); } uint32 Get_lipstickID() const { return(IsNull_lipstickID() ? 0 : *lipstickID); } void Set_lipstickID(uint32 val) { Clear_lipstickID(); lipstickID = new uint32(val); } void Clear_lipstickID() { if(!IsNull_lipstickID()) delete lipstickID; lipstickID = 0; }
	bool IsNull_makeupID() const { return(makeupID == 0); } uint32 Get_makeupID() const { return(IsNull_makeupID() ? 0 : *makeupID); } void Set_makeupID(uint32 val) { Clear_makeupID(); makeupID = new uint32(val); } void Clear_makeupID() { if(!IsNull_makeupID()) delete makeupID; makeupID = 0; }

	uint32 costumeID;
	uint32 eyebrowsID;
	uint32 eyesID;
	uint32 hairID;
	uint32 skinID;
	uint32 backgroundID;
	uint32 lightID;

	double headRotation1;
	double headRotation2;
	double headRotation3;

	double eyeRotation1;
	double eyeRotation2;
	double eyeRotation3;

	double camPos1;
	double camPos2;
	double camPos3;

	bool IsNull_morph1e() const { return(morph1e == 0); } double Get_morph1e() const { return(IsNull_morph1e() ? 0 : *morph1e); } void Set_morph1e(double val) { Clear_morph1e(); morph1e = new double(val); } void Clear_morph1e() { if(!IsNull_morph1e()) delete morph1e; morph1e = 0; }
	bool IsNull_morph1n() const { return(morph1n == 0); } double Get_morph1n() const { return(IsNull_morph1n() ? 0 : *morph1n); } void Set_morph1n(double val) { Clear_morph1n(); morph1n = new double(val); } void Clear_morph1n() { if(!IsNull_morph1n()) delete morph1n; morph1n = 0; }
	bool IsNull_morph1s() const { return(morph1s == 0); } double Get_morph1s() const { return(IsNull_morph1s() ? 0 : *morph1s); } void Set_morph1s(double val) { Clear_morph1s(); morph1s = new double(val); } void Clear_morph1s() { if(!IsNull_morph1s()) delete morph1s; morph1s = 0; }
	bool IsNull_morph1w() const { return(morph1w == 0); } double Get_morph1w() const { return(IsNull_morph1w() ? 0 : *morph1w); } void Set_morph1w(double val) { Clear_morph1w(); morph1w = new double(val); } void Clear_morph1w() { if(!IsNull_morph1w()) delete morph1w; morph1w = 0; }
	bool IsNull_morph2e() const { return(morph2e == 0); } double Get_morph2e() const { return(IsNull_morph2e() ? 0 : *morph2e); } void Set_morph2e(double val) { Clear_morph2e(); morph2e = new double(val); } void Clear_morph2e() { if(!IsNull_morph2e()) delete morph2e; morph2e = 0; }
	bool IsNull_morph2n() const { return(morph2n == 0); } double Get_morph2n() const { return(IsNull_morph2n() ? 0 : *morph2n); } void Set_morph2n(double val) { Clear_morph2n(); morph2n = new double(val); } void Clear_morph2n() { if(!IsNull_morph2n()) delete morph2n; morph2n = 0; }
	bool IsNull_morph2s() const { return(morph2s == 0); } double Get_morph2s() const { return(IsNull_morph2s() ? 0 : *morph2s); } void Set_morph2s(double val) { Clear_morph2s(); morph2s = new double(val); } void Clear_morph2s() { if(!IsNull_morph2s()) delete morph2s; morph2s = 0; }
	bool IsNull_morph2w() const { return(morph2w == 0); } double Get_morph2w() const { return(IsNull_morph2w() ? 0 : *morph2w); } void Set_morph2w(double val) { Clear_morph2w(); morph2w = new double(val); } void Clear_morph2w() { if(!IsNull_morph2w()) delete morph2w; morph2w = 0; }
	bool IsNull_morph3e() const { return(morph3e == 0); } double Get_morph3e() const { return(IsNull_morph3e() ? 0 : *morph3e); } void Set_morph3e(double val) { Clear_morph3e(); morph3e = new double(val); } void Clear_morph3e() { if(!IsNull_morph3e()) delete morph3e; morph3e = 0; }
	bool IsNull_morph3n() const { return(morph3n == 0); } double Get_morph3n() const { return(IsNull_morph3n() ? 0 : *morph3n); } void Set_morph3n(double val) { Clear_morph3n(); morph3n = new double(val); } void Clear_morph3n() { if(!IsNull_morph3n()) delete morph3n; morph3n = 0; }
	bool IsNull_morph3s() const { return(morph3s == 0); } double Get_morph3s() const { return(IsNull_morph3s() ? 0 : *morph3s); } void Set_morph3s(double val) { Clear_morph3s(); morph3s = new double(val); } void Clear_morph3s() { if(!IsNull_morph3s()) delete morph3s; morph3s = 0; }
	bool IsNull_morph3w() const { return(morph3w == 0); } double Get_morph3w() const { return(IsNull_morph3w() ? 0 : *morph3w); } void Set_morph3w(double val) { Clear_morph3w(); morph3w = new double(val); } void Clear_morph3w() { if(!IsNull_morph3w()) delete morph3w; morph3w = 0; }
	bool IsNull_morph4e() const { return(morph4e == 0); } double Get_morph4e() const { return(IsNull_morph4e() ? 0 : *morph4e); } void Set_morph4e(double val) { Clear_morph4e(); morph4e = new double(val); } void Clear_morph4e() { if(!IsNull_morph4e()) delete morph4e; morph4e = 0; }
	bool IsNull_morph4n() const { return(morph4n == 0); } double Get_morph4n() const { return(IsNull_morph4n() ? 0 : *morph4n); } void Set_morph4n(double val) { Clear_morph4n(); morph4n = new double(val); } void Clear_morph4n() { if(!IsNull_morph4n()) delete morph4n; morph4n = 0; }
	bool IsNull_morph4s() const { return(morph4s == 0); } double Get_morph4s() const { return(IsNull_morph4s() ? 0 : *morph4s); } void Set_morph4s(double val) { Clear_morph4s(); morph4s = new double(val); } void Clear_morph4s() { if(!IsNull_morph4s()) delete morph4s; morph4s = 0; }
	bool IsNull_morph4w() const { return(morph4w == 0); } double Get_morph4w() const { return(IsNull_morph4w() ? 0 : *morph4w); } void Set_morph4w(double val) { Clear_morph4w(); morph4w = new double(val); } void Clear_morph4w() { if(!IsNull_morph4w()) delete morph4w; morph4w = 0; }

	void Build(const std::map<std::string, PyRep *> &from);
	void operator=(const CharacterAppearance &from);

protected:

	uint32 *accessoryID;
	uint32 *beardID;

	uint32 *decoID;
	uint32 *lipstickID;
	uint32 *makeupID;

	double *morph1e;
	double *morph1n;
	double *morph1s;
	double *morph1w;
	double *morph2e;
	double *morph2n;
	double *morph2s;
	double *morph2w;
	double *morph3e;
	double *morph3n;
	double *morph3s;
	double *morph3w;
	double *morph4e;
	double *morph4n;
	double *morph4s;
	double *morph4w;
};

class CorpMemberInfo {
public:
	CorpMemberInfo()
	: corpHQ(0), corprole(0), rolesAtAll(0), rolesAtBase(0), rolesAtHQ(0), rolesAtOther(0) {}
	uint32 corpHQ;	//this really doesn't belong here...
	uint64 corprole;
	uint64 rolesAtAll;
	uint64 rolesAtBase;
	uint64 rolesAtHQ;
	uint64 rolesAtOther;
};

class Functor {
public:
	Functor() {}
	virtual ~Functor() {}
	virtual void operator()() = 0;
};

class ClientFunctor : public Functor {
public:
	ClientFunctor(Client *c) : m_client(c) {}
	virtual ~ClientFunctor() {}
	//leave () pure
protected:
	Client *const m_client;
};

class FunctorTimerQueue {
public:
	typedef uint32 TimerID;
	
	FunctorTimerQueue();
	~FunctorTimerQueue();

	//schedule takes ownership of the functor.
	TimerID Schedule(Functor **what, uint32 in_how_many_ms);
	bool Cancel(TimerID id);
	
	void Process();
	
protected:
	//this could be done a TON better, but want to get something
	//working first, it is call encapsulated in here, so it should
	//be simple to gut this object and put in a real scheduler.
	class Entry {
	public:
		Entry(TimerID id, Functor *func, uint32 time_ms);
		~Entry();
		const TimerID id;
		Functor *const func;
		Timer when;
	};
	TimerID m_nextID;
	std::vector<Entry *> m_queue;	//not ordered or anything useful.
};

//DO NOT INHERIT THIS OBJECT!
class Client : public DynamicSystemEntity {
public:

	Client(EveClientSession* xsession);
	virtual ~Client();

	// session info
	ClientSession session;

	// ships module manager....... shouldn't this be in a ship class or so...
	ModuleManager modules;

	void	QueuePacket(PyPacket *p);
	void	FastQueuePacket(PyPacket *p);


	virtual void	Update();
	virtual void	ProcessDestiny();

	uint32 GetAccountID() const { return(m_accountID); }
	uint32 GetRole() const { return(m_role); }

	uint32 GetCharacterID() const { return(m_char.charid); }
	const CharacterData &GetChar() const { return(m_char); }
	uint32 GetCorporationID() const { return(m_char.corporationID); }
	uint32 GetAllianceID() const { return(m_char.allianceID); }
	const CorpMemberInfo &GetCorpInfo() const { return(m_corpstate); }

	uint32 GetLocationID() const { return(IsInSpace() ? GetSystemID() : GetStationID()); }
	uint32 GetStationID() const { return(m_char.stationID); }
	uint32 GetSystemID() const { return(m_char.solarSystemID); }
	uint32 GetConstellationID() const { return(m_char.constellationID); }
	uint32 GetRegionID() const { return(m_char.regionID); }
	inline double x() const { return(GetPosition().x); }	//this is terribly inefficient.
	inline double y() const { return(GetPosition().y); }	//this is terribly inefficient.
	inline double z() const { return(GetPosition().z); }	//this is terribly inefficient.
	bool IsInSpace() const { return(GetStationID() == 0); }
	
	double GetBalance() const { return m_char.balance; }
	bool AddBalance(double amount);

	void Login(CryptoChallengePacket *pack);

	uint32 GetShipID() const;
	void BoardShip(InventoryItem *new_ship);
	void MoveToLocation(uint32 location, const GPoint &pt);
	void MoveToPosition(const GPoint &pt);
	void MoveItem(uint32 itemID, uint32 location, EVEItemFlags flag);
	bool EnterSystem();
	bool Load(uint32 char_id);
	void JoinCorporationUpdate(uint32 corp_id);
	inline InventoryItem *Ship() const { return(m_ship); }
	void SavePosition();
	
	double GetPropulsionStrength() const;
	
	bool LaunchDrone(InventoryItem *drone);
	
	void SendNotification(const PyAddress &dest, EVENotificationStream *noti, bool seq=true);
	void SendNotification(const char *notifyType, const char *idType, PyRepTuple **payload, bool seq=true);
	void SessionSync();

	//destiny stuff...
	void WarpTo(const GPoint &p);
	void StargateJump(uint32 fromGate, uint32 toGate);
	
	//SystemEntity interface:
	virtual EntityClass GetClass() const { return(ecClient); }
	virtual bool IsClient() const { return true; }
	virtual Client *CastToClient() { return(this); }
	virtual const Client *CastToClient() const { return(this); }

	virtual uint32 GetID() const { return GetShipID(); }	//our entity in space is our ship!
	virtual const char *GetName() const { return(m_char.name.c_str()); }
	virtual double GetRadius() const;
	virtual PyRepDict *MakeSlimItem() const;
	virtual void MakeDamageState(DoDestinyDamageState &into) const;
	virtual void QueueDestinyUpdate(PyRepTuple *du);
	virtual void QueueDestinyEvent(PyRepTuple *multiEvent);

	virtual void TargetAdded(SystemEntity *who);
	virtual void TargetLost(SystemEntity *who);
	virtual void TargetedAdd(SystemEntity *who);
	virtual void TargetedLost(SystemEntity *who);
	virtual void TargetsCleared();

	virtual double GetMass() const;
	virtual double GetMaxVelocity() const;
	virtual double GetAgility() const;

	virtual void ApplyDamageModifiers(Damage &d, SystemEntity *target);
	virtual void ApplyDamage(Damage &d);
	virtual void Killed(Damage &fatal_blow);
	virtual SystemManager *System() const { return(m_system); }
	
	void SendErrorMsg(const char *fmt, ...);
	void SendNotifyMsg(const char *fmt, ...);
	void SendInfoModalMsg(const char *fmt, ...);
	void SelfChatMessage(const char *fmt, ...);
	void SelfEveMail(const char *subject, const char *fmt, ...);
	void ChannelJoined(LSCChannel *chan);
	void ChannelLeft(LSCChannel *chan);

	// easy send function....
	void Send(PyPacket* packet)
	{
		if (mClientSession)
		{
			mClientSession->Send(packet);
		}
		else
			return;
	}
	
	void _ProcessNotification(PyPacket *packet);
	//call stuff
	void _ProcessCallRequest(PyPacket *packet);
	void _SendCallReturn(PyPacket *req, PyRep **return_value, const char *channel = NULL);
	void _SendException(PyPacket *req, MACHONETERR_TYPE type, PyRep **payload);

protected:
	void _SendPingRequest();
	void _CheckSessionChange();
	void _ReduceDamage(Damage &d);
	
	InventoryItem *m_ship;

	PyServiceMgr *const m_services;

	EveClientSession* mClientSession;

	Timer m_pingTimer;

	uint32 m_accountID;
	uint32 m_role;		//really should be a uint64
	uint32 m_gangRole;

	SystemManager *m_system;	//we do not own this

	CharacterData m_char;
	CorpMemberInfo m_corpstate;

	std::set<LSCChannel *> m_channels;	//we do not own these.
	
	//this whole move system is a piece of crap:
	typedef enum
	{
		msIdle,
		msJump
	} _MoveState;
	
	void _postMove(_MoveState type, uint32 wait_ms=500);
	_MoveState m_moveState;
	Timer m_moveTimer;
	uint32 m_moveSystemID;
	GPoint m_movePoint;
	void _ExecuteJump();
	
private:

	//FunctorTimerQueue m_delayQueue;

	/************************************************************************/
	/* Destiny related stuff                                                */
	/************************************************************************/

	// atm we assume we don't need locks here...
	//typedef FastQueue<PyRep *,DummyLock> DestinyEventQueue;
	//typedef FastQueue<PyRepTuple *,DummyLock> DestinyActionQueue;
	
	//DestinyEventQueue m_destinyEventQueue;
	//DestinyActionQueue m_destinyUpdateQueue;

	//queues for destiny updates:
	std::vector<PyRep *> m_destinyEventQueue;			//we own these. These are events as used in OnMultiEvent
	std::vector<PyRepTuple *> m_destinyUpdateQueue;		//we own these. They are the `update` which go into DoDestinyAction

	void _SendQueuedUpdates(uint32 stamp);
	
	uint32 m_nextNotifySequence;
};

//simple functor for void calls.
//not needed right now
/*class SimpleClientFunctor : public ClientFunctor {
public:
	typedef void (Client::* clientCall)();
	SimpleClientFunctor(Client *c, clientCall call) : ClientFunctor(c), m_call(call) {}
	virtual ~SimpleClientFunctor() {}
	virtual void operator()() {
		(m_client->*m_call)();
	}
public:
	const clientCall m_call;
};*/

#endif
