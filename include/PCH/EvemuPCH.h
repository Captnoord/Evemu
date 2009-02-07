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

#ifndef __EVEPCH_H
#define __EVEPCH_H

#include "ascent.h"

//4355

#include <algorithm>

#include "PyStringTable.h"
#include "../Communication/PyStream.h"
#include "FileLogger.h"
#include "DBRow.h"

#include "PyObjects.h"
#include "MarshalStream.h"


#include "../Communication/packet_types.h"
#include "../Communication/EVEVersion.h"

/* Ascent new stuff */
#include "AccountMgr.h"
#include "EveSession.h"
#include "../server/EveClientSocket.h"
#include "../server/EveClientSession.h"

#include "../server/Space/Space.h"
#include "../server/Space/SpaceRunnable.h"
#include "../server/Space/RegionMapManager.h"

#include "../server/ClientSession.h"

#include "../TestCases/TestCaseMarshal.h"

/*Database handles*/
extern Database* Database_dynamic;
extern Database* Database_static;

// for static/read only data.
#define StaticDatabase (*Database_static)

// for dynamic saving/loading server related data.
#define DynamicDatabase (*Database_dynamic)

#endif//__EVEPCH_H
