#ifndef __SQLITEDATABASE_H
#define __SQLITEDATABASE_H

#include <sqlite/sqlite3.h>

struct SQLiteDatabaseConnection : public DatabaseConnection
{
	sqlite3 * handle;

	char ** tabledata;
	int rows;
	int cols;
};

class SERVER_DECL SQLiteDatabase : public Database
{
	friend class QueryThread;
	friend class AsyncQuery;
public:
	SQLiteDatabase();
	~SQLiteDatabase();

	bool Initialize(const char* filename, unsigned int port,
		const char* Username, const char* Password, const char* DatabaseName,
		uint32 ConnectionCount, uint32 BufferSize);

	void Shutdown();

	string EscapeString(string& Escape);
	void EscapeLongString(const char * str, uint32 len, stringstream& out);
	string EscapeString(const char * esc, DatabaseConnection * con);

	bool SupportsReplaceInto();
	bool SupportsTableLocking();

	bool _HandleError(DatabaseConnection *conn, uint32 ErrorNumber);
	bool _Reconnect(DatabaseConnection *conn);

	
protected:

	bool _SendQuery(DatabaseConnection *con, const char* Sql, bool Self = false);

	void _BeginTransaction(DatabaseConnection * conn);
	void _EndTransaction(DatabaseConnection * conn);

	QueryResult * _StoreQueryResult(DatabaseConnection * con);



	string mConnectString;
};

class SERVER_DECL SQLiteQueryResult : public QueryResult
{
public:
	SQLiteQueryResult(char ** res, uint32 FieldCount, uint32 RowCount);
	~SQLiteQueryResult();

	bool NextRow();

protected:
	char ** mResult;
	uint32 mRow;
};

#endif		// __POSTGRESDATABASE_H
