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

#ifndef __COMMON__DB__CORE_H__INCL__
#define __COMMON__DB__CORE_H__INCL__

#include "db/Type.h"

//this whole file could be interface-ized to support a different database
//if you can get over the SQL incompatibilities and mysql auto increment problems.

namespace common
{
    /**
     * @brief Houses all database-related stuff.
     */
    namespace db
    {
        class QueryResult;

        /**
         * @brief Represents an error during DB query.
         *
         * @author Zhur
         */
        class Error
        {
            friend class Core;

        public:
            /// A primary constructor.
            Error();

            /// @return An error number.
            unsigned int no() const { return mErrNo; }
            /// @return An error message.
            const char* msg() const { return mErrMsg; }

        protected:
            /**
             * @brief Sets the error.
             *
             * @param[in] errNo  An error number.
             * @param[in] errMsg A description of the error, must be static.
             */
            void SetError( unsigned int errNo, const char* errMsg );
            /**
             * @brief Clears the error.
             */
            void ClearError();

            /// An error number.
            unsigned int mErrNo;
            /// A static string describing the error.
            const char* mErrMsg;
        };

        /**
         * @brief A single row in a result.
         *
         * @author Zhur
         */
        class ResultRow
        {
            friend class QueryResult;

        public:
            /// A primary constructor.
            ResultRow();

            /// @see QueryResult::columnCount() const
            size_t count() const;
            /// @see QueryResult::columnName( size_t ) const
            const char* name( size_t index ) const;
            /// @see QueryResult::columnType( size_t ) const
            TYPE type( size_t index ) const;
            /**
             * @brief Obtains length of a column.
             *
             * @param[in] index An index of the column.
             *
             * @return The length.
             */
            size_t length( size_t index ) const;

            /// @see QueryResult::isColumnUnsigned( size_t ) const
            bool isUnsigned( size_t index ) const;
            /// @see QueryResult::isColumnBinary( size_t ) const
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
                return util::String< char >::to< T >( mRow[ index ] );
            }

        protected:
            /**
             * @brief Sets the row.
             *
             * @param[in] res     A query result.
             * @param[in] row     A row from the query result.
             * @param[in] lengths An array with lengths of columns.
             */
            void SetRow( QueryResult* res, const MYSQL_ROW& row, const unsigned long* lengths );

            /// The row.
            MYSQL_ROW mRow;
            /// The lengths array.
            const unsigned long* mLengths;

            /// The associated query result.
            QueryResult* mResult;
        };

        /**
         * @brief A database query result.
         *
         * @author Zhur
         */
        class QueryResult
        {
            friend class Core;

        public:
            /// A primary constructor.
            QueryResult();
            /// A destructor.
            ~QueryResult();

            /// A query error (if Core::RunQuery returned <code>false</code>).
            Error error;

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
            TYPE columnType( size_t index ) const;
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
             * @param[out] into Where to store the result.
             *
             * @retval true  Operation successfull.
             * @retval false Operation failed (most likely no more rows left).
             */
            bool GetRow( ResultRow& into );
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

            /// A MYSQL type -> TYPE signed column type map.
            static const TYPE MYSQL_TYPE_TABLE_SIGNED[];
            /// A MYSQL type -> TYPE unsigned column type map.
            static const TYPE MYSQL_TYPE_TABLE_UNSIGNED[];
        };

