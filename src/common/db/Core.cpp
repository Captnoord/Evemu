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

#include "db/Core.h"
#include "utils/Log.h"
#include "utils/StrConv.h"

/************************************************************************/
/* Db::Error                                                            */
/************************************************************************/
Db::Error::Error()
{
    ClearError();
}

void Db::Error::SetError( unsigned int errNo, const char* errMsg )
{
    mErrNo = errNo;
    mErrMsg = errMsg;
}

void Db::Error::ClearError()
{
    SetError( 0, "No Error" );
}

/*************************************************************************/
/* Db::ResultRow                                                         */
/*************************************************************************/
Db::ResultRow::ResultRow()
: mRow( NULL ),
  mLengths( NULL ),
  mResult( NULL )
{
}

size_t Db::ResultRow::count() const
{
    return mResult->columnCount();
}

const char* Db::ResultRow::name( size_t index ) const
{
    return mResult->columnName( index );
}

Db::TYPE Db::ResultRow::type( size_t index ) const
{
    return mResult->columnType( index );
}

size_t Db::ResultRow::length( size_t index ) const
{
    assert( index < count() );
    return mLengths[ index ];
}

bool Db::ResultRow::isUnsigned( size_t index ) const
{
    return mResult->isColumnUnsigned( index );
}

bool Db::ResultRow::isBinary( size_t index ) const
{
    return mResult->isColumnBinary( index );
}

bool Db::ResultRow::isNull( size_t index ) const
{
    assert( index < count() );
    return NULL == mRow[ index ];
}

void Db::ResultRow::SetRow( Db::QueryResult* res, const MYSQL_ROW& row, const unsigned long* lengths )
{
    mRow = row;
    mResult = res;
    mLengths = lengths;
}

/************************************************************************/
/* Db::QueryResult                                                      */
/************************************************************************/
/* treating all strings as wide isn't probably the best solution but it's
   the easiest one which preserves wide strings. */
const Db::TYPE Db::QueryResult::MYSQL_TYPE_TABLE_SIGNED[] =
{
    Db::TYPE_ERROR,   //[ 0]MYSQL_TYPE_DECIMAL            /* DECIMAL or NUMERIC field */
    Db::TYPE_I1,      //[ 1]MYSQL_TYPE_TINY               /* TINYINT field */
    Db::TYPE_I2,      //[ 2]MYSQL_TYPE_SHORT              /* SMALLINT field */
    Db::TYPE_I4,      //[ 3]MYSQL_TYPE_LONG               /* INTEGER field */
    Db::TYPE_R4,      //[ 4]MYSQL_TYPE_FLOAT              /* FLOAT field */
    Db::TYPE_R8,      //[ 5]MYSQL_TYPE_DOUBLE             /* DOUBLE or REAL field */
    Db::TYPE_ERROR,   //[ 6]MYSQL_TYPE_NULL               /* NULL-type field */
    Db::TYPE_FILETIME,//[ 7]MYSQL_TYPE_TIMESTAMP          /* TIMESTAMP field */
    Db::TYPE_I8,      //[ 8]MYSQL_TYPE_LONGLONG           /* BIGINT field */
    Db::TYPE_I4,      //[ 9]MYSQL_TYPE_INT24              /* MEDIUMINT field */
    Db::TYPE_ERROR,   //[10]MYSQL_TYPE_DATE               /* DATE field */
    Db::TYPE_ERROR,   //[11]MYSQL_TYPE_TIME               /* TIME field */
    Db::TYPE_ERROR,   //[12]MYSQL_TYPE_DATETIME           /* DATETIME field */
    Db::TYPE_ERROR,   //[13]MYSQL_TYPE_YEAR               /* YEAR field */
    Db::TYPE_ERROR,   //[14]MYSQL_TYPE_NEWDATE            /* ??? */
    Db::TYPE_ERROR,   //[15]MYSQL_TYPE_VARCHAR            /* ??? */
    Db::TYPE_BOOL,    //[16]MYSQL_TYPE_BIT                /* BIT field (MySQL 5.0.3 and up) */
    Db::TYPE_ERROR,   //[17]MYSQL_TYPE_NEWDECIMAL=246     /* Precision math DECIMAL or NUMERIC field (MySQL 5.0.3 and up) */
    Db::TYPE_ERROR,   //[18]MYSQL_TYPE_ENUM=247           /* ENUM field */
    Db::TYPE_ERROR,   //[19]MYSQL_TYPE_SET=248            /* SET field */
    Db::TYPE_WSTR,    //[20]MYSQL_TYPE_TINY_BLOB=249      /* TINYBLOB or TINYTEXT field */
    Db::TYPE_WSTR,    //[21]MYSQL_TYPE_MEDIUM_BLOB=250    /* MEDIUMBLOB or MEDIUMTEXT field */
    Db::TYPE_WSTR,    //[22]MYSQL_TYPE_LONG_BLOB=251      /* LONGBLOB or LONGTEXT field */
    Db::TYPE_WSTR,    //[23]MYSQL_TYPE_BLOB=252           /* BLOB or TEXT field */
    Db::TYPE_WSTR,    //[24]MYSQL_TYPE_VAR_STRING=253     /* VARCHAR or VARBINARY field */
    Db::TYPE_WSTR,    //[25]MYSQL_TYPE_STRING=254         /* CHAR or BINARY field */
    Db::TYPE_ERROR,   //[26]MYSQL_TYPE_GEOMETRY=255       /* Spatial field */
};

