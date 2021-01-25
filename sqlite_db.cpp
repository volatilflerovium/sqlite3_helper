/*********************************************************************
* SqlRow class                               								*
* SQLiteDB class                                                     *
*                                                                    *
* Version: 1.0                                                       *
* Date:    23-01-2021                                                *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/
#include "sqlite_db.h"

const int TypeToFunction<int>::defaultValue(0);
TypeToFunction<int>::FuncInt TypeToFunction<int>::getColumnData = &sqlite3_column_int;

const double TypeToFunction<double>::defaultValue(0);
TypeToFunction<double>::FuncDouble TypeToFunction<double>::getColumnData = &sqlite3_column_double;

std::string TypeToFunction<std::string>::defaultValue("");
TypeToFunction<std::string>::FuncText TypeToFunction<std::string>::getColumnData = &TypeToFunction<std::string>::columnToString;


bool operator<(const SqlRows::FieldName& fieldNameL, const SqlRows::FieldName& fieldNameR) {
	return std::strcmp(fieldNameL.field, fieldNameR.field) < 0;
}

SqlRows SQLiteDB::getRows(const char* query) {
	sqlite3_stmt* statement;
	if (sqlite3_prepare_v2(DB, query, -1, &statement, 0) == SQLITE_OK){
		m_numColumns = sqlite3_column_count(statement);
		if (m_numColumns){
			return SqlRows(statement);
		}
	}
	return SqlRows(nullptr);
}
