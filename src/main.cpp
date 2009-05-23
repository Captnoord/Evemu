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

/* turtles VS unicorns */

#include "EvemuPCH.h"
#include "EVEVersion.h"
#include <signal.h>

static volatile bool m_stopEvent;
initialiseSingleton(FileLogger);

/* basic prototypes */
bool StartDatabase();
bool ShutdownDatabase();
void _HookSignals();
void _UnhookSignals();
static void _OnSignal(int s);

// Default main loop settings
#define ServerFrameworkMinute(x) (x * 60 * 1000)
#define ServerFrameworkDelay 50															// default server sleep 50ms
#define ServerThreadPoolDelay ServerFrameworkMinute(5)									// 5 minutes (5m * 60s * 1000ms)
#define ServerThreadPoolUpdateRelease (ServerThreadPoolDelay / ServerFrameworkDelay)	// 6000 times 50 ms = 5 minutes

#define ServerThreadPoolUpdateTest (ServerFrameworkMinute(1) / ServerFrameworkDelay)

#ifdef _DEBUG
#  define ServerThreadPoolUpdate ServerThreadPoolUpdateRelease
#else
#  define ServerThreadPoolUpdate ServerThreadPoolUpdateRelease
#endif//_DEBUG

#ifdef WIN32
static const char* default_config_file = "evemu-server.conf";
#else
static const char* default_config_file = "evemu-server.conf";
#endif

Database* Database_dynamic;
Database* Database_static;

int main(int argc, char *argv[])
{
	/* set the global time variable */
	UNIXTIME = time(NULL);
	g_localTime = *localtime(&UNIXTIME);

	printf("\nCopyright (C) 2006-2009 EVEmu Team. http://evemu.mmoforge.org\n");
	printf("\nThis program is free software; you can redistribute it and/or modify ");
	printf("it under the terms of the GNU General Public License as published by ");
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

	/* loading the config file */
	Log.Notice( "Config", "Loading Config Files...\n" );
	if( Config.SetSource( default_config_file ) )
		Log.Success( "Config", ">> evemu-server.conf" );
	else
	{
		Log.Error( "Config", ">> evemu-server.conf" );
		return false;
	}

	/* connect the db system to the db */
	if(!StartDatabase())
	{
		Log.Error("Database", "Unable to connect to the db.");
		return 0;
	}

	time_t curTime;

	uint32 realCurrTime, realPrevTime;
	realCurrTime = realPrevTime = GetTickCount();

	// Socket loop timing
	uint32 start;
	uint32 diff;
	uint32 last_time = GetTickCount();
	uint32 etime;
	uint32 next_printout = GetTickCount(), next_send = GetTickCount();

	/* server services */
	new Space;					/* not depending on anything atm. */
	new AccountMgr;				/* depending on the database system. */
	new ObjectCachingSvc;		/* depending on unknown. */
	new ConfigMgr;				/* depending on the database system. */
	
	/*if(!sSpace.LoadSpaceFormFromDB(Database_static))
	{
		Log.Error("Space", "failed to load space form, from Database.");
		return 1;
	}*/

	// runnable thread to update server systems and sessions
	SpaceRunnable * sr = new SpaceRunnable();
	ThreadPool.ExecuteTask(sr);

	new SocketMgr;
	new SocketGarbageCollector;
	
	sSocketMgr.SpawnWorkerThreads();

	/* listen settings */
	string host = Config.GetStringDefault( "Listen", "Host", "127.0.0.1" );
	int wsport = Config.GetIntDefault( "Listen", "ServerPort", 26000 );

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
	
	//sSpace.DeleteSpaceForm();
	ShutdownDatabase();

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
void OnCrash( bool Terminate ) {}
#endif

/* internal db start up wrapper... */
bool StartDatabase()
{
	std::string dynamic_hostname;
	std::string dynamic_username;
	std::string dynamic_password;
	std::string dynamic_db_name;

	std::string static_hostname;
	std::string static_username;
	std::string static_password;
	std::string static_db_name;

	int dynamic_port;
	int dynamic_connCount;
	int dynamic_type;
	
	int static_port;
	int static_connCount;
	int static_type;	

	bool result =				Config.GetString( "dynamic_database", "Username", &dynamic_username );
	result = !result ? result : Config.GetString( "dynamic_database", "Password", &dynamic_password );
	result = !result ? result : Config.GetString( "dynamic_database", "Hostname", &dynamic_hostname );
	result = !result ? result : Config.GetString( "dynamic_database", "Name", &dynamic_db_name );
	result = !result ? result : Config.GetInt( "dynamic_database", "Port", &dynamic_port );
	result = !result ? result : Config.GetInt( "dynamic_database", "Type", &dynamic_type );
	result = !result ? result : Config.GetInt( "dynamic_database", "ConnectionCount", &dynamic_connCount );

	result = !result ? result : Config.GetString( "static_database", "Username", &static_username );
	result = !result ? result : Config.GetString( "static_database", "Password", &static_password );
	result = !result ? result : Config.GetString( "static_database", "Hostname", &static_hostname );
	result = !result ? result : Config.GetString( "static_database", "Name", &static_db_name );
	result = !result ? result : Config.GetInt( "static_database", "Port", &static_port );
	result = !result ? result : Config.GetInt( "static_database", "Type", &static_type );
	result = !result ? result : Config.GetInt( "static_database", "ConnectionCount", &static_connCount );

	if (result == false)
	{
		Log.Error( "Database","something went wrong while reading the database settings from the config file. Exiting..." );
		return false;
	}

	// we use 2 database handles
	Database_static = Database::CreateDatabaseInterface(static_type);
	Database_dynamic = Database::CreateDatabaseInterface(dynamic_type);

	// Initialize it
	if( !StaticDatabase.Initialize(static_hostname.c_str(), static_port, static_username.c_str(),
		static_password.c_str(), static_db_name.c_str(), static_connCount, 16384 ) )
	{
		Log.Error( "Database","Static database initialization failed. Exiting..." );
		return false;
	}

	if( !DynamicDatabase.Initialize(dynamic_hostname.c_str(), dynamic_port, dynamic_username.c_str(),
		dynamic_password.c_str(), dynamic_db_name.c_str(), dynamic_connCount, 16384 ) )
	{
		Log.Error( "Database","Dynamic database initialization failed. Exiting..." );
		return false;
	}

	return true;
}

/* internal db shutdown wrapper... */
bool ShutdownDatabase()
{
	SafeDelete(Database_static);
	SafeDelete(Database_dynamic);
	return true;
}
