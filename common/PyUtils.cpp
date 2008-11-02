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

#include "PyUtils.h"
#include "logsys.h"
#include "PyRep.h"

bool ParseIntegerList(const PyRepTuple *iargs, const char *caller, std::vector<uint32> &into)
{
	if(iargs->items.size() != 1)
	{
		_log(SERVICE__ERROR, "Invalid arg count to %s: %d", caller, iargs->items.size());
		//TODO: throw exception
		return false;
	}
	else if(!iargs->items[0]->CheckType(PyRep::List))
	{
		_log(SERVICE__ERROR, "Invalid argument to %s: got %s, list expected", caller, iargs->items[0]->TypeString());
		return false;
	}
	
	const PyRepList *chanlist = (const PyRepList *) iargs->items[0];

	PyRepList::const_iterator cur, end;
	cur = chanlist->begin();
	end = chanlist->end();
	for(; cur != end; cur++)
	{
		if((*cur)->CheckType(PyRep::Integer) == false)
		{
			_log(SERVICE__ERROR, "Invalid argument to %s: got %s, int expected", caller, (*cur)->TypeString());
			return false;
		}
		//const PyRepInteger *i = (const PyRepInteger *) *cur;
		uint32 val = ((PyRepInteger *)(*cur))->GetUint32();
		into.push_back(val);
	}
	return true;
}


















