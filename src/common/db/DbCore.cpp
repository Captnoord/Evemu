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
    Author:     Zhur
*/

#include "CommonPCH.h"

#include "db/DbCore.h"
#include "log/Log.h"
#include "utils/StrConv.h"

/************************************************************************/
/* DbError                                                              */
/************************************************************************/
DbError::DbError()
{
    ClearError();
}

void DbError::SetError( uint32 errNo, const char* errMsg )
{
    mErrNo = errNo;
    mErrMsg = errMsg;
}

void DbError::ClearError()
{
    SetError( 0, "No Error" );
}

/*************************************************************************/
/* DbResultRow                                                           */
/*************************************************************************/
DbResultRow::DbResultRow()
: mRow( NULL ),
  mLengths( NULL ),
  mResult( NULL )
{
}

size_t DbResultRow::count() const
{
    return mResult->columnCount();
}

const char* DbResultRow::name( size_t index ) const
{
    return mResult->columnName( index );
}

DBTYPE DbResultRow::type( size_t index ) const
{
    return mResult->columnType( index );
}

size_t DbResultRow::length( size_t index ) const
{
    assert( index < count() );
    return mLengths[ index ];
}

bool DbResultRow::isUnsigned( size_t index ) const
{
    return mResult->isColumnUnsigned( index );
}

bool DbResultRow::isBinary( size_t index ) const
{
    return mResult->isColumnBinary( index );
}

bool DbResultRow::isNull( size_t index ) const
{
    assert( index < count() );
    return NULL == mRow[ index ];
}

void DbResultRow::SetRow( DbQueryResult* res, const MYSQL_ROW& row, const unsigned long* lengths )
{
    mRow = row;
    mResult = res;
    mLengths = lengths;
}

/************************************************************************/
/* DbQueryResult                                                        */
/************************************************************************/
/* treating all strings as wide isn't probably the best solution but it's
   the easiest one which preserves wide strings. */
const DBTYPE DbQueryResult::MYSQL_DBTYPE_TABLE_SIGNED[] =
{
    DBTYPE_ERROR,   //[ 0]MYSQL_TYPE_DECIMAL            /* DECIMAL or NUMERIC field */
    DBTYPE_I1,      //[ 1]MYSQL_TYPE_TINY               /* TINYINT field */
    DBTYPE_I2,      //[ 2]MYSQL_TYPE_SHORT              /* SMALLINT field */
    DBTYPE_I4,      //[ 3]MYSQL_TYPE_LONG               /* INTEGER field */
    DBTYPE_R4,      //[ 4]MYSQL_TYPE_FLOAT              /* FLOAT field */
    DBTYPE_R8,      //[ 5]MYSQL_TYPE_DOUBLE             /* DOUBLE or REAL field */
    DBTYPE_ERROR,   //[ 6]MYSQL_TYPE_NULL               /* NULL-type field */
    DBTYPE_FILETIME,//[ 7]MYSQL_TYPE_TIMESTAMP          /* TIMESTAMP field */
    DBTYPE_I8,      //[ 8]MYSQL_TYPE_LONGLONG           /* BIGINT field */
    DBTYPE_I4,      //[ 9]MYSQL_TYPE_INT24              /* MEDIUMINT field */
    DBTYPE_ERROR,   //[10]MYSQL_TYPE_DATE               /* DATE field */
    DBTYPE_ERROR,   //[11]MYSQL_TYPE_TIME               /* TIME field */
    DBTYPE_ERROR,   //[12]MYSQL_TYPE_DATETIME           /* DATETIME field */
    DBTYPE_ERROR,   //[13]MYSQL_TYPE_YEAR               /* YEAR field */
    DBTYPE_ERROR,   //[14]MYSQL_TYPE_NEWDATE            /* ??? */
    DBTYPE_ERROR,   //[15]MYSQL_TYPE_VARCHAR            /* ??? */
    DBTYPE_BOOL,    //[16]MYSQL_TYPE_BIT                /* BIT field (MySQL 5.0.3 and up) */
    DBTYPE_ERROR,   //[17]MYSQL_TYPE_NEWDECIMAL=246     /* Precision math DECIMAL or NUMERIC field (MySQL 5.0.3 and up) */
    DBTYPE_ERROR,   //[18]MYSQL_TYPE_ENUM=247           /* ENUM field */
    DBTYPE_ERROR,   //[19]MYSQL_TYPE_SET=248            /* SET field */
    DBTYPE_WSTR,    //[20]MYSQL_TYPE_TINY_BLOB=249      /* TINYBLOB or TINYTEXT field */
    DBTYPE_WSTR,    //[21]MYSQL_TYPE_MEDIUM_BLOB=250    /* MEDIUMBLOB or MEDIUMTEXT field */
    DBTYPE_WSTR,    //[22]MYSQL_TYPE_LONG_BLOB=251      /* LONGBLOB or LONGTEXT field */
    DBTYPE_WSTR,    //[23]MYSQL_TYPE_BLOB=252           /* BLOB or TEXT field */
    DBTYPE_WSTR,    //[24]MYSQL_TYPE_VAR_STRING=253     /* VARCHAR or VARBINARY field */
    DBTYPE_WSTR,    //[25]MYSQL_TYPE_STRING=254         /* CHAR or BINARY field */
    DBTYPE_ERROR,   //[26]MYSQL_TYPE_GEOMETRY=255       /* Spatial field */
};

