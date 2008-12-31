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

class PyPacket
{
	PyPacket() {}
};

#include "EVEMarshalOpcodes.h"
#include "PyStringTable.h"
#include "PyStream.h"
#include "FileLogger.h"

#include "../common/packet_types.h"
#include "../common/EVEVersion.h"

/* Ascent new stuff */
#include "../common/EveClientSocket.h"
#include "../common/EveClientSession.h"
#include "../common/Space.h"
#include "../common/SpaceRunnable.h"

#include "../server/ClientSession.h"



/* db stuff */
extern Database * Database_Generic;

#define GenericDatabase (*Database_Generic)

#endif//__EVEPCH_H
