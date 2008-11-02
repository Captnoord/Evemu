/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2008 Ascent Team <http://www.ascentemu.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "Database/DatabaseEnv.h"
#include "Common.h"
#include "CrashHandler.h"
#include "NGLog.h"

#if defined(ENABLE_DATABASE_MYSQL)
#  pragma message("[DATABASE LAYER] MySql")
#  include "Database/MySQLDatabase.h"
#endif

#if defined(ENABLE_DATABASE_POSTGRES)
#  pragma message("[DATABASE LAYER] PostgreSql")
#  include "Database/PostgresDatabase.h"
#endif

#if defined(ENABLE_DATABASE_SQLITE)
#  pragma message("[DATABASE LAYER] Sqlite 3")
#  include "Database/SQLiteDatabase.h"
#endif

Database * Database::CreateDatabaseInterface(uint32 uType)
{
#if !defined(NO_DBLAYER_SQL)
	switch(uType)
	{
#if defined(ENABLE_DATABASE_MYSQL)
	case 1:		// MYSQL
		return new MySQLDatabase();
		break;
#endif//ENABLE_DATABASE_MYSQL
	
#if defined(ENABLE_DATABASE_POSTGRES)
	case 2:		// POSTGRES
		return new PostgresDatabase();
		break;
#endif//ENABLE_DATABASE_POSTGRES

#if defined(ENABLE_DATABASE_SQLITE)
	case 3:		// SQLITE
		return new SQLiteDatabase();
		break;
#endif//ENABLE_DATABASE_SQLITE
	}
#else
	uType = 0; // stupid warning fix
	#pragma message("[DATABASE LAYER] database system disabled")
#endif//NO_DBLAYER_SQL

	Log.LargeErrorMessage(LARGERRORMESSAGE_ERROR, "You have attempted to connect to a database that is unsupported or nonexistent.\nCheck your config and try again.");
	abort();
	return NULL;
}
