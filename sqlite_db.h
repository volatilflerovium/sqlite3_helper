/*********************************************************************
* SqlRow class                               								*
* SQLiteDB class                                                     *
*                                                                    *
* Version: 2.0                                                       *
* Date:    16-10-2021                                                *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/
#ifndef SQLITE_DB_H
#define SQLITE_DB_H

#include <cstring>
#include <string>
#include <sqlite3.h> 

#include "sqlite_db_traits.h"
#include "sqlite_result_rows.h"

//######################################################################

typedef void (*SqlRowFunc)(SqlRows&);

/**
 * Wrapper for SQLite C++ Interface.
 * 
 * Simplifies quering a database using the SQLite C++ Interface. 
 * 
 */

class SQLiteDB 
{
	public:
		/**
		 * Constructor, opens a connection to an SQLite database file 
		 * using sqlite3_open_v2
		 *
	    * @param dbName Database file name.
	    * @param openMode The flags parameter to sqlite3_open_v2 
	    * @param zVfs The name of the sqlite3_vfs object that defines 
	    *     the operating system interface that the new database 
	    *     connection should use.
	    * @throws const char* thrown if it is not possible to open 
	    *     the connection to the database.  
	    * 
	    * @see [sqlite3_open_v2](https://www3.sqlite.org/c3ref/open.html)
       */
		SQLiteDB(const char* dbName, int openMode, const char* zVfs);

       /**
		 * Constructor, opens a connection to an SQLite database file 
		 * using sqlite3_open_v2
		 *
	    * @param dbName Database file name.
	    * @param openMode The flags parameter to sqlite3_open_v2 
	    * @throws const char* thrown if it is not possible to open 
	    *    the connection to the database.  
	    * 
	    * @see [sqlite3_open_v2](https://www3.sqlite.org/c3ref/open.html)
       */
		SQLiteDB(const char* dbName, int openMode=SQLITE_OPEN_READWRITE);
		
		 /**
		 * Constructor, opens a connection to an SQLite database file 
		 * using sqlite3_open16
		 *
	    * @param dbName Database file name.
	    * @throws const char* thrown if it is not possible to open 
	    *     the connection to the database.  
	    * 
	    * @see [sqlite3_open16](https://www3.sqlite.org/c3ref/open.html) 
       */
		SQLiteDB(const void* dbName);

		/**
		 * Destructor, close the database connection.
		 */
		virtual ~SQLiteDB();

		/**
		 * Return the error code of the last error ocurred 
		 * 
		 * @see [errcode](http://www.sqlite.org/c3ref/errcode.html).
		 */
		int lastErrorCode();

		/**
		 * Return a description of the last error 
		 * 
		 * @see [errcode](http://www.sqlite.org/c3ref/errcode.html).
		 */
		const char* lastErrorMsg();
		
		/**
		 * Returns the rowid of the most recent successful INSERT into a 
		 * rowid table or virtual table on database connection D. Inserts 
		 * into WITHOUT ROWID tables are not recorded. If no successful 
		 * INSERTs into rowid tables have ever occurred on the database 
		 * connection D, then sqlite3_last_insert_rowid(D) returns zero.
		 * 
		 * @see [Last Insert Rowid](https://sqlite.org/c3ref/last_insert_rowid.html)
		 */
		sqlite3_int64 lastInsertID();

		/**
		 * Returns true if any row was modified, inserted or deleted by the 
		 * most recently completed INSERT, UPDATE or DELETE.
		 * 
		 * @see [Count The Number Of Rows Modified](https://sqlite.org/c3ref/changes.html)
		 */
		bool dataChanged() const;

		/**
		 * Execute a SQL query.
		 *
		 * @tparam UTF the type for the SQL query to execute it only accept 
		 *       const char* for UTF8 and
		 *       const void* for UTF16
		 * @param query a SQL query
		 * @param prepFlags if prepFlags=0 then sqlite3_prepare_v2 is used,
		 *     if prepFlags>0, then this value is passed to sqlite3_prepare_v3
		 * @return bool true if the query executes succefully.
		 * 
		 * @note according to the documentation, the only difference between 
		 *      sqlite3_prepare_v2 and sqlite3_prepare_v3 is the prepFlags parameter.
		 *      Also, currently there are just 3 valid flags (0x01, 0x02, 0x04).
		 * 
		 * @see [Prepare Flags](https://www3.sqlite.org/c3ref/c_prepare_normalize.html#sqlitepreparepersistent)
		 */
		template<typename UTF>
		bool executeQuery(UTF query, unsigned int prepFlags=0);

