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



#include "NetService.h"
#include "../common/logsys.h"
#include "../common/PyRep.h"
#include "../common/PyPacket.h"
#include "../common/EVEUtils.h"
#include "Client.h"
#include "PyServiceCD.h"
#include "PyServiceMgr.h"
#include "cache/ObjCacheService.h"

PyCallable_Make_InnerDispatcher(NetService)

NetService::NetService(PyServiceMgr *mgr)
: PyService(mgr, "machoNet"),
m_dispatch(new Dispatcher(this))
{
	_SetCallDispatcher(m_dispatch);

	PyCallable_REG_CALL(NetService, GetInitVals)
	PyCallable_REG_CALL(NetService, GetTime)
}

NetService::~NetService() {
	delete m_dispatch;
}

PyCallResult NetService::Handle_GetInitVals(PyCallArgs &call)
{
	PyRep *result = NULL;

	PyRepTuple *main = new PyRepTuple(2);

        PyRep *serverinfo = call.client->GetServices()->GetCache()->GetCacheHint("machoNet.serviceInfo");
	if(serverinfo == NULL) {
		_log(CLIENT__ERROR, "Unable to load cache hint for machoNet.serviceInfo");
		return(new PyRepNone());
	}

        PyRepDict *initvals = new PyRepDict();
        //send all the cache hints needed for server info.
        call.client->GetServices()->GetCache()->InsertCacheHints(ObjCacheService::hLoginCachables, initvals);

        main->items[0] = serverinfo;
        main->items[1] = initvals;

        result = main;

	return(result);
}
PyCallResult NetService::Handle_GetTime(PyCallArgs &call)
{
	PyRep *result = NULL;
	result = new PyRepInteger(Win32TimeNow());
	return(result);
}




