const Db::TYPE Db::QueryResult::MYSQL_TYPE_TABLE_UNSIGNED[] =
{
    Db::TYPE_ERROR,   //[ 0]MYSQL_TYPE_DECIMAL            /* DECIMAL or NUMERIC field */
    Db::TYPE_UI1,     //[ 1]MYSQL_TYPE_TINY               /* TINYINT field */
    Db::TYPE_UI2,     //[ 2]MYSQL_TYPE_SHORT              /* SMALLINT field */
    Db::TYPE_UI4,     //[ 3]MYSQL_TYPE_LONG               /* INTEGER field */
    Db::TYPE_R4,      //[ 4]MYSQL_TYPE_FLOAT              /* FLOAT field */
    Db::TYPE_R8,      //[ 5]MYSQL_TYPE_DOUBLE             /* DOUBLE or REAL field */
    Db::TYPE_ERROR,   //[ 6]MYSQL_TYPE_NULL               /* NULL-type field */
    Db::TYPE_FILETIME,//[ 7]MYSQL_TYPE_TIMESTAMP          /* TIMESTAMP field */
    Db::TYPE_UI8,     //[ 8]MYSQL_TYPE_LONGLONG           /* BIGINT field */
    Db::TYPE_UI4,     //[ 9]MYSQL_TYPE_INT24              /* MEDIUMINT field */
    Db::TYPE_ERROR,   //[10]MYSQL_TYPE_DATE               /* DATE field */
    Db::TYPE_ERROR,   //[11]MYSQL_TYPE_TIME               /* TIME field */
    Db::TYPE_ERROR,   //[12]MYSQL_TYPE_DATETIME           /* DATETIME field */
    Db::TYPE_ERROR,   //[13]MYSQL_TYPE_YEAR               /* YEAR field */
    Db::TYPE_ERROR,   //[14]MYSQL_TYPE_NEWDATE            /* ??? */
    Db::TYPE_ERROR,   //[15]MYSQL_TYPE_VARCHAR            /* ??? */
    Db::TYPE_BOOL,    //[16]MYSQL_TYPE_BIT                /* BIT field (MySQL 5.0.3 and up) */
    Db::TYPE_ERROR,   //[17]MYSQL_TYPE_NEWDECIMAL=246     /* Precision math DECIMAL or NUMERIC field (MySQL 5.0.3 and up) */
    Db::TYPE_ERROR,   //[18]MYSQL_TYPE_ENUM=247           /* ENUM field */
    Db::TYPE_ERROR,   //[19]MYSQL_TYPE_SET=248            /* SET field */
    Db::TYPE_WSTR,    //[20]MYSQL_TYPE_TINY_BLOB=249      /* TINYBLOB or TINYTEXT field */
    Db::TYPE_WSTR,    //[21]MYSQL_TYPE_MEDIUM_BLOB=250    /* MEDIUMBLOB or MEDIUMTEXT field */
    Db::TYPE_WSTR,    //[22]MYSQL_TYPE_LONG_BLOB=251      /* LONGBLOB or LONGTEXT field */
    Db::TYPE_WSTR,    //[23]MYSQL_TYPE_BLOB=252           /* BLOB or TEXT field */
    Db::TYPE_WSTR,    //[24]MYSQL_TYPE_VAR_STRING=253     /* VARCHAR or VARBINARY field */
    Db::TYPE_WSTR,    //[25]MYSQL_TYPE_STRING=254         /* CHAR or BINARY field */
    Db::TYPE_ERROR,   //[26]MYSQL_TYPE_GEOMETRY=255       /* Spatial field */
};

