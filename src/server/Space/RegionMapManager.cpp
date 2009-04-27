/*
	------------------------------------------------------------------------------------
	LICENSE:
	------------------------------------------------------------------------------------
	This file is part of EVEmu: EVE Online Server Emulator
	Copyright 2006 - 2008 The EVEmu Team
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
	Author:		mmcs
*/

#include "EvemuPCH.h"
#define SPACE_REGION_UPDATE_DELAY 50

RegionMapMgr::RegionMapMgr(uint32 regionID,uint8 nrWorkers) :
m_regionID(regionID), m_workersSize(nrWorkers),m_isLoaded(false), m_solarsystemsSize(0), m_solarsystems(NULL)
{


}
	
RegionMapMgr::~RegionMapMgr()
{
	
	
}

/*void loadRegion_fromDB(Database *targetStaticDB, Database *targetDynamicDB)
{
	// TODO: create solars information and load kdtree from dynamic db Solar Objects cache.
	
	// region is loaded, then the manager can spawn the threads.
	m_isLoaded = true;
}*/

void RegionMapMgr::SpawnWorkerThreads()
{
	if(m_isLoaded)
	{
    	for(uint8 i = 0; i < m_workersSize; ++i)
        	ThreadPool.ExecuteTask(new RegionMapWorkerThread());
    }
}


RegionMapWorkerThread::RegionMapWorkerThread() : ThreadContext()
{


}

bool RegionMapWorkerThread::run()
{
	/*SetThreadName("RegionMapWorkerThread");

	Log.Notice("RegionMapWorkerThread","Booting...");

	uint32 LastSpaceUpdate = GetTickCount();
	uint32 LastSessionsUpdate = GetTickCount();

	//THREAD_TRY_EXECUTION2

	while(m_threadRunning == true)
	{
		uint32 diff;
		//calculate time passed
		uint32 now,execution_start;
		now = GetTickCount();
		execution_start = now;

		if( now < LastSpaceUpdate)//overrun
			diff = SPACE_REGION_UPDATE_DELAY;
		else
			diff = now - LastSpaceUpdate;
		
		LastSpaceUpdate = now;
		sSpace.Update( diff );
		
		now = GetTickCount();
		
		if( now < LastSessionsUpdate)//overrun
			diff = SPACE_REGION_UPDATE_DELAY;
		else
			diff = now-LastSessionsUpdate;
		
		LastSessionsUpdate = now;
		sSpace.UpdateSessions( diff );
		
		now = GetTickCount();
		//we have to wait now 
		
		if(execution_start > now)//overrun
			diff = SPACE_REGION_UPDATE_DELAY - now;
		else
			diff = now - execution_start;//time used for updating */

		/*This is execution time compensating system
			if execution took more than default delay 
			no need to make this sleep*/
	/*	if(diff < SPACE_REGION_UPDATE_DELAY)
			Sleep(SPACE_REGION_UPDATE_DELAY - diff);
	}

	//THREAD_HANDLE_CRASH2*/
	return true;
}