const DBTYPE DbQueryResult::MYSQL_DBTYPE_TABLE_UNSIGNED[] =
{
    DBTYPE_ERROR,   //[ 0]MYSQL_TYPE_DECIMAL            /* DECIMAL or NUMERIC field */
    DBTYPE_UI1,     //[ 1]MYSQL_TYPE_TINY               /* TINYINT field */
    DBTYPE_UI2,     //[ 2]MYSQL_TYPE_SHORT              /* SMALLINT field */
    DBTYPE_UI4,     //[ 3]MYSQL_TYPE_LONG               /* INTEGER field */
    DBTYPE_R4,      //[ 4]MYSQL_TYPE_FLOAT              /* FLOAT field */
    DBTYPE_R8,      //[ 5]MYSQL_TYPE_DOUBLE             /* DOUBLE or REAL field */
    DBTYPE_ERROR,   //[ 6]MYSQL_TYPE_NULL               /* NULL-type field */
    DBTYPE_FILETIME,//[ 7]MYSQL_TYPE_TIMESTAMP          /* TIMESTAMP field */
    DBTYPE_UI8,     //[ 8]MYSQL_TYPE_LONGLONG           /* BIGINT field */
    DBTYPE_UI4,     //[ 9]MYSQL_TYPE_INT24              /* MEDIUMINT field */
    DBTYPE_ERROR,   //[10]MYSQL_TYPE_DATE               /* DATE field */
    DBTYPE_ERROR,   //[11]MYSQL_TYPE_TIME               /* TIME field */
    DBTYPE_ERROR,   //[12]MYSQL_TYPE_DATETIME           /* DATETIME field */
    DBTYPE_ERROR,   //[13]MYSQL_TYPE_YEAR               /* YEAR field */
    DBTYPE_ERROR,   //[14]MYSQL_TYPE_NEWDATE            /* ??? */
    DBTYPE_ERROR,   //[15]MYSQL_TYPE_VARCHAR            /* ??? */
    DBTYPE_BOOL,    //[16]MYSQL_TYPE_BIT                /* BIT field (MySQL 5.0.3 and up) */
    DBTYPE_ERROR,   //[17]MYSQL_TYPE_NEWDECIMAL=246     /* Precision math DECIMAL or NUMERIC field (MySQL 5.0.3 and up) */
    DBTYPE_ERROR,   //[18]MYSQL_TYPE_ENUM=247           /* ENUM field */
    DBTYPE_ERROR,   //[19]MYSQL_TYPE_SET=248            /* SET field */
    DBTYPE_WSTR,    //[20]MYSQL_TYPE_TINY_BLOB=249      /* TINYBLOB or TINYTEXT field */
    DBTYPE_WSTR,    //[21]MYSQL_TYPE_MEDIUM_BLOB=250    /* MEDIUMBLOB or MEDIUMTEXT field */
    DBTYPE_WSTR,    //[22]MYSQL_TYPE_LONG_BLOB=251      /* LONGBLOB or LONGTEXT field */
    DBTYPE_WSTR,    //[23]MYSQL_TYPE_BLOB=252           /* BLOB or TEXT field */
    DBTYPE_WSTR,    //[24]MYSQL_TYPE_VAR_STRING=253     /* VARCHAR or VARBINARY field */
    DBTYPE_WSTR,    //[25]MYSQL_TYPE_STRING=254         /* CHAR or BINARY field */
    DBTYPE_ERROR,   //[26]MYSQL_TYPE_GEOMETRY=255       /* Spatial field */
};