Db::QueryResult::QueryResult()
: mColumnCount( 0 ),
  mResult( NULL ),
  mFields( NULL )
{
}

Db::QueryResult::~QueryResult()
{
    SafeDeleteArray( mFields );

    if( NULL != mResult )
        mysql_free_result( mResult );
}

const char* Db::QueryResult::columnName( size_t index ) const
{
    assert( index < columnCount() );
    return mFields[ index ]->name;
}

Db::TYPE Db::QueryResult::columnType( size_t index ) const
{
    assert( index < columnCount() );
    unsigned int columnType = mFields[ index ]->type;

    /* tricky needs to be checked */
    if ( columnType > MYSQL_TYPE_BIT )
        columnType -= ( MYSQL_TYPE_NEWDECIMAL - MYSQL_TYPE_BIT - 1 );

    Db::TYPE result = ( isColumnUnsigned( index )
                        ? MYSQL_TYPE_TABLE_UNSIGNED
                        : MYSQL_TYPE_TABLE_SIGNED )[ columnType ];

    /* if result is (wide) binary string, set result to DBTYPE_BYTES. */
    if( ( Db::TYPE_STR == result
          || Db::TYPE_WSTR == result )
        && isColumnBinary( index ) )
    {
        result = Db::TYPE_BYTES;
    }

    /* debug check */
    assert( Db::TYPE_ERROR != result );

    return result;
}

bool Db::QueryResult::isColumnUnsigned( size_t index ) const
{
    assert( index < columnCount() );
    return 0 != ( mFields[index]->flags & UNSIGNED_FLAG );
}

bool Db::QueryResult::isColumnBinary( size_t index ) const
{
    assert( index < columnCount() );
    // According to MySQL C API Documentation, binary string
    // fields like BLOB or VAR_BINARY have charset "63".
    return 63 == mFields[ index ]->charsetnr;
}

bool Db::QueryResult::GetRow( Db::ResultRow& into )
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

void Db::QueryResult::Reset()
{
    if( NULL != mResult )
        mysql_data_seek( mResult, 0);
}

void Db::QueryResult::SetResult( MYSQL_RES** res, size_t colCount )
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
/* Db::Core                                                              */
/*************************************************************************/
bool Db::Core::IsSafeString( const char* str )
{
    for(; *str != '\0'; ++str )
    {
        switch( *str )
        {
            case '\'':
            case '\\':
                return false;
        }
    }

    return true;
}

Db::Core::Core( bool compress, bool ssl )
: mMysqlStatus( STATUS_CLOSED ),
  mCompress( compress ),
  mSSL( ssl )
{
    mysql_init( &mMysql );
}

Db::Core::~Core()
{
    mysql_close( &mMysql );
}

bool Db::Core::RunQuery( Db::QueryResult& into, const char* queryFmt, ... )
{
    va_list ap;
    va_start( ap, queryFmt );

    char* query = NULL;
    size_t queryLen = vasprintf( &query, queryFmt, ap );

    va_end( ap );

    bool result = false;
    {
        MutexLock lock( mMysqlMutex );

        if( RunQueryLocked( into.error, query, queryLen ) )
        {
            size_t colCount = mysql_field_count( &mMysql );
            if( 0 == colCount )
            {
                into.error.SetError( 0xFFFF, "Db::Core::RunQuery: No Result" );
                sLog.Error( "Db::Core Query", "Query: %s failed because did not return a result", query );
            }
            else
            {
                MYSQL_RES* res = mysql_store_result( &mMysql );
                into.SetResult( &res, colCount );

                result = true;
            }
        }
    }

    SafeFree( query );
    return result;
}