		/**
		 * Execute a SQL query.
		 *
		 * @tparam UTF the type for the SQL query to execute it only accept 
		 *       const char* for UTF8 and
		 *       const void* for UTF16
		 * @param query a SQL query
		 * @param qParams wrapper structure for additional parameters needed
		 *     by sqlite3_prepare_v2 and sqlite3_prepare16_v2
		 * @return bool true if the query executes succefully.
		 * 
		 * @see QParams
		 */
		template<typename UTF>
		bool executeQuery(UTF query, QParams qParams);
		

		//######################################################

		/**
		 * Execute a SQL query.
		 * 
		 * @param query a SQL query
		 * @param prepFlags if prepFlags=0 then sqlite3_prepare_v2 is used,
		 *     if prepFlags>0, then this value is passed to sqlite3_prepare_v3
		 * @return a SqlRows object for the rows of the result of the query.
		 * 
		 * @see SqlRows
		 */ 
		template<typename UTF>
		SqlRows getResultRows(UTF query, unsigned int prepFlags=0);

		/**
		 * Execute a SQL query.
		 * 
		 * @param query a SQL query
		 * @param qParams wrapper for non trivial parameters of sqlite3_prepare_v2
		 *     sqlite3_prepare_v3
		 * @return a SqlRows object for the rows of the result of the query.
		 * 
		 * @see SqlRows
		 */
		template<typename UTF>
		SqlRows getResultRows(UTF query, QParams qParams);

		//######################################################
		/**
		 * Execute a SQL query and apply a callback function on 
		 * each row of the result.
		 * 
		 * @param query a SQL statement to be executed.
       * @param callback a function of type SqlRowFunc which will be 
       *    applied to each row of the result.
		 * @param prepFlags if prepFlags=0 then sqlite3_prepare_v2 is used,
		 *     if prepFlags>0, then this value is passed to sqlite3_prepare_v3
		 * 
       * @see SqlRowFunc
		 * 
		 */
		template<typename UTF> 
		void applyToRows(UTF query, SqlRowFunc callback, unsigned int prepFlags=0);

		/**
		 * Overload of SQLiteDB::applyToRows using QParams
		 * 
		 */
		template<typename UTF> 
		void applyToRows(UTF query, SqlRowFunc callback, QParams qParams);

		//######################################################

		/**
		 * Bind values to prepared SQL statement and execute the prepared query.
		 * 
		 * @param query a SQL template query with parameters '?'
		 * @param args variadic number of arguments, one for each unspecified 
		 *      parameter '?' and in the same order as they will be applied.
		 * @return SqlRows object holding the rows in the result.
		 * 
		 * @note some of these parameters to be binded are convenient wrapper 
		 *    structures that encapsulate specific parameters for the appropiate
		 *    binding routines. For example to bind a blob we can use the wrapper
		 *    structure blob(const void* v, int n, fn cbk);
		 * 
		 * @see BindParams
		 */
		template<typename UTF, typename... Args>
		SqlRows executeSecureQuery(unsigned int prepFlags, UTF query, Args ...args);

		/**
		 * Overload for SQLiteDB::executeSecureQuery(unsigned int prepFlags, UTF query, Args ...args);
		 */
		template<typename UTF, typename... Args>
		SqlRows executeSecureQuery(QParams qParams, UTF query, Args ...args);

		/**
		 * Wrapper for SQLiteDB::executeSecureQuery(0, query, args...);
		 */
		template<typename UTF, typename... Args>
		SqlRows executeSecureQueryNf(UTF query, Args ...args);

		//######################################################

		/**
		 * Execute a SQL query and get the value of the first column of 
		 * the first row of the results.
		 *
		 * @param query a SQL query
		 * @param[out] resultValue reference to a variable where the value 
		 *     result of the query will be set
		 * @param prepFlags if prepFlags=0 then sqlite3_prepare_v2 is used,
		 *     if prepFlags>0, then this value is passed to sqlite3_prepare_v3
		 * 
		 * @return bool true if the query executes succefully and the result 
		 *     has at least one row, false otherwise.
		 */
		template<typename UTF>
		bool uniqueAsInt(UTF query, int& resultValue, unsigned int prepFlags=0);

		/**
		 * Overload for SQLiteDB::uniqueAsInt(UTF query, int& resultValue, unsigned int prepFlags=0)
		 */
		template<typename UTF>
		bool uniqueAsInt(UTF query, int& resultValue, QParams qParams);

		//######################################################
		/**
		 * Execute a SQL query and get the value of the first column of 
		 * the first row of the results.
		 *
		 * @see SQLiteDB::uniqueAsInt
		 */
		template<typename UTF>
		bool uniqueAsDouble(UTF query, double& resultValue, unsigned int prepFlags=0);

		/**
		 * Execute a SQL query and get the value of the first column of 
		 * the first row of the results.
		 *
		 * @see SQLiteDB::uniqueAsInt
		 */
		template<typename UTF>
		bool uniqueAsDouble(UTF query, double& resultValue, QParams qParams);

