#include "DatabaseEnv.h"
#include "Common.h"
#include "../CrashHandler.h"
#include "../NGLog.h"

#if defined(ENABLE_DATABASE_MYSQL)
#pragma message("[DATABASE LAYER] MySql")
#include "MySQLDatabase.h"
#endif

#if defined(ENABLE_DATABASE_POSTGRES)
#pragma message("[DATABASE LAYER] PostgreSql")
#include "PostgresDatabase.h"
#endif

#if defined(ENABLE_DATABASE_SQLITE)
#pragma message("[DATABASE LAYER] Sqlite 3")
#include "SQLiteDatabase.h"
#endif

Database * Database::CreateDatabaseInterface(uint32 uType)
{
	switch(uType)
	{
#if defined(ENABLE_DATABASE_MYSQL)
	case 1:		// MYSQL
		return new MySQLDatabase();
		break;
#endif
	
#if defined(ENABLE_DATABASE_POSTGRES)
	case 2:		// POSTGRES
		return new PostgresDatabase();
		break;
#endif

#if defined(ENABLE_DATABASE_SQLITE)
	case 3:		// SQLITE
		return new SQLiteDatabase();
		break;
#endif
	default:
#pragma message("[DATABASE LAYER] no database system selected")
		break;
	}

	Log.LargeErrorMessage(LARGERRORMESSAGE_ERROR, "You have attempted to connect to a database that is unsupported or nonexistant.\nCheck your config and try again.");
	abort();
	return NULL;
}