        /**
         * @brief A database service.
         *
         * @author Zhur
         */
        class Core
        : public util::Singleton< Core >
        {
        public:
            /// Describes current status of MySQL connection.
            enum Status
            {
                /// No connection.
                STATUS_CLOSED,
                /// Successfully connected.
                STATUS_CONNECTED,
                /// An error occurred.
                STATUS_ERROR
            };

            /**
             * @brief Checks for characters which might cause SQL problems.
             *
             * @param[in] str A string to check.
             *
             * @retval true  The string is dangerous.
             * @retval false The string is OK.
             */
            static bool IsSafeString( const char* str );

            /**
             * @brief A primary constructor.
             *
             * @param[in] compress Use compression during transmission.
             * @param[in] ssl      Use encryption during transmission.
             */
            Core( bool compress = false, bool ssl = false );
            /// A destructor.
            ~Core();

            /// Obtains current status of MySQL connection.
            Status status() const { return mMysqlStatus; }

            /**
             * @brief Runs a query.
             *
             * @param[out] into     Where to store the result.
             * @param[in]  queryFmt Format string of the query.
             * @param[in]  ...      Arguments to embed into @a queryFmt.
             *
             * @retval true  Query successfull.
             * @retval false Query failed.
             */
            bool RunQuery( QueryResult& into, const char* queryFmt, ... );
            /**
             * @brief Runs a query, ignoring any returned data.
             *
             * @param[out] err      Where to store an error (if any occurs).
             * @param[in]  queryFmt Format string of the query.
             * @param[in]  ...      Arguments to embed into @a queryFmt.
             *
             * @retval true  Query successfull.
             * @retval false Query failed.
             */
            bool RunQuery( Error& err, const char* queryFmt, ... );
            /**
             * @brief Runs a query, obtaining a number of affected rows.
             *
             * @param[out] err          Where to store an error (if any occurs).
             * @param[out] affectedRows Where to store the number of affected rows.
             * @param[in]  queryFmt     Format string of the query.
             * @param[in]  ...          Arguments to embed into @a queryFmt.
             *
             * @retval true  Query successfull.
             * @retval false Query failed.
             */
            bool RunQuery( Error& err, size_t& affectedRows, const char* queryFmt, ... );
            /**
             * @brief Runs a query, obtaining a last inserted ID.
             *
             * @param[out] err          Where to store an error (if any occurs).
             * @param[out] lastInsertId Where to store the last inserted ID.
             * @param[in]  queryFmt     Format string of the query.
             * @param[in]  ...          Arguments to embed into @a queryFmt.
             *
             * @retval true  Query successfull.
             * @retval false Query failed.
             */
            bool RunQueryLID( Error& err, uint64& lastInsertId, const char* queryFmt, ... );

            /**
             * @brief Runs a query.
             *
             * @param[in]  query        The query to run.
             * @param[in]  queryLen     A length of the query.
             * @param[out] result       Where to store the result of the query.
             * @param[out] errNo        Where to store an error ID (if an error occurs).
             * @param[out] errMsg       Where to store an error message (if an error occurs).
             * @param[out] affectedRows Where to store a number of affected rows.
             * @param[out] lastInsertId Where to store a last inserted ID.
             * @param[in]  retry        Try connecting again if a connection is lost.
             *
             * @retval true  Query successfull.
             * @retval false Query failed.
             */
            bool RunQuery( const char* query, size_t queryLen, MYSQL_RES** result = NULL,
                           unsigned int* errNo = NULL, char* errMsg = NULL, size_t* affectedRows = NULL,
                           uint64* lastInsertId = NULL, bool retry = true );

            /**
             * @brief Escapes a string.
             *
             * @param[out] to   Where to store the escaped string.
             * @param[in]  from The string to escape.
             * @param[in]  len  A length of the string to escape.
             *
             * @return A length of the escaped string.
             */
            size_t DoEscapeString( char* to, const char* from, size_t len );
            /**
             * @brief Escapes a string.
             *
             * @param[out] to   Where to store the escaped string.
             * @param[in]  from The string to escape.
             */
            void DoEscapeString( std::string& to, const std::string& from );
            /**
             * @brief Pings the server.
             */
            void Ping();

            /**
             * @brief Opens a connection to a MySQL server.
             *
             * @param[in]  host     A hostname of the server.
             * @param[in]  user     A username for authentication.
             * @param[in]  password A password for authentication.
             * @param[in]  database A name of the database to use.
             * @param[in]  port     A port on which the server is listening.
             * @param[out] errNo    Where to store an error ID (if an error occurs).
             * @param[out] errMsg   Where to store an error message (if an error occurs).
             * @param[in]  compress Use compression during transmission.
             * @param[in]  ssl      Use encryption during transmission.
             *
             * @retval true  Connection successfull.
             * @retval false Connection failed.
             */
            bool Open( const char* host, const char* user, const char* password,
                       const char* database, int16 port, unsigned int* errNo = NULL,
                       char* errMsg = NULL, bool compress = false, bool ssl = false );
            /**
             * @brief Opens a connection to a MySQL server.
             *
             * @param[out] err      Where to store an error (if any occurs).
             * @param[in]  host     A hostname of the server.
             * @param[in]  user     A username for authentication.
             * @param[in]  password A password for authentication.
             * @param[in]  database A name of the database to use.
             * @param[in]  port     A port on which the server is listening.
             * @param[in]  compress Use compression during transmission.
             * @param[in]  ssl      Use encryption during transmission.
             *
             * @retval true  Connection successfull.
             * @retval false Connection failed.
             */
            bool Open( Error& err, const char* host, const char* user, const char* password,
                       const char* database, int16 port, bool compress = false, bool ssl = false );

        private:
            /**
             * @brief Opens a connection to the MySQL server.
             *
             * mMysqlMutex must be locked prior calling to this method.
             *
             * @param[out] errNo  Where to store an error ID (if an error occurs).
             * @param[out] errMsg Where to store an error message (if an error occurs).
             *
             * @retval true  Connection successfull.
             * @retval false Connection failed.
             */
            bool OpenLocked( unsigned int* errNo = 0, char* errMsg = 0 );
            /**
             * @brief Runs a query.
             *
             * mMysqlMutex must be locked prior calling to this method.
             *
             * @param[out] err      Where to store an error (if any occurs).
             * @param[in]  query    The query to run.
             * @param[in]  queryLen A length of the query.
             * @param[in]  retry    Try connecting again if a connection is lost.
             * 
             * @retval true  Query successfull.
             * @retval false Query failed.
             */
            bool RunQueryLocked( Error& err, const char* query, size_t queryLen, bool retry = true );

            /// The MYSQL object we're using.
            MYSQL mMysql;
            /// A mutex to protect mMysql.
            mt::Mutex mMysqlMutex;
            /// A status of mMysql.
            Status mMysqlStatus;

            /// A hostname of MySQL server.
            std::string mHost;
            /// An authentication username.
            std::string mUser;
            /// An authentication password.
            std::string mPassword;
            /// A database name to use.
            std::string mDatabase;
            /// A port the MySQL server is running on.
            int16 mPort;

            /// Use compression during transmission.
            bool mCompress;
            /// Use encryption during transmission.
            bool mSSL;
        };
    }
}

/// A macro for easier access to the common::db::Core object.
#define sDatabase \
    ( common::db::Core::get() )

#endif /* !__COMMON__DB__CORE_H__INCL__ */
