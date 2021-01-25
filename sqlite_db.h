/*********************************************************************
* SqlRow class                               								*
* SQLiteDB class                                                     *
*                                                                    *
* Version: 1.0                                                       *
* Date:    23-01-2021                                                *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/

#ifndef SQLITE_DB_H
#define SQLITE_DB_H

#include <cstring>
#include <string>
#include <sqlite3.h> 
#include <map>
#include <functional>

//using namespace std;

//########################################################################

template<typename T>
struct TypeToFunction
{};

template<>
struct TypeToFunction<int>
{
	typedef int(*FuncInt)(sqlite3_stmt*, int);
	static const int defaultValue;
	static FuncInt getColumnData;
};

template<>
struct TypeToFunction<double>
{
	typedef double(*FuncDouble)(sqlite3_stmt*, int);
	static const double defaultValue;
	static FuncDouble getColumnData;
};

template<>
struct TypeToFunction<std::string>
{
	typedef std::string(*FuncText)(sqlite3_stmt*, int);
	static std::string defaultValue;
	static FuncText getColumnData;
	static std::string columnToString(sqlite3_stmt* sqlitest, int){
		return reinterpret_cast<char const*>(sqlite3_column_text(sqlitest, 0));
	}
};

//########################################################################

class SqlRows
{
	public:
		SqlRows(sqlite3_stmt* statement)
			:m_statement(statement)
		{
			for (int i = 0; i < sqlite3_column_count(statement); i++) {
				m_fieldNames[sqlite3_column_name(statement, i)] = i;
			}
		}
	
		~SqlRows(){
			sqlite3_finalize(m_statement);
		}
	
		bool yield(){
			if (SQLITE_ROW == sqlite3_step(m_statement)){
				return true;
			}
			sqlite3_finalize(m_statement);
			m_statement = nullptr;
			return false;
		}
	
		int AS_INT(const char* field){
			return sqlite3_column_int(m_statement, m_fieldNames[field]);
		}
	
		double AS_DOUBLE(const char* field){
			return sqlite3_column_double(m_statement, m_fieldNames[field]);
		}
	
		const unsigned char* AS_TEXT(const char* field){
			return sqlite3_column_text(m_statement, m_fieldNames[field]);
		}
	
		struct FieldName {
			const char* field;
			FieldName(const char* cstr)
				:field(std::move(cstr))
			{}
		};
	
	private:
		std::map<FieldName, int> m_fieldNames;
		sqlite3_stmt* m_statement;
};

bool operator<(const SqlRows::FieldName& fieldNameL, const SqlRows::FieldName& fieldNameR);

//####################################################################

class SQLiteDB 
{
	public:
	
		~SQLiteDB()
		{
			sqlite3_close(DB);
		}
	
		SQLiteDB(const char* db)
			:DB(nullptr),
			m_changed(false)
		{
			if (sqlite3_open(db, &DB)){
				throw sqlite3_errmsg(DB);
			}
		}
	
		int lastErrorCode(){
			return sqlite3_errcode(DB);
		}

		const char* lastErrorMsg(){
			return sqlite3_errmsg(DB);
		}

		template<typename Func>
		void applyToRows(const char* query, Func callback);
	
		SqlRows getRows(const char* query);
	
		bool executeQuery(const char* query) {
			return executeQuery(query, [](void *data, int argc, char **argv, char **azColName) {return 0; });
		}
	
		template<typename Func>
		bool executeQuery(const char* query, Func callback);
	
		bool dataChanged() const{
			return m_changed;
		}

		bool uniqueAsInt(const char* query, int& resultValue){
			return getUnique<int>(query, resultValue);
		}

		bool uniqueAsDouble(const char* query, double& resultValue){
			return getUnique<double>(query, resultValue);
		}

		bool uniqueAsText(const char* query, std::string& resultValue){
			return getUnique<std::string>(query, resultValue);
		}

		sqlite3_int64 lastInsertID(){
			return sqlite3_last_insert_rowid(DB);
		}
	
	private:
		sqlite3* DB;
		bool m_changed;
		int m_numColumns;//The number of columns in the result set. As far as the query return at least a row, m_numColmns will be the number of columns in that row.
	
		template<typename T>
		bool getUnique(const char* query, T& resultValue);
};

template<typename T>
bool SQLiteDB::getUnique(const char* query, T& resultValue){
	sqlite3_stmt *statement;
	if (sqlite3_prepare_v2(DB, query, -1, &statement, 0) == SQLITE_OK){
		m_numColumns = sqlite3_column_count(statement);
		if (m_numColumns){
			if (SQLITE_ROW == sqlite3_step(statement)){
				resultValue=TypeToFunction<T>::getColumnData(statement, 0);
				return true;
			}
		}
	}
	return false;
}

template<typename Func>
void SQLiteDB::applyToRows(const char* query, Func callback) {
	sqlite3_stmt* statement;
	if (sqlite3_prepare_v2(DB, query, -1, &statement, 0) == SQLITE_OK){
		m_numColumns = sqlite3_column_count(statement);
		if (m_numColumns){
			SqlRows row(statement);
			while (row.yield()){
				callback(row);
			}
			// no need to call sqlite3_finalize here as it will be called by the destructor of SqlRow
		}
	}
}

template<typename Func>
bool SQLiteDB::executeQuery(const char* query, Func callback){
	char *zErrMsg = 0;
	const char* data = "";

	int rc = sqlite3_exec(DB, query, callback, (void*)data, &zErrMsg);

	if (rc != SQLITE_OK){
		sqlite3_free(zErrMsg);
		return false;
	}
	m_changed = (sqlite3_changes(DB) != 0);
	return true;
}


#endif