DbQueryResult::DbQueryResult()
: mColumnCount( 0 ),
  mResult( NULL ),
  mFields( NULL )
{
}

DbQueryResult::~DbQueryResult()
{
    SafeDeleteArray( mFields );

    if( NULL != mResult )
        mysql_free_result( mResult );
}

const char* DbQueryResult::columnName( size_t index ) const
{
    assert( index < columnCount() );
    return mFields[ index ]->name;
}

DBTYPE DbQueryResult::columnType( size_t index ) const
{
    assert( index < columnCount() );
    uint32 columnType = mFields[ index ]->type;

    /* tricky needs to be checked */
    if ( columnType > MYSQL_TYPE_BIT )
        columnType -= ( MYSQL_TYPE_NEWDECIMAL - MYSQL_TYPE_BIT - 1 );

    DBTYPE result = ( isColumnUnsigned( index ) ? MYSQL_DBTYPE_TABLE_UNSIGNED : MYSQL_DBTYPE_TABLE_SIGNED )[ columnType ];

    /* if result is (wide) binary string, set result to DBTYPE_BYTES. */
    if( ( DBTYPE_STR == result
          || DBTYPE_WSTR == result )
        && isColumnBinary( index ) )
    {
        result = DBTYPE_BYTES;
    }

    /* debug check */
    assert( DBTYPE_ERROR != result );

    return result;
}

bool DbQueryResult::isColumnUnsigned( size_t index ) const
{
    assert( index < columnCount() );
    return 0 != ( mFields[index]->flags & UNSIGNED_FLAG );
}

bool DbQueryResult::isColumnBinary( size_t index ) const
{
    assert( index < columnCount() );
    // According to MySQL C API Documentation, binary string
    // fields like BLOB or VAR_BINARY have charset "63".
    return 63 == mFields[ index ]->charsetnr;
}

bool DbQueryResult::GetRow( DbResultRow& into )
{
    if( NULL == mResult )
        return false;

    MYSQL_ROW row = mysql_fetch_row( mResult );
    if( NULL == row )
        return false;

    const unsigned long* lengths = mysql_fetch_lengths( mResult );
    if( NULL == lengths )
        return false;

    into.SetRow( this, row, lengths );
    return true;
}

void DbQueryResult::Reset()
{
    if( NULL != mResult )
        mysql_data_seek( mResult, 0);
}

void DbQueryResult::SetResult( MYSQL_RES** res, size_t colCount )
{
    SafeDeleteArray( mFields );

    if( NULL != mResult )
        mysql_free_result( mResult );

    mResult = *res;
    *res = NULL;
    mColumnCount = colCount;

    if( NULL != mResult )
    {
        mFields = new MYSQL_FIELD*[ columnCount() ];

        // we are
        for( size_t i = 0; i < columnCount(); ++i )
            mFields[ i ] = mysql_fetch_field( mResult );
    }
}

/*************************************************************************/
/* DbCore                                                                */
/*************************************************************************/
DbCore::DbCore(bool compress, bool ssl) : pCompress(compress), pSSL(ssl)
{
    mysql_init(&mysql);
    pStatus = Closed;
}

DbCore::~DbCore()
{
    mysql_close(&mysql);
}

// Sends the MySQL server a ping
void DbCore::ping()
{
    // well, if it's locked, someone's using it. If someone's using it, it doesn't need a ping
    if( MDatabase.TryLock() )
    {
        mysql_ping( &mysql );
        MDatabase.Unlock();
    }
}