		//######################################################

		/**
		 * Execute a SQL query and get the value of the first column of 
		 * the first row of the results.
		 *
		 * @see SQLiteDB::uniqueAsInt
		 */
		template<typename UTF>
		bool uniqueAsString(UTF query, std::string& resultValue, QParams qParams);
	
		/**
		 * Execute a SQL query and get the value of the first column of 
		 * the first row of the results.
		 *
		 * @see SQLiteDB::uniqueAsInt
		 */
		template<typename UTF>
		bool uniqueAsString(UTF query, std::string& resultValue, unsigned int prepFlags=0);


	protected:
		sqlite3* m_DB;
		/*The number of columns in the result set. As far as the 
		 * query return at least a row, m_numColmns will be the 
		 * number of columns in that row.
		*/
		int m_numColumns;

		template<typename UTF, typename T, typename P=unsigned int>
		bool getUnique(UTF query, T& resultValue, P qParams=0);

		template<typename UTF, typename P=unsigned int>
		SqlRows getResultRowsInner(UTF query, P qParams=0);

		template<typename UTF, typename P=unsigned int> 
		void applyToRowsInner(UTF query, SqlRowFunc callback, P qParams=0);

		template<typename UTF, typename P>
		bool executeQueryInner(UTF query, P qParams);
};

//======================================================================
//======================================================================

SQLiteDB::SQLiteDB(const char* dbName, int openMode, const char* zVfs)
:m_DB(nullptr)
{
	if(sqlite3_open_v2(dbName, &m_DB, openMode, zVfs)>0){
		throw sqlite3_errmsg(m_DB);
	}
}

//======================================================================

SQLiteDB::SQLiteDB(const char* dbName, int openMode)
:SQLiteDB(dbName, openMode, NULL)
{}

//======================================================================
SQLiteDB::SQLiteDB(const void* dbName)
:m_DB(nullptr)
{
	if(sqlite3_open16(dbName, &m_DB)>0){
		throw sqlite3_errmsg(m_DB);
	}
}
//======================================================================
inline SQLiteDB::~SQLiteDB(){
	sqlite3_close(m_DB);
}	
//======================================================================
inline int SQLiteDB::lastErrorCode()
{
	return sqlite3_errcode(m_DB);
}
//======================================================================
inline const char* SQLiteDB::lastErrorMsg()
{
	return sqlite3_errmsg(m_DB);
}

//======================================================================

inline sqlite3_int64 SQLiteDB::lastInsertID()
{
	return sqlite3_last_insert_rowid(m_DB);
}

//======================================================================

inline bool SQLiteDB::dataChanged() const
{
	return (sqlite3_changes(m_DB) != 0);
}

//======================================================================


template<typename UTF, typename T, typename P=unsigned int>
bool SQLiteDB::getUnique(UTF query, T& resultValue, P qParams){
	static_assert(DB_CONNECT<UTF>::is_valid, "parameter query should be const char* or const void*");
	static_assert(IS_QParam<P>::is_valid, "qParams should be QParams or unsigned int");

	sqlite3_stmt* statement;
	if(sqlite3Prepare(m_DB, query, &statement, qParams) == SQLITE_OK){
		m_numColumns = sqlite3_column_count(statement);
		if (m_numColumns){
			if (SQLITE_ROW == sqlite3_step(statement)){
				resultValue=ColumnData<T>::getColumnData(statement, 0);
				sqlite3_finalize(statement);
				return true;
			}
		}
	}
	sqlite3_finalize(statement);

	return false;
}

//----------------------------------------------------------------------

template<typename UTF, typename P=unsigned int>
SqlRows SQLiteDB::getResultRowsInner(UTF query, P qParams) {
	static_assert(DB_CONNECT<UTF>::is_valid, "parameter query should be const char* or const void*");
	static_assert(IS_QParam<P>::is_valid, "qParams should be QParams or unsigned int");

	sqlite3_stmt* statement;
	if(sqlite3Prepare(m_DB, query, &statement, qParams) == SQLITE_OK){
		m_numColumns = sqlite3_column_count(statement);
		if (m_numColumns){			
			return statement;			
			// no need to call sqlite3_finalize here as it will be called by 
			//the destructor of SqlRow
		}
	}
	sqlite3_finalize(statement);

	return nullptr;
}

//----------------------------------------------------------------------

template<typename UTF>
SqlRows SQLiteDB::getResultRows(UTF query, QParams qParams){
	return getResultRowsInner(query, qParams);
}

template<typename UTF>
SqlRows SQLiteDB::getResultRows(UTF query, unsigned int prepFlags){
	return getResultRowsInner(query, prepFlags);
}

