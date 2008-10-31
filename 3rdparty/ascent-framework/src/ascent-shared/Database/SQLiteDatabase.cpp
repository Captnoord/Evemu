/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
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
#include "CrashHandler.h"
#include "NGLog.h"

#if defined(ENABLE_DATABASE_SQLITE) && !defined(NO_DBLAYER_SQL)

#include "SQLiteDatabase.h"
#pragma comment(lib, "sqlite3.lib")

SQLiteDatabase::~SQLiteDatabase()
{
	SafeDelete(mConnections[0]);
}

SQLiteDatabase::SQLiteDatabase() : Database()
{
}

void SQLiteDatabase::_BeginTransaction(DatabaseConnection * conn)
{
	_SendQuery( conn, "BEGIN TRANSACTION", false );
}

void SQLiteDatabase::_EndTransaction(DatabaseConnection * conn)
{
	//_SendQuery( conn, "COMMIT TRANSACTION", false );
	_SendQuery( conn, "COMMIT", false );
}

bool SQLiteDatabase::Initialize(const char* filename, unsigned int port, const char* Username, const char* Password, const char* DatabaseName, uint32 ConnectionCount, uint32 BufferSize)
{
	int result;
	SQLiteDatabaseConnection * con;

	mHostname = string(filename);
	
	// sqlite limitations?
	mConnectionCount = 1;	//mConnectionCount = ConnectionCount;
	mUsername = "";			//mUsername = string(Username);
	mPassword = "";			//mPassword = string(Password);

	Log.Notice("SQLiteDatabase", "Opening `%s`...", mHostname.c_str());
	mConnections = new DatabaseConnection*[mConnectionCount];
	mConnections[0] = new SQLiteDatabaseConnection;

	con = static_cast<SQLiteDatabaseConnection*>(mConnections[0]);
	
	result = sqlite3_open( mHostname.c_str(), &con->handle );
	if( con->handle == NULL || result != SQLITE_OK )
	{
		Log.Error("SQLiteDatabase", "Could not open %s.", mHostname.c_str());
		return false;
	}

	return true;
}

string SQLiteDatabase::EscapeString(string& Escape)
{
	char a2[DATABASE_QUERY_BUFFER_SIZE] = {0};
	sqlite3_snprintf( DATABASE_QUERY_BUFFER_SIZE, a2, Escape.c_str() );
	return string(a2);
}

void SQLiteDatabase::EscapeLongString(const char* str, uint32 len, stringstream& out)
{
	char a2[DATABASE_QUERY_LONG_BUFFER_SIZE] = {0};
	sqlite3_snprintf( DATABASE_QUERY_LONG_BUFFER_SIZE, a2, str );
	out.write(a2, (std::streamsize)strlen(a2));
}

string SQLiteDatabase::EscapeString(const char* esc, DatabaseConnection * con)
{
	char a2[DATABASE_QUERY_BUFFER_SIZE] = {0};
	sqlite3_snprintf( DATABASE_QUERY_BUFFER_SIZE, a2, esc );
	return string(a2);
}

void SQLiteDatabase::Shutdown()
{
	sqlite3_close( static_cast<SQLiteDatabaseConnection*>(*mConnections)->handle );
}

bool SQLiteDatabase::_SendQuery(DatabaseConnection* con, const char* Sql, bool Self)
{
	SQLiteDatabaseConnection * db = static_cast<SQLiteDatabaseConnection*>( con );

	char * err = NULL;
	int result = sqlite3_get_table( db->handle, Sql, &db->tabledata, &db->rows, &db->cols, &err );

	if( result != SQLITE_OK )
	{
		Log.Error("SQLiteDatabase", "Query failed with error message: %s", err ? err : "NULL");
		return false;
	}

	return true;
}

QueryResult * SQLiteDatabase::_StoreQueryResult(DatabaseConnection* con)
{
	SQLiteQueryResult * res;
	SQLiteDatabaseConnection * db = static_cast<SQLiteDatabaseConnection*>(con);
	
	// check if we have a valid db connection
	if( db == NULL )
		return NULL;

	// check if we actually have a result
	if ( db->cols == 0 || db->rows == 0 )
	{
		if ( db->tabledata != NULL )
			sqlite3_free_table(db->tabledata);
		return NULL;
	}

	res = new SQLiteQueryResult( db->tabledata, db->cols, db->rows );
	res->NextRow();

	// dirty hack.. to make sure we don't do this again...
	db->tabledata = NULL;

	return res;
}

bool SQLiteDatabase::SupportsReplaceInto()
{
	return false;
}

bool SQLiteDatabase::SupportsTableLocking()
{
	return false;
}

// TODO implement this..
bool SQLiteDatabase::_HandleError(DatabaseConnection* conn, uint32 ErrorNumber)
{
	return true;
}

// there isn't such thing as reconnect on sqlite
bool SQLiteDatabase::_Reconnect(DatabaseConnection* conn)
{
	return true;
}

SQLiteQueryResult::SQLiteQueryResult(char** res, uint32 FieldCount, uint32 RowCount) : QueryResult(FieldCount, RowCount), mResult( res ), mRow(0)
{
	mCurrentRow = new Field[FieldCount];
}

SQLiteQueryResult::~SQLiteQueryResult()
{
	sqlite3_free_table( mResult );
	SafeDeleteArray(mCurrentRow);
}

bool SQLiteQueryResult::NextRow()
{
	char * pValue;
	uint32 i;

	if( mRow == mRowCount )
		return false;

	for( i = 0; i < mFieldCount; ++i )
	{
		int index = i + mRow * mFieldCount;
		pValue = mResult[index];

		mCurrentRow[i].SetValue( pValue );
	}

	++mRow;
	return true;
}

#endif
