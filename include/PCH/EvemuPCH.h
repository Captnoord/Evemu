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
	Author:		Captnoord, mmcs
*/

#ifndef __EVEPCH_H
#define __EVEPCH_H

#include <algorithm>

/* the 'common' ascent framework include */
#include "ascent.h"

/* 'evemu-server' generic includes */
#include "PyStringTable.h"
#include "EVEMarshalOpcodes.h"
#include "PyStream.h"
#include "MarshalReadStream.h"
#include "FileLogger.h"
#include "DBRow.h"

#include "PyObjects.h"
#include "MarshalStream.h"

#include "packet_types.h"
#include "EVEVersion.h"

#include "AccountMgr.h"
#include "EveSession.h"
#include "EveClientSocket.h"
#include "EveClientSession.h"

#include "Space.h"
#include "SpaceRunnable.h"
#include "RegionMapManager.h"

/* this one is evil.... */
//#include "../server/ClientSession.h"

/* test cases */
#include "TestCaseMarshal.h"

/* Database handles */
extern Database* Database_dynamic;
extern Database* Database_static;

/* database handle for static/read only data. */
#define StaticDatabase (*Database_static)

/* database handle for dynamic saving/loading server related data. */
#define DynamicDatabase (*Database_dynamic)

#endif//__EVEPCH_H
