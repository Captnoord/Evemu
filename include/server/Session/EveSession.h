#ifndef _EVEINFOSESSION_H
#define _EVEINFOSESSION_H

/**
 * \class PyItr
 *
 * @brief a class that has acts like a single object of a PyDictionary
 *
 * 
 *
 * @author Captnoord
 * @date January 2009
 */
class PySessionAttribute
{
public:
	PySessionAttribute() : mEntry(10), mIsDirty() {}

	template<typename T>
	PySessionAttribute &operator=(const T &value) {

		// this is a small hack...
		if (mEntry.size() == 0)
			mEntry << value;
		else
		{
			mEntry.reset();
			mEntry << value;
		}
		
		return *this;
	}

	template<typename T>
	void change(T & towhat)
	{
		mEntry.reset();
		mEntry << towhat;
		mIsDirty = true;
	}

	void reset() {mEntry.reset();}

protected:
	PyStream mEntry;
	bool mIsDirty;
};

/**
 * \class EveInfoSession
 *
 * @brief small utility class to wrap the std::tr1::unorded_map functionality into a PyDict style
 *
 * 
 *
 * @author Captnoord
 * @date January 2009
 */
class SessionDict
{
public:
	SessionDict();

	PySessionAttribute &operator[](const char* keyName) {
		return _lookupAttribute(keyName);
	}

	template<typename T>
	void ChangeAttribute(const char* attr, T & towhat)
	{
		PySessionAttribute &attribute = _lookupAttribute(attr);
		attribute.change(towhat);
	}

private:
	PySessionAttribute &_lookupAttribute(const char * attr)
	{
		DictMapConstItr Itr = mDictStorage.find(attr);

		if (Itr != mDictStorage.end())
		{
			return *((PySessionAttribute*)&Itr->second);
		}
		else
		{
			//std::pair<iterator, bool> insert(const value_type& val);

			std::pair<DictMapItr, bool> ret = mDictStorage.insert(std::make_pair(attr, PySessionAttribute()));
			return *((PySessionAttribute*)&ret.first);
		}
	}


protected:
	typedef std::tr1::unordered_map<std::string, PySessionAttribute>	DictMap;
	typedef DictMap::iterator											DictMapItr;
	typedef DictMap::const_iterator										DictMapConstItr;

	DictMap mDictStorage;
};

/**
 * \class EveInfoSession
 *
 * @brief the info session keeps track of often used data.
 *
 * 
 *
 * @author Captnoord
 * @date January 2009
 */
class EveSession
{
public:
	EveSession(uint32 sid);

	// operator pass trough...
	/*PySessionAttribute &operator[](const char* keyName) {
		return mSessionDict[keyName];
	}*/

	void init(uint32 sid, uint32 role);

	/* utility functions */

	PySessionAttribute GetDefaultValueOfAttribute(const char *attribute);

	template<typename T>
	void ChangeAttribute(const char* attr, T & towhat)
	{
		mSessionDict.ChangeAttribute(attr, towhat);
	}

	void LogSessionHistory(const char *reason, const char *details = NULL, bool noBlather = false);

	bool IsMutating();
	bool IsChanging();
	bool IsItSafe();
	bool IsItSemiSafe();
	
#if 0
	void RegisterMachoObjectConnection(, objectID, connection, refID);
	void UnregisterMachoObjectConnection(, objectID, connection);
	void RegisterMachoObject(, objectID, object, refID);
	void UnregisterMachoObject(, objectID, refID, suppressnotification = 1);

	/*
	Notification ID's are used by macho to send sequential notifications to a
	client.
	*/
	void GetNotificationID();
	void DumpSession(, prefix, reason);
	
	/*
	Enters a masquerading mode for this session, until our mask is removed and
	the lights are turned on.	
	*/
	void Masquerade(, props = None);
	
	/*'\n            Since the global session object is now a true Sissy, we need a method to\n            acquire a non-Sissy version of ourselves when people are storing the\n            global session for future use.\n        '*/
	void GetActualSession();
	void Callback(, fn, *args, **keywords);
	
	void LogSessionHistory(, reason, details = None, noBlather = 0);
	void LogSessionError(, what, novalidate = 0);
	void LogSessionWarn(, what);

	void __LogSessionProblem(, what, how, novalidate = 0);
	void SetSessionVariable(, k, v);
	void GetSessionVariable(, k, defaultValue = None);
	void GetTime();
	void GetDistributedProps(, orwhat);
	void DependantAttributes(, attribute);
	void GetDefaultValueOfAttribute(, attribute);
	void ClearAttributes(, isRemote = 0);
	void ValidateSession(, prefix);
	void __ChangeAttribute(, attr, towhat);
	void SetAttributes(, requested, isRemote = 0);
	void ApplyRemoteAttributeChanges(, clueless, changes);
	void ApplyInitialState(, initialstate);
	void __repr__();
	void __setattr__(, attr, value);
	void DisconnectObject(, object, key = None, delaySecs = 1);
	void RedirectObject(, object, serviceName = None, bindParams = None, key = None);
	void GetConnectionVariable(, object, k, defaultValue = None);
	void SetConnectionVariable(, object, k, v);
	void GetInstanceVariable(, object, k);
	void SetInstanceVariable(, object, k, v);
	void ConnectToObject(, object, serviceName = None, bindParams = None);
	void ConnectToClientService(, svc, idtype = None, theID = None);
	void ConnectToService(, svc, **keywords);
	void ConnectToFuture(, svc, when);
	void ConnectToAllServices(, svc, batchInterval = 0);
	void ConnectToRemoteService(, svc, nodeID = None);
	void ConnectToSolServerService(, svc, nodeID = None);
	void ConnectToProxyServerService(, svc, nodeID = None);
	void ConnectToAnyService(, svc);
	void ConnectToAllNeighboringServices(, svc, batchInterval = 0);
	void ConnectToAllProxyServerServices(, svc, batchInterval = 0);
	void ConnectToAllSolServerServices(, svc, batchInterval = 0);
	void RemoteServiceCall(, dest, service, method, *args, **keywords);
	void RemoteServiceCallWithoutTheStars(, dest, service, method, *args, **keywords);
	void RemoteAnyServiceCall(, service, method, *args, **keywords);
	void RemoteServiceNotify(, dest, service, method, *args, **keywords);
	void RemoteServiceNotifyWithoutTheStars(, dest, service, method, args, keywords);
	void RemoteAnyServiceNotify(, service, method, *args, **keywords);
	void ResetSessionChangeTimer(, reason);
#endif

protected:
	SessionDict mSessionDict;
};


#endif //_EVEINFOSESSION_H
