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

#include "FactionWarMgrService.h"
#include "../Client.h"
#include "../PyServiceCD.h"
#include "../cache/ObjCacheService.h"

#include "../packets/General.h"

PyCallable_Make_InnerDispatcher(FactionWarMgrService)

FactionWarMgrService::FactionWarMgrService(PyServiceMgr *mgr, DBcore *db)
: PyService(mgr, "facWarMgr"),
  m_dispatch(new Dispatcher(this)),
  m_db(db)
{
	_SetCallDispatcher(m_dispatch);

	PyCallable_REG_CALL(FactionWarMgrService, GetWarFactions)
	PyCallable_REG_CALL(FactionWarMgrService, GetFacWarSystems)
	PyCallable_REG_CALL(FactionWarMgrService, GetCharacterRankOverview)
}

PyCallResult FactionWarMgrService::Handle_GetWarFactions(PyCallArgs &call) {
	_log(SERVICE__ERROR, "%s::GetWarFactions unimplemented.", GetName());

	return(new PyRepList());
}

PyCallResult FactionWarMgrService::Handle_GetFacWarSystems(PyCallArgs &call) {
	_log(SERVICE__ERROR, "%s::GetFacWarSystems not fully unimplemented.", GetName());

	ObjectCachedMethodID method_id(GetName(), "GetFacWarSystems");

	if(!m_manager->GetCache()->IsCacheLoaded(method_id)) {
		PyRepDict *dict = new PyRepDict;
		if(!m_db.GetFacWarSystems(*dict))
			return(NULL);
		m_manager->GetCache()->GiveCache(method_id, (PyRep **)&dict);
	}

	return(m_manager->GetCache()->MakeObjectCachedMethodCallResult(method_id));
}

PyCallResult FactionWarMgrService::Handle_GetCharacterRankOverview(PyCallArgs &call) {
	Call_SingleIntegerArg arg;
	if(!arg.Decode(&call.tuple)) {
		_log(SERVICE__ERROR, "Failed to decode args.");
		return(NULL);
	}

	_log(SERVICE__WARNING, "%s::GetCharacterRankOverview(charid=%lu) unimplemented.", GetName(), arg.arg);

	util_Rowset rs;

	rs.header.push_back("currentRank");
	rs.header.push_back("highestRank");
	rs.header.push_back("factionID");
	rs.header.push_back("lastModified");

	return(rs.Encode());
}