//query which returns a result (error is stored in the result if it occurs)
bool DbCore::RunQuery(DbQueryResult &into, const char *query_fmt, ...) {
    MutexLock lock(MDatabase);

    char query[16384];
    va_list vlist;
    va_start(vlist, query_fmt);
    uint32 querylen = vsnprintf(query, 16384, query_fmt, vlist);
    va_end(vlist);

    if(!DoQuery_locked(into.error, query, querylen)) {
        return false;
    }

    uint32 col_count = mysql_field_count(&mysql);
    if(col_count == 0) {
        into.error.SetError(0xFFFF, "DbCore::RunQuery: No Result");
        sLog.Error("DbCore Query", "Query: %s failed because did not return a result", query);
        return false;
    }

    MYSQL_RES *result = mysql_store_result(&mysql);

    //give them the result set.
    into.SetResult(&result, col_count);

    return true;
}

//query which returns no information except error status
bool DbCore::RunQuery(DbError &err, const char *query_fmt, ...) {
    MutexLock lock(MDatabase);

    va_list args;
    va_start(args, query_fmt);
    char *query = NULL;
    uint32 querylen = vasprintf(&query, query_fmt, args);
    va_end(args);

    if(!DoQuery_locked(err, query, querylen)) {
        free(query);
        return false;
    }

    free(query);
    return true;
}

//query which returns affected rows:
bool DbCore::RunQuery(DbError &err, uint32 &affected_rows, const char *query_fmt, ...) {
    MutexLock lock(MDatabase);

    va_list args;
    va_start(args, query_fmt);
    char *query = NULL;
    uint32 querylen = vasprintf(&query, query_fmt, args);
    va_end(args);

    if(!DoQuery_locked(err, query, querylen)) {
        free(query);
        return false;
    }
    free(query);

    affected_rows = mysql_affected_rows(&mysql);

    return true;
}

//query which returns last insert ID:
bool DbCore::RunQueryLID(DbError &err, uint32 &last_insert_id, const char *query_fmt, ...) {
    MutexLock lock(MDatabase);

    va_list args;
    va_start(args, query_fmt);
    char *query = NULL;
    uint32 querylen = vasprintf(&query, query_fmt, args);
    va_end(args);

    if(!DoQuery_locked(err, query, querylen)) {
        free(query);
        return false;
    }
    free(query);

    last_insert_id = mysql_insert_id(&mysql);

    return true;
}

bool DbCore::DoQuery_locked(DbError &err, const char *query, int32 querylen, bool retry)
{
    if (pStatus != Connected)
        Open_locked();

    if (mysql_real_query(&mysql, query, querylen)) {
        int num = mysql_errno(&mysql);

        if (num == CR_SERVER_GONE_ERROR)
            pStatus = Error;

        if (retry && (num == CR_SERVER_LOST || num == CR_SERVER_GONE_ERROR))
        {
            sLog.Error("DbCore", "Lost connection, attempting to recover....");
            return DoQuery_locked(err, query, querylen, false);
        }

        pStatus = Error;
        err.SetError(num, mysql_error(&mysql));
        sLog.Error("DbCore Query", "#%d in '%s': %s", err.GetErrNo(), query, err.c_str());
        return false;
    }

    err.ClearError();
    return true;
}


bool DbCore::RunQuery(const char* query, int32 querylen, char* errbuf, MYSQL_RES** result, int32* affected_rows, int32* last_insert_id, int32* errnum, bool retry) {
    if (errnum)
        *errnum = 0;
    if (errbuf)
        errbuf[0] = 0;
    MutexLock lock(MDatabase);

    DbError err;
    if(!DoQuery_locked(err, query, querylen, retry))
    {
        sLog.Error("DbCore Query", "Query: %s failed", query);
        if(errnum != NULL)
            *errnum = err.GetErrNo();
        if(errbuf != NULL)
            strcpy(errbuf, err.c_str());
        return false;
    }

    if (result) {
        if(mysql_field_count(&mysql)) {
            *result = mysql_store_result(&mysql);
        } else {
            *result = NULL;
            if (errnum)
                *errnum = UINT_MAX;
            if (errbuf)
                strcpy(errbuf, "DbCore::RunQuery: No Result");
            sLog.Error("DbCore Query", "Query: %s failed because it should return a result", query);
            return false;
        }
    }
    if (affected_rows)
        *affected_rows = mysql_affected_rows(&mysql);
    if (last_insert_id)
        *last_insert_id = mysql_insert_id(&mysql);
    return true;
}

