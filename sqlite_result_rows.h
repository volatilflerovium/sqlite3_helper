/*********************************************************************
* SqlRow class                               								*
* SQLiteDB class                                                     *
*                                                                    *
* Version: 2.0                                                       *
* Date:    16-10-2021                                                *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/
#ifndef SQLITE_RESULT_ROWS_H
#define SQLITE_RESULT_ROWS_H

#include <cstring>
#include <string>
#include <sqlite3.h> 
#include <map>

#include "sqlite_db_traits.h"

//######################################################################

class SQLiteDB;

/**
 * Class to iterate through the rows in the result of a prepare statement.
 * 
 */

class SqlRows
{
	public:			
		virtual ~SqlRows();

		/**
		 * Iterate through the rows in the result.
		 * 
		 * @return bool true if there is any row available in the result,
		 *     false otherwise
		 */
		bool yield();
		
		/**
		 * Reset the state of the prepared statement object back to its 
		 * initial state, ready to be re-executed.
		 * 
		 * @return an appropriate error code.
		 * 
		 * @see (error codes)[https://www3.sqlite.org/rescode.html]
		 */
		int reset();

		/**
		 * Wrapper for sqlite3_column_int. 
		 */
		int as_int(const char* field);

		/**
		 * Wrapper for sqlite3_column_double. 
		 */
		double as_double(const char* field);

		/**
		 * Wrapper for sqlite3_column_text. 
		 */
		const unsigned char* as_text(const char* field);

		/**
		 * Wrapper for sqlite3_column_blob. 
		 */
		const void* as_blob(const char* field);

		/**
		 * Wrapper for sqlite3_column_int64. 
		 */
		sqlite3_int64 as_int64(const char* field);

		/**
		 * Wrapper for sqlite3_column_text16. 
		 */
		const void* as_text16(const char* field);

		/**
		 * Wrapper for sqlite3_column_value. 
		 */		
		sqlite3_value* as_value(const char* field);
		
		/**
		 * Wrapper for sqlite3_column_bytes. 
		 */
		int as_bytes(const char* field);

		/**
		 * Wrapper for sqlite3_column_bytes16. 
		 */
		int as_bytes16(const char* field);
		
		/**
		 * Wrapper for sqlite3_column_type. 
		 */
		int as_type(const char* field);

		/**
		 * Access the value in the current result row of a specific column 
		 * by the name of the column.
		 * 
		 * Every time SqlRows::yield() is called and return true, a new row
		 * from the result is loaded and ready to be consumed
		 * 
		 * @tparam the type of the column to be returned
		 * @param field the name of the column in the result
		 * @return the value of the column with name field
		 * 
		 */
		template<typename T>
		typename ColumnData<T>::returnType data_as(const char* field);

		struct FieldName {
			const char* field;
			FieldName(const char* cstr)
				:field(std::move(cstr))
			{}
		};

	private:
		std::map<FieldName, int> m_fieldNames;
		sqlite3_stmt* m_statement;
		 
		SqlRows(sqlite3_stmt* statement);
		
		int findKey(const char* field);

	friend SQLiteDB;
};

//----------------------------------------------------------------------

inline bool operator<(const SqlRows::FieldName& fieldNameL, const SqlRows::FieldName& fieldNameR) {
	return std::strcmp(fieldNameL.field, fieldNameR.field) < 0;
}

//----------------------------------------------------------------------

SqlRows::SqlRows(sqlite3_stmt* statement)
:m_statement(statement)					
{
	for (int i = 0; i < sqlite3_column_count(statement); i++) {
		m_fieldNames[reinterpret_cast<const char*>(sqlite3_column_name(statement, i))] = i;
	}
}

//----------------------------------------------------------------------

inline SqlRows::~SqlRows(){
	sqlite3_finalize(m_statement);
}

//----------------------------------------------------------------------

bool SqlRows::yield(){
	if (SQLITE_ROW == sqlite3_step(m_statement)){
		return true;
	}
	return false;
}

//----------------------------------------------------------------------

int SqlRows::reset(){
	return sqlite3_reset(m_statement);
}

//----------------------------------------------------------------------

int SqlRows::as_int(const char* field){
	return sqlite3_column_int(m_statement, findKey(field));
}

//----------------------------------------------------------------------

double SqlRows::as_double(const char* field){
	return sqlite3_column_double(m_statement, findKey(field));
}

//----------------------------------------------------------------------

const unsigned char* SqlRows::as_text(const char* field){
	return sqlite3_column_text(m_statement, findKey(field));
}

//----------------------------------------------------------------------

template<typename T>
typename ColumnData<T>::returnType SqlRows::data_as(const char* field){
	return ColumnData<T>::getColumnData(m_statement, findKey(field));
}

//----------------------------------------------------------------------

inline int SqlRows::findKey(const char* field){
	if(m_fieldNames.find(field)!=m_fieldNames.end()){
		return m_fieldNames[field];
	}
	throw "Key not found.";
}

//----------------------------------------------------------------------

inline const void* SqlRows::as_blob(const char* field){
	return sqlite3_column_blob(m_statement, findKey(field));
}

//----------------------------------------------------------------------

inline sqlite3_int64 SqlRows::as_int64(const char* field){
	return sqlite3_column_int64(m_statement, findKey(field));
}

//----------------------------------------------------------------------

inline const void* SqlRows::as_text16(const char* field){
	return sqlite3_column_text16(m_statement, findKey(field));
}

//----------------------------------------------------------------------

inline sqlite3_value* SqlRows::as_value(const char* field){
	return sqlite3_column_value(m_statement, findKey(field));
}

//----------------------------------------------------------------------

inline int SqlRows::as_bytes(const char* field){
	return sqlite3_column_bytes(m_statement, findKey(field));
}

//----------------------------------------------------------------------

inline int SqlRows::as_bytes16(const char* field){
	return sqlite3_column_bytes16(m_statement, findKey(field));
}

//----------------------------------------------------------------------

inline int SqlRows::as_type(const char* field){
	return sqlite3_column_type(m_statement, findKey(field));
}


#endif
