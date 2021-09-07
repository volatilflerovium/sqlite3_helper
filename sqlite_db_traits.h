/*********************************************************************
* TypeToFunction traits                                              *
* Templete bindData                                                  *
* Template binding                                                   *
*                                                                    *
* Version: 1.0                                                       *
* Date:    23-01-2021                                                *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/

#ifndef SQLITE_DB_TRAITS_H
#define SQLITE_DB_TRAITS_H

#include <cstring>
#include <string>
#include <sqlite3.h> 

template<typename T>
struct TypeToFunction
{};

template<>
struct TypeToFunction<int>
{
	typedef int(*ColFunc)(sqlite3_stmt*, int);
	typedef int(*BindFunc)(sqlite3_stmt*, int, int);
	static const int defaultValue;
	static ColFunc getColumnData;
	static BindFunc bindData;
};

template<>
struct TypeToFunction<double>
{
	typedef double(*ColFunc)(sqlite3_stmt*, int);
	typedef int(*BindFunc)(sqlite3_stmt*, int, double);
	static const double defaultValue;
	static ColFunc getColumnData;
	static BindFunc bindData;
};

template<>
struct TypeToFunction<std::string>
{
	typedef std::string(*ColFunc)(sqlite3_stmt*, int);
	typedef int(*BindFunc)(sqlite3_stmt*, int, const char*, int, void(*)(void*));
	static std::string defaultValue;
	static ColFunc getColumnData;
	static BindFunc bindData;
	static std::string columnToString(sqlite3_stmt* sqlitest, int){
		return reinterpret_cast<char const*>(sqlite3_column_text(sqlitest, 0));
	}
};


template<>
struct TypeToFunction<sqlite3_int64>
{
	typedef sqlite3_int64(*ColFunc)(sqlite3_stmt*, int);
	typedef int(*BindFunc)(sqlite3_stmt*, int, sqlite3_int64);
	static const int defaultValue;
	static ColFunc getColumnData;
	static BindFunc bindData;
};

template<>
struct TypeToFunction<sqlite3_value*>
{
	typedef sqlite3_value*(*ColFunc)(sqlite3_stmt*, int);
	typedef int(*BindFunc)(sqlite3_stmt*, int, const sqlite3_value*);
	static const int defaultValue;
	static ColFunc getColumnData;
	static BindFunc bindData;
};

template<>
struct TypeToFunction<const void*>
{
	typedef const void*(*ColFunc)(sqlite3_stmt*, int);
	typedef int(*BindFunc)(sqlite3_stmt*, int, const void*, int n, void(*)(void*));
	static const int defaultValue;
	static ColFunc getColumnData;
	static BindFunc bindData;
};

//########################################################################

template<typename T>
int bindData(sqlite3_stmt* statement, int pos, T t){
	return TypeToFunction<T>::bindData(statement, pos, t);
}

inline int bindData(sqlite3_stmt* statement, int pos, std::string str){
	return TypeToFunction<std::string>::bindData(statement, pos, str.c_str(), -1, NULL);
}


inline int bindData(sqlite3_stmt* statement, int pos, const char* cstr){
	return sqlite3_bind_text(statement, pos, cstr, -1, NULL);
}

inline int bindData(sqlite3_stmt* statement, int pos, const void* data){
	return TypeToFunction<const void*>::bindData(statement, pos, data, -1, NULL);
}
 
template<typename T>
int binding(sqlite3_stmt* statement, int r, T t){
	return bindData(statement, r+1, t);
}

template<typename T, typename... Args>
int binding(sqlite3_stmt* statement, int r, T t, Args ...args){
	if(SQLITE_OK!=bindData(statement, r+1, t)){
		return SQLITE_ERROR;
	}
	return binding(statement, r+1, args...);
}

#endif