bool Db::Core::RunQuery( Db::Error& err, const char* queryFmt, ... )
{
    va_list ap;
    va_start( ap, queryFmt );

    char* query = NULL;
    size_t queryLen = vasprintf( &query, queryFmt, ap );

    va_end( ap );

    bool result = false;
    {
        MutexLock lock( mMysqlMutex );
        result = RunQueryLocked( err, query, queryLen );
    }

    SafeFree( query );
    return result;
}

bool Db::Core::RunQuery( Db::Error& err, size_t& affectedRows, const char* queryFmt, ... )
{
    va_list ap;
    va_start( ap, queryFmt );

    char* query = NULL;
    size_t queryLen = vasprintf( &query, queryFmt, ap );

    va_end( ap );

    bool result = false;
    {
        MutexLock lock( mMysqlMutex );

        if( RunQueryLocked( err, query, queryLen ) )
        {
            affectedRows = mysql_affected_rows( &mMysql );
            result = true;
        }
    }

    SafeFree( query );
    return result;
}

bool Db::Core::RunQueryLID( Db::Error& err, uint64& lastInsertId, const char* queryFmt, ... )
{
    va_list ap;
    va_start( ap, queryFmt );

    char* query = NULL;
    size_t queryLen = vasprintf( &query, queryFmt, ap );

    va_end( ap );

    bool result = false;
    {
        MutexLock lock( mMysqlMutex );

        if( RunQueryLocked( err, query, queryLen ) )
        {
            lastInsertId = mysql_insert_id( &mMysql );
            result = true;
        }
    }

    SafeFree( query );
    return result;
}

bool Db::Core::RunQuery( const char* query, size_t queryLen, MYSQL_RES** result,
                       unsigned int* errNo, char* errMsg, size_t* affectedRows,
                       uint64* lastInsertId, bool retry )
{
    if( NULL != errNo )
        *errNo = 0;

    if( NULL != errMsg )
        errMsg[0] = 0;

    {
        MutexLock lock( mMysqlMutex );

        Db::Error err;
        if( !RunQueryLocked( err, query, queryLen, retry ) )
        {
            sLog.Error( "Db::Core Query", "Query: %s failed", query );

            if( NULL != errNo )
                *errNo = err.no();

            if( NULL != errMsg )
                strcpy( errMsg, err.msg() );

            return false;
        }

        if( result )
        {
            if( 0 != mysql_field_count( &mMysql ) )
                *result = mysql_store_result( &mMysql );
            else
            {
                *result = NULL;

                if( NULL != errNo )
                    *errNo = UINT_MAX;

                if( NULL != errMsg )
                    strcpy( errMsg, "Db::Core::RunQuery: No Result" );

                sLog.Error( "Db::Core Query", "Query: %s failed because it should return a result", query );
                return false;
            }
        }

        if( NULL != affectedRows )
            *affectedRows = mysql_affected_rows( &mMysql );

        if( NULL != lastInsertId )
            *lastInsertId = mysql_insert_id( &mMysql );
    }

    return true;
}

size_t Db::Core::DoEscapeString( char* to, const char* from, size_t len )
{
    MutexLock lock( mMysqlMutex );

    return mysql_real_escape_string( &mMysql, to, from, len );
}

void Db::Core::DoEscapeString( std::string& to, const std::string& from )
{
    // make enough room
    size_t len = from.length();
    to.resize( 2 * len + 1 );

    {
        MutexLock lock( mMysqlMutex );

        len = mysql_real_escape_string( &mMysql, &to[0], &from[0], len );
    }

    to.resize( len + 1 );
}

void Db::Core::Ping()
{
    // well, if it's locked, someone's using it. If someone's using it, it doesn't need a ping
    if( mMysqlMutex.TryLock() )
    {
        mysql_ping( &mMysql );
        mMysqlMutex.Unlock();
    }
}

