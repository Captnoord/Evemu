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

#ifndef __DB__DBCORE_H__INCL__
#define __DB__DBCORE_H__INCL__

//this whole file could be interface-ized to support a different database
//if you can get over the SQL incompatibilities and mysql auto increment problems.

#include "db/DbType.h"
#include "mt/Mutex.h"
#include "utils/Singleton.h"
#include "utils/StrConv.h"

class DbQueryResult;

/**
 * @brief Represents an error during DB query.
 *
 * @author Zhur
 */
class DbError
{
    friend class DbCore;

public:
    /// A primary constructor.
    DbError();

    /// @return An error number.
    uint32 no() const { return mErrNo; }
    /// @return An error message.
    const char* msg() const { return mErrMsg; }

protected:
    /**
     * @brief Sets the error.
     *
     * @param[in] errNo  An error number.
     * @param[in] errMsg A description of the error, must be static.
     */
    void SetError( uint32 errNo, const char* errMsg );
    /**
     * @brief Clears the error.
     */
    void ClearError();

    /// An error number.
    uint32 mErrNo;
    /// A static string describing the error.
    const char* mErrMsg;
};

/**
 * @brief A single row in a result.
 *
 * @author Zhur
 */
class DbResultRow
{
    friend class DbQueryResult;

public:
    /// A primary constructor.
    DbResultRow();

    /// @see DbQueryResult::columnCount().
    size_t count() const;
    /// @see DbQueryResult::columnName( size_t ).
    const char* name( size_t index ) const;
    /// @see DbQueryResult::columnType( size_t ).
    DBTYPE type( size_t index ) const;
    /**
     * @brief Obtains length of a column.
     *
     * @param[in] index An index of the column.
     *
     * @return The length.
     */
    size_t length( size_t index ) const;

    /// @see DbQueryResult::isColumnUnsigned( size_t ).
    bool isUnsigned( size_t index ) const;
    /// @see DbQueryResult::isColumnSigned( size_t ).
    bool isBinary( size_t index ) const;
    /**
     * @brief Checks if a column is NULL.
     *
     * @param[in] index An index of the column.
     *
     * @return true  The column is NULL.
     * @return false The column is not NULL.
     */
    bool isNull( size_t index ) const;

    /**
     * @brief Obtains a value of a column.
     *
     * @param[in] index An index of the column.
     *
     * @return The value.
     */
    template< typename T >
    T as( size_t index ) const
    {
        assert( !isNull( index ) );
        return strTo< T >( mRow[ index ] );
    }

protected:
    /**
     * @brief Sets the row.
     *
     * @param[in] res     A query result.
     * @param[in] row     A row from the query result.
     * @param[in] lengths An array with lengths of columns.
     */
    void SetRow( DbQueryResult* res, const MYSQL_ROW& row, const unsigned long* lengths );

    /// The row.
    MYSQL_ROW mRow;
    /// The lengths array.
    const unsigned long* mLengths;

    /// The associated query result.
    DbQueryResult* mResult;
};

/**
 * @brief A database query result.
 *
 * @author Zhur
 */
class DbQueryResult
{
    friend class DbCore;

public:
    /// A primary constructor.
    DbQueryResult();
    /// A destructor.
    ~DbQueryResult();

    /// A query error (if Db::Core::RunQuery returned @c false).
    DbError error;

    /// @return A number of rows.
    size_t rowCount() const { return mResult->row_count; }
    /// @return A number of columns.
    size_t columnCount() const { return mColumnCount; }
    /**
     * @brief Obtains a name of a column.
     *
     * @param[in] index An index of the column.
     *
     * @return The name.
     */
    const char* columnName( size_t index ) const;
    /**
     * @brief Obtains a type of a column.
     *
     * @param[in] index An index of the column.
     *
     * @return The type.
     */
    DBTYPE columnType( size_t index ) const;
    /**
     * @brief Checks if the column is unsigned.
     *
     * @param[in] index An index of the column.
     *
     * @retval true  The column is unsigned.
     * @retval false The column is not unsigned.
     */
    bool isColumnUnsigned( size_t index ) const;
    /**
     * @brief Checks if the column is binary.
     *
     * @param[in] index An index of the column.
     *
     * @retval true  The column is binary.
     * @retval false The column is not binary.
     */
    bool isColumnBinary( size_t index ) const;

