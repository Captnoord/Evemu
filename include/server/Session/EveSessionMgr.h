/**
 * \class EveSessionMgr
 *
 * @brief a brief eve session manager to keep track of the sessions
 *
 * 
 *
 * @author Captnoord
 * @date January 2009
 */

#define EveSecond 10000000
#define EveSessionChangeDelay (30 * EveSecond)

class PyObject;


class Item;
class Inventory;
class EveMemStream;
class PyDict;

class EveSession;

typedef enum
{
	allianceid,
	charid,
	clientID,
	constellationid,
	corpid,
	gangid,
	multicastID,
	nodeID,
	objectID,
	regionid,
	squadid,
	shipid,
	stationid,
	locationid,
	solarsystemid,
	solarsystemid2,
	userid,
	wingid,
} idTypes;

static const char* idTypeNames[] = 
{
	"allianceid",
	"charid",
	"clientID",
	"constellationid",
	"corpid",
	"gangid",
	"multicastID",
	"nodeID",
	"objectID",
	"regionid",
	"squadid",
	"shipid",
	"stationid",
	"locationid",
	"solarsystemid",
	"solarsystemid2",
	"userid",
	"wingid",
};

static const char* GetidTypeName(idTypes type)
{
	return idTypeNames[type];
}

/* singleton??? */

/*
OnInventoryChange
OnMachoObjectDisconnect
OnSessionEnd
OnSessionChanged
OnRun
OnSessionAttach
OnSessionDetach
OnStop
OnObjectPublicAttributesUpdated
OnSessionMutated
*/

/* Eve Session mgr notify events
__notifyevents__ = ['ProcessInventoryChange', 'ProcessSessionChange', 'DoSessionChanging']
*/
class EveSessionMgr
{
public:
	EveSessionMgr();

	void GetSessionStatistics() {};
	//void PerformSessionChange(std::string &sessionChangeReason, func, *args, **keywords):
	void UpdateSessionAttributes(idTypes idtype, uint32 theID, PyDict *dict) {};
	void CloseUserSessions(uint32 *userIDs, std::string &reason, uint32 clientID = 0) {};
	void PerformSessionAttributeUpdate(uint32 clientID, PyDict *dict); // hackish... horrable as described in the client
	void EndAllGameSessions(bool remote = false);
	
	void GetProxyNodeFromID(idTypes idtype, uint32 theID, bool refresh = false);
	void GetClientIDFromID(idTypes idtype, uint32 theID, bool refresh = false);
	//void ConnectToClientService(svc, idTypes idtype, uint32 theID); // not very useful to me atm
	void GetLocalClientIDs() {};
	//void BatchedRemoteCall(batchedCalls); // unable to clarify atm.
	void GetSessionDetails(uint32 clientID, uint32 sid);
private:
	void init() {};

	void GetProxySessionManager(uint32 nodeID);
	void __GetObjectTypeAndGuts(PyObject &object);//Helper function to rip the type name and the actual object out of an object or proxy object
	void __RaisePSCIP(std::string &oldReason, std::string &newReason, time_t timeLeft = 0);
	//void PerformSessionLockedOperation(*args, **keywords):
	void ProcessSessionChange(bool isRemote, EveSession *sess, PyDict *change);
	void GetSessionFromParams(idTypes idtype, uint32 theID);
	void Run(EveMemStream * memstream = NULL);
	void ProcessInventoryChange(Item &item, PyDict &change, bool isRemote, Inventory &inventory2);
	void GetSessionValuesFromItemID(uint32 itemID, Inventory &inventory2 = NULL, Item &theItem = NULL);
	void GetInitialValuesFromCharID(uint32 charID);
};
