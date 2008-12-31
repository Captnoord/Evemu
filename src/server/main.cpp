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
	Author:		zhur
*/


#include "EvemuPCH.h"

#include <signal.h>

static volatile bool RunLoops = true;

int main(int argc, char *argv[])
{
	printf("\nCopyright (C) 2006-2008 EVEmu Team. http://evemu.mmoforge.org\n");
	printf("\nThis program is free software; you can redistribute it and/or modify");
	printf("it under the terms of the GNU General Public License as published by");
	printf("the Free Software Foundation; version 2 of the License.");
	printf("For a copy of this license, see the COPYING file provided with this distribution.\n");
	Log.Line();
	
	//sLog.String("Revision: %s", EVEMU_REVISION);
	sLog.String("Revision: %h", 666);
	sLog.String("Supported Client: %s, Version %.2f, Build %d, MachoNet %d", EveProjectVersion, EveVersionNumber, EveBuildVersion, MachoNetVersion);

	printf( "The key combination <Ctrl-C> will safely shut down the server at any time.\n" );
	Log.Line();

#ifndef WIN32
	if(geteuid() == 0 || getegid() == 0)
	{
		Log.LargeErrorMessage( LARGERRORMESSAGE_WARNING, "You are running Evemu as root.", "This is not needed, and may be a possible security risk.", "It is advised to hit CTRL+C now and", "start as a non-privileged user.", NULL);
	}
#endif//WIN32

	new FileLogger("packet_log.txt");
	sFileLogger.enable();// developer warning....:D

	/* start the 'threadpool', this needs to be in front of things that uses the 'threadpool' */
	ThreadPool.Startup();

	// Startup banner
	UNIXTIME = time(NULL);
	g_localTime = *localtime(&UNIXTIME);

	time_t curTime;

	uint32 realCurrTime, realPrevTime;
	realCurrTime = realPrevTime = getMSTime();

	// Socket loop timing
	uint32 start;
	uint32 diff;
	uint32 last_time = now();
	uint32 etime;
	uint32 next_printout = getMSTime(), next_send = getMSTime();

	new Space;

	// runnable thread to update server systems and sessions
	SpaceRunnable * sr = new SpaceRunnable();
	ThreadPool.ExecuteTask(sr);

	new SocketMgr;
	new SocketGarbageCollector;
	
	sSocketMgr.SpawnWorkerThreads();

	static volatile bool m_stopEvent;

	/* these settings are 'hardcoded' for now */
	uint32 wsport = 26000;
	std::string host = "127.0.0.1";

#ifdef WIN32
	HANDLE hThread = GetCurrentThread();
#endif

	uint32 loopcounter = 0;

	// Create listener
	ListenSocket<EveClientSocket> * ls = new ListenSocket<EveClientSocket>(host.c_str(), wsport);
	bool listnersockcreate = ls->IsOpen();
#ifdef WIN32
	if( listnersockcreate == true )
		ThreadPool.ExecuteTask(ls);
#endif

	sLog.Basic("Server startup [done]");

	/* main thread loop */
	while( !m_stopEvent && listnersockcreate )
	{
		start = now();
		diff = start - last_time;

		if(! ((++loopcounter) % 10000) )		// 5mins
		{
			ThreadPool.ShowStats();
			ThreadPool.IntegrityCheck();
		}

		/* since time() is an expensive system call, we only update it once per server loop */
		curTime = time(NULL);
		if( UNIXTIME != curTime )
		{
			UNIXTIME = curTime;
			g_localTime = *localtime(&curTime);
		}

		// check for garbage sockets
		sSocketGarbageCollector.Update();

		// temp solution for updating services
		//services.Process();

		/* UPDATE */
		last_time = now();
		etime = last_time - start;

		// do the stuff for thread sleeping
		if( 50 > etime )
		{
#ifdef WIN32
			WaitForSingleObject( hThread, 50 - etime );
#else
			Sleep( 50 - etime );
#endif
		}
	}
	return 1;
}