int32 DbCore::DoEscapeString(char* tobuf, const char* frombuf, int32 fromlen)
{
    return mysql_real_escape_string(&mysql, tobuf, frombuf, fromlen);
}

void DbCore::DoEscapeString(std::string &to, const std::string &from)
{
    uint32 len = (uint32)from.length();
    to.resize(len*2 + 1);   // make enough room
    uint32 esc_len = mysql_real_escape_string(&mysql, &to[0], from.c_str(), len);
    to.resize(esc_len+1); // optional.
}

//look for things in the string which might cause SQL problems
bool DbCore::IsSafeString(const char *str) {
    for(; *str != '\0'; str++) {
        switch(*str) {
        case '\'':
        case '\\':
            return false;
        }
    }
    return true;
}

bool DbCore::Open(const char* iHost, const char* iUser, const char* iPassword, const char* iDatabase, int16 iPort, int32* errnum, char* errbuf, bool iCompress, bool iSSL) {
    MutexLock lock(MDatabase);

    pHost = iHost;
    pUser = iUser;
    pPassword = iPassword;
    pDatabase = iDatabase;
    pCompress = iCompress;
    pPort = iPort;
    pSSL = iSSL;

    return Open_locked(errnum, errbuf);
}

bool DbCore::Open(DbError &err, const char* iHost, const char* iUser, const char* iPassword, const char* iDatabase, int16 iPort, bool iCompress, bool iSSL) {
    MutexLock lock(MDatabase);

    pHost = iHost;
    pUser = iUser;
    pPassword = iPassword;
    pDatabase = iDatabase;
    pCompress = iCompress;
    pPort = iPort;
    pSSL = iSSL;

    int32 errnum;
    char errbuf[1024];

    if(!Open_locked(&errnum, errbuf)) {
        err.SetError(errnum, errbuf);
        return false;
    }

    return true;
}


bool DbCore::Open_locked(int32* errnum, char* errbuf) {
    if (errbuf)
        errbuf[0] = 0;
    if (GetStatus() == Connected)
        return true;
    if (GetStatus() == Error)
        mysql_close(&mysql);    //do we need to call init again?
    if (pHost.empty())
        return false;

    sLog.Message( "dbcore",
                  "Connecting to\n"
                  "    DB:     %s\n"
                  "    server: %s:%d\n"
                  "    user:   %s",
                  pDatabase.c_str(),
                  pHost.c_str(), pPort,
                  pUser.c_str() );

    /*
    Quagmire - added CLIENT_FOUND_ROWS flag to the connect
    otherwise DB update calls would say 0 rows affected when the value already equaled
    what the function was trying to set it to, therefore the function would think it failed
    */
    int32 flags = CLIENT_FOUND_ROWS;
    if (pCompress)
        flags |= CLIENT_COMPRESS;
    if (pSSL)
        flags |= CLIENT_SSL;
    if (mysql_real_connect(&mysql, pHost.c_str(), pUser.c_str(), pPassword.c_str(), pDatabase.c_str(), pPort, 0, flags)) {
        pStatus = Connected;
    } else {
        pStatus = Error;
        if (errnum)
            *errnum = mysql_errno(&mysql);
        if (errbuf)
            snprintf(errbuf, MYSQL_ERRMSG_SIZE, "#%i: %s", mysql_errno(&mysql), mysql_error(&mysql));
        return false;
    }

    // Setup character set we wish to use
    if(mysql_set_character_set(&mysql, "utf8") != 0) {
        pStatus = Error;
        if(errnum)
            *errnum = mysql_errno(&mysql);
        if(errbuf)
            snprintf(errbuf, MYSQL_ERRMSG_SIZE, "#%i: %s", mysql_errno(&mysql), mysql_error(&mysql));
        return false;
    }

    return true;
}