template<typename UTF> 
void SQLiteDB::applyToRows(UTF query, SqlRowFunc callback, QParams qParams){
	applyToRowsInner(query, callback, qParams);
}


template<typename UTF> 
void SQLiteDB::applyToRows(UTF query, SqlRowFunc callback, unsigned int prepFlags){
	applyToRowsInner(query, callback, prepFlags);
}

template<typename UTF>
bool SQLiteDB::executeQuery(UTF query, QParams qParams){
	return executeQueryInner(query, qParams);
}

template<typename UTF>
bool SQLiteDB::executeQuery(UTF query, unsigned int prepFlags){
	return executeQueryInner(query, prepFlags);
}

//----------------------------------------------------------------------

template<typename UTF, typename P=unsigned int>
bool SQLiteDB::executeQueryInner(UTF query, P qParams) {
	static_assert(DB_CONNECT<UTF>::is_valid, "parameter query should be const char* or const void*");
	static_assert(IS_QParam<P>::is_valid, "qParams should be QParams or unsigned int");

	sqlite3_stmt* statement;
	if(sqlite3Prepare(m_DB, query, &statement, qParams) == SQLITE_OK){
		sqlite3_step(statement);		

		return true;
	}
	sqlite3_finalize(statement);

	return false;
}

//----------------------------------------------------------------------

template<typename UTF, typename... Args>
SqlRows SQLiteDB::executeSecureQuery(QParams qParams, UTF query, Args ...args){
	static_assert(DB_CONNECT<UTF>::is_valid, "parameter query should be const char* or const void*");

	sqlite3_stmt* statement;
	if(sqlite3Prepare(m_DB, query, &statement, qParams) == SQLITE_OK){
		if(SQLITE_OK==binding(statement, 0, std::forward<Args>(args)...)){
			m_numColumns = sqlite3_column_count(statement);
			if (m_numColumns){
				return statement;
			}
			sqlite3_step(statement);
		}
	}
	sqlite3_finalize(statement);

	return nullptr;
}

//----------------------------------------------------------------------

template<typename UTF, typename... Args>
SqlRows SQLiteDB::executeSecureQuery(unsigned int prepFlags, UTF query, Args ...args){
	static_assert(DB_CONNECT<UTF>::is_valid, "parameter query should be const char* or const void*");
	if(prepFlags==0){
		return 
		executeSecureQuery<UTF>(QParams(DB_CONNECT<UTF>::strLength(query), DB_CONNECT<UTF>::is_utf8), query, std::forward<Args>(args)...);
	}
	return executeSecureQuery<UTF>(QParams(DB_CONNECT<UTF>::strLength(query), DB_CONNECT<UTF>::is_utf8, prepFlags), query, std::forward<Args>(args)...);
}


template<typename UTF, typename... Args>
SqlRows SQLiteDB::executeSecureQueryNf(UTF query, Args ...args){
	return executeSecureQuery<UTF, Args...>(QParams(DB_CONNECT<UTF>::strLength(query), DB_CONNECT<UTF>::is_utf8), query, std::forward<Args>(args)...);
}
//----------------------------------------------------------------------

template<typename UTF, typename P=unsigned int>
void SQLiteDB::applyToRowsInner(UTF query, SqlRowFunc callback, P qParams) {
	SqlRows row=getResultRowsInner(query, qParams);
	while(row.yield()){
		callback(row);
	}
}

//----------------------------------------------------------------------

template<typename UTF>
bool SQLiteDB::uniqueAsInt(UTF query, int& resultValue, QParams qParams){
	return getUnique<UTF, int>(query, resultValue, qParams);
}

template<typename UTF>
bool SQLiteDB::uniqueAsInt(UTF query, int& resultValue, unsigned int prepFlags){
	return getUnique<UTF, int>(query, resultValue, prepFlags);
}
//----------------------------------------------------------------------

template<typename UTF>
bool SQLiteDB::uniqueAsDouble(UTF query, double& resultValue, QParams qParams){
	return getUnique<UTF, double>(query, resultValue, qParams);
}

template<typename UTF>
bool SQLiteDB::uniqueAsDouble(UTF query, double& resultValue, unsigned int prepFlags){
	return getUnique<UTF, double>(query, resultValue, prepFlags);
}

//----------------------------------------------------------------------

template<typename UTF>
bool SQLiteDB::uniqueAsString(UTF query, std::string& resultValue, QParams qParams){
	return getUnique<UTF, std::string>(query, resultValue, qParams);
}

template<typename UTF>
bool SQLiteDB::uniqueAsString(UTF query, std::string& resultValue, unsigned int prepFlags){
	return getUnique<UTF, std::string>(query, resultValue, prepFlags);
}

//######################################################################
//######################################################################


#endif
