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

#include "FactionWarMgrDB.h"

#include "../common/PyRep.h"
#include "../common/DBcore.h"

FactionWarMgrDB::FactionWarMgrDB(DBcore *db)
: ServiceDB(db)
{
}

bool FactionWarMgrDB::GetFacWarSystems(PyRepDict &into) {
	DBQueryResult res;

	_log(DATABASE__MESSAGE, "GetFacWarSystems unimplemented.");

	//fill some crap
	PyRepDict *dict = new PyRepDict;
	dict->add("occupierID", new PyRepInteger(500002));
	dict->add("factionID", new PyRepInteger(500002));
	into.add(new PyRepInteger(30002097), dict);

	return(true);
}

