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
	Author:		zhur, Captnoord, mmcs
*/

#include "EvemuPCH.h"
#include "EVEVersion.h"
#include <signal.h>

//#include "TestCaseMain.h"
//#include "TestCaseMarshal.h"

static volatile bool m_stopEvent;
initialiseSingleton(FileLogger);

/* basic prototypes */
bool _DBStartup(std::string _hostname, uint32 _port, std::string _username, std::string _password, std::string _database, std::string _databaseDynamic);
bool _DBShutdown();
static void _OnSignal(int s);
void _HookSignals();
void _UnhookSignals();

// Default main loop settings
#define ServerFrameworkMinute(x) (x * 60 * 1000)
#define ServerFrameworkDelay 50															// default server sleep 50ms
#define ServerThreadPoolDelay ServerFrameworkMinute(5)									// 5 minutes (5m * 60s * 1000ms)
#define ServerThreadPoolUpdateRelease (ServerThreadPoolDelay / ServerFrameworkDelay)	// 6000 times 50 ms = 5 minutes

#define ServerThreadPoolUpdateTest (ServerFrameworkMinute(1) / ServerFrameworkDelay)

#ifdef _DEBUG
#  define ServerThreadPoolUpdate ServerThreadPoolUpdateTest
#else
#  define ServerThreadPoolUpdate ServerThreadPoolUpdateRelease
#endif//_DEBUG

Database* Database_dynamic;
Database* Database_static;

int main(int argc, char *argv[])
{
	printf("\nCopyright (C) 2006-2009 EVEmu Team. http://evemu.mmoforge.org\n");
	printf("\nThis program is free software; you can redistribute it and/or modify");
	printf("it under the terms of the GNU General Public License as published by");
	printf("the Free Software Foundation; version 2 of the License.");
	printf("For a copy of this license, see the COPYING file provided with this distribution.\n");
	Log.Line();
	
	//sLog.String("Revision: %s", EVEMU_REVISION);
	sLog.String("Revision: %u", 666);
	sLog.String("Supported Client: %s, Version %.2f, Build %d, MachoNet %d", EveProjectVersion, EveVersionNumber, EveBuildVersion, MachoNetVersion);

	printf( "The key combination <Ctrl-C> will safely shut down the server at any time.\n" );
	Log.Line();

#ifndef WIN32
	if(geteuid() == 0 || getegid() == 0)
	{
		Log.LargeErrorMessage( LARGERRORMESSAGE_WARNING, "You are running evemu-server as root.", "This is not needed, and may be a possible security risk.", "It is advised to hit CTRL+C now and", "start as a non-privileged user.", NULL);
	}
#endif//WIN32

	new FileLogger("packet_log.txt");
	sFileLogger.enable();// developer warning....:D

	/* start the 'threadpool', this needs to be in front of things that uses the 'threadpool' */
	ThreadPool.Startup();

	/* connect the db system to the db */
	bool dbret = _DBStartup("localhost",3306, "padawa", "theforce", "evemu_rewrite_static", "evemu_rewrite_dynamic");
	if(dbret == false)
	{
		Log.Error("Database", "Unable to connect to the db");
		return 0;
	}

	// Startup banner
	UNIXTIME = time(NULL);
	g_localTime = *localtime(&UNIXTIME);

	time_t curTime;

	uint32 realCurrTime, realPrevTime;
	realCurrTime = realPrevTime = GetTickCount();

	// Socket loop timing
	uint32 start;
	uint32 diff;
	uint32 last_time = GetTickCount();
	uint32 etime;
	uint32 next_printout = GetTickCount(), next_send = GetTickCount();

	new Space;
	new AccountMgr;

	// runnable thread to update server systems and sessions
	SpaceRunnable * sr = new SpaceRunnable();
	ThreadPool.ExecuteTask(sr);

	new SocketMgr;
	new SocketGarbageCollector;
	
	sSocketMgr.SpawnWorkerThreads();

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
		start = GetTickCount();
		diff = start - last_time;

		if(! ((++loopcounter) % ServerThreadPoolUpdate) )		// 5mins
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
		last_time = GetTickCount();
		etime = last_time - start;

		// do the stuff for thread sleeping
		if( ServerFrameworkDelay > etime )
		{
#ifdef WIN32
			WaitForSingleObject( hThread, ServerFrameworkDelay - etime );
#else
			Sleep( ServerFrameworkDelay - etime );
#endif
		}
	}

	_DBShutdown();

	return 1;
}

void _OnSignal(int s)
{
	switch (s)
	{
#ifndef WIN32
	case SIGHUP:
		
		break;
#endif
	case SIGINT:
	case SIGTERM:
	case SIGABRT:
#ifdef _WIN32
	case SIGBREAK:
#endif
		m_stopEvent = true;
		break;
	}

	signal(s, _OnSignal);
}

void _HookSignals()
{
	signal( SIGINT, _OnSignal );
	signal( SIGTERM, _OnSignal );
	signal( SIGABRT, _OnSignal );
#ifdef _WIN32
	signal( SIGBREAK, _OnSignal );
#else
	signal( SIGHUP, _OnSignal );
	signal( SIGUSR1, _OnSignal);
#endif
}

void _UnhookSignals()
{
	signal( SIGINT, 0 );
	signal( SIGTERM, 0 );
	signal( SIGABRT, 0 );
#ifdef _WIN32
	signal( SIGBREAK, 0 );
#else
	signal( SIGHUP, 0 );
#endif

}

#ifdef WIN32

Mutex m_crashedMutex;

// Crash Handler
void OnCrash( bool Terminate )
{
}

#endif

/* internal db start up wrapper... */
bool _DBStartup(std::string _hostname, uint32 _port, std::string _username, std::string _password, std::string _databaseStatic, std::string _databaseDynamic)
{
	std::string hostname = _hostname;
	std::string username = _username;
	std::string password = _password;
	std::string staticdb = _databaseStatic;
	std::string dynamicdb = _databaseDynamic;

	uint32 port = _port;
	int type = 1; // 1 is mysql
	int connectionCount = 3; // make this configurable

	// we use 2 database handles
	Database_static = Database::CreateDatabaseInterface(type);
	Database_dynamic = Database::CreateDatabaseInterface(type);

	// Initialize it
	if( !StaticDatabase.Initialize(hostname.c_str(), port, username.c_str(),
		password.c_str(), staticdb.c_str(), connectionCount, 16384 ) )
	{
		Log.Error( "Database","Static database initialization failed. Exiting..." );
		return false;
	}

	if( !DynamicDatabase.Initialize(hostname.c_str(), port, username.c_str(),
		password.c_str(), dynamicdb.c_str(), connectionCount, 16384 ) )
	{
		Log.Error( "Database","Dynamic database initialization failed. Exiting..." );
		return false;
	}

	return true;
}

/* internal db shutdown wrapper... */
bool _DBShutdown()
{
	SafeDelete(Database_static);
	SafeDelete(Database_dynamic);
	return true;
}