    /**
     * @brief Obtains a next result row.
     *
     * @param[in] into Where to store the result.
     *
     * @retval true  Operation successfull.
     * @return false Operation failed (most likely no more rows left).
     */
    bool GetRow( DbResultRow& into );
    /**
     * @brief Resets the result.
     */
    void Reset();

protected:
    /**
     * @brief Sets a new result.
     *
     * @param[in] res      The result.
     * @param[in] colCount A number of columns.
     */
    void SetResult( MYSQL_RES** res, size_t colCount );

    /// A column count.
    size_t mColumnCount;
    /// A result.
    MYSQL_RES* mResult;
    /// An array of rows.
    MYSQL_FIELD** mFields;

    /// A MYSQL -> DBTYPE signed column type map.
    static const DBTYPE MYSQL_DBTYPE_TABLE_SIGNED[];
    /// A MYSQL -> DBTYPE unsigned column type map.
    static const DBTYPE MYSQL_DBTYPE_TABLE_UNSIGNED[];
};

class DbCore
: public Singleton<DbCore>
{
public:
    enum eStatus { Closed, Connected, Error };

    DbCore(bool compress=false, bool ssl=false);
    ~DbCore();
    eStatus GetStatus() const { return pStatus; }

    //new shorter syntax:
    //query which returns a result (error is stored in the result if it occurs)
    bool    RunQuery(DbQueryResult &into, const char *query_fmt, ...);
    //query which returns no information except error status
    bool    RunQuery(DbError &err, const char *query_fmt, ...);
    //query which returns affected rows:
    bool    RunQuery(DbError &err, uint32 &affected_rows, const char *query_fmt, ...);
    //query which returns last insert ID:
    bool    RunQueryLID(DbError &err, uint32 &last_insert_id, const char *query_fmt, ...);

    //old style to be used with MakeAnyLengthString
    bool    RunQuery(const char* query, int32 querylen, char* errbuf = 0, MYSQL_RES** result = 0, int32* affected_rows = 0, int32* last_insert_id = 0, int32* errnum = 0, bool retry = true);

    int32   DoEscapeString(char* tobuf, const char* frombuf, int32 fromlen);
    void    DoEscapeString(std::string &to, const std::string &from);
    static bool IsSafeString(const char *str);
    void    ping();

//  static bool ReadDBINI(char *host, char *user, char *pass, char *db, int32 &port, bool &compress, bool *items);
    bool    Open(const char* iHost, const char* iUser, const char* iPassword, const char* iDatabase, int16 iPort, int32* errnum = 0, char* errbuf = 0, bool iCompress = false, bool iSSL = false);
    bool    Open(DbError &err, const char* iHost, const char* iUser, const char* iPassword, const char* iDatabase, int16 iPort, bool iCompress = false, bool iSSL = false);

protected:
    MYSQL*  getMySQL(){ return &mysql; }

private:
    //MDatabase must be locked before these calls:
    bool    Open_locked(int32* errnum = 0, char* errbuf = 0);
    bool    DoQuery_locked(DbError &err, const char *query, int32 querylen, bool retry = true);

    MYSQL   mysql;
    Mutex   MDatabase;
    eStatus pStatus;

    std::string pHost;
    std::string pUser;
    std::string pPassword;
    std::string pDatabase;
    bool    pCompress;
    int16   pPort;
    bool    pSSL;
};

#define sDatabase \
    ( DbCore::get() )

#endif /* !__DB__DBCORE_H__INCL__ */
