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
		 * Access the values in the current result row of a specific column 
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

		/**
		 * Specialised wrapper for SqlRows::data_as<int> to return integers values 
		 * from columns with same type. 
		 */
		int as_int(const char* field);

		/**
		 * Specialised wrapper for SqlRows::data_as<double> to return double values 
		 * from columns with same type. 
		 */
		double as_double(const char* field);

		/**
		 * Specialised wrapper for SqlRows::data_as<const char*> to return 
		 * const char* values from columns with same type. 
		 */
		const unsigned char* as_text(const char* field);

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

#endif