bool Db::Core::Open( const char* host, const char* user, const char* password,
                   const char* database, int16 port, unsigned int* errNo, char* errMsg,
                   bool compress, bool ssl )
{
    MutexLock lock( mMysqlMutex );

    mHost = host;
    mUser = user;
    mPassword = password;
    mDatabase = database;
    mPort = port;

    mCompress = compress;
    mSSL = ssl;

    return OpenLocked( errNo, errMsg );
}

bool Db::Core::Open( Db::Error& err, const char* host, const char* user, const char* password,
                   const char* database, int16 port, bool compress, bool ssl )
{
    MutexLock lock( mMysqlMutex );

    mHost = host;
    mUser = user;
    mPassword = password;
    mDatabase = database;
    mPort = port;

    mCompress = compress;
    mSSL = ssl;

    unsigned int errNo;
    char errMsg[ 0x400 ];

    if( !OpenLocked( &errNo, errMsg ) )
    {
        err.SetError( errNo, errMsg );
        return false;
    }

    return true;
}


bool Db::Core::OpenLocked( unsigned int* errNo, char* errMsg )
{
    if( NULL != errMsg )
        errMsg[0] = 0;

    if( STATUS_CONNECTED == status() )
        return true;
    else if( STATUS_ERROR == status() )
        //do we need to call init again?
        mysql_close( &mMysql );

    if( mHost.empty() )
        return false;

    sLog.Message( "dbcore",
                  "Connecting to\n"
                  "    DB:     %s\n"
                  "    server: %s:%d\n"
                  "    user:   %s",
                  mDatabase.c_str(),
                  mHost.c_str(), mPort,
                  mUser.c_str() );

    /*
    Quagmire - added CLIENT_FOUND_ROWS flag to the connect
    otherwise DB update calls would say 0 rows affected when the value already equaled
    what the function was trying to set it to, therefore the function would think it failed
    */
    unsigned long flags = CLIENT_FOUND_ROWS;

    if( mCompress )
        flags |= CLIENT_COMPRESS;

    if( mSSL )
        flags |= CLIENT_SSL;

    if( mysql_real_connect( &mMysql, mHost.c_str(), mUser.c_str(), mPassword.c_str(),
                            mDatabase.c_str(), mPort, 0, flags ) )
    {
        mMysqlStatus = STATUS_CONNECTED;
    }
    else
    {
        mMysqlStatus = STATUS_ERROR;

        if( NULL != errNo )
            *errNo = mysql_errno( &mMysql );

        if( NULL != errMsg )
            snprintf( errMsg, MYSQL_ERRMSG_SIZE, "#%i: %s",
                      mysql_errno( &mMysql ), mysql_error( &mMysql ) );

        return false;
    }

    // Turn on UTF-8 character encoding
    if( 0 != mysql_set_character_set( &mMysql, "utf8" ) )
    {
        mMysqlStatus = STATUS_ERROR;

        if( NULL != errNo )
            *errNo = mysql_errno( &mMysql );

        if( NULL != errMsg )
            snprintf( errMsg, MYSQL_ERRMSG_SIZE, "#%i: %s",
                      mysql_errno( &mMysql ), mysql_error( &mMysql ) );

        return false;
    }

    return true;
}

bool Db::Core::RunQueryLocked( Db::Error& err, const char* query, size_t queryLen, bool retry )
{
    if( STATUS_CONNECTED != status() )
        OpenLocked();

    if( mysql_real_query( &mMysql, query, queryLen ) )
    {
        unsigned int errNo = mysql_errno( &mMysql );

        if( CR_SERVER_GONE_ERROR == errNo )
            mMysqlStatus = STATUS_ERROR;

        if( retry && ( CR_SERVER_LOST == errNo
                       || CR_SERVER_GONE_ERROR == errNo ) )
        {
            sLog.Error( "Db::Core", "Lost connection, attempting to recover...." );

            return RunQueryLocked( err, query, queryLen, false );
        }

        mMysqlStatus = STATUS_ERROR;
        err.SetError( errNo, mysql_error( &mMysql ) );

        sLog.Error( "Db::Core Query", "#%d in '%s': %s",
                    err.no(), query, err.msg() );
        return false;
    }

    err.ClearError();
    return true;
}
