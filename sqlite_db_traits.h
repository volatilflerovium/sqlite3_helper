/*********************************************************************
* TypeToFunction traits                                              *
* Templete bindData                                                  *
* Template binding                                                   *
*                                                                    *
* Version: 2.0                                                       *
* Date:    16-10-2021                                                *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/
#ifndef SQLITE_DB_TRAITS_H
#define SQLITE_DB_TRAITS_H

#include <cstring>
#include <string>
#include <sqlite3.h> 

#include <iostream>

typedef void(*fn)(void*);

enum class SQLiteTypes
{
	Bytes,
	Bytes16,
	SQL_TYPE,
	NULL_TYPE,
};

struct bytes
{
	enum{val=static_cast<int>(SQLiteTypes::Bytes)};
};

struct bytes16
{
	enum{val=static_cast<int>(SQLiteTypes::Bytes16)};
};

struct sqlite_type
{
	enum{val=static_cast<int>(SQLiteTypes::SQL_TYPE)};
};

struct null_data
{
	enum{val=static_cast<int>(SQLiteTypes::NULL_TYPE)};
};

/*
 * Happy to choose more fancy/descritive names... whenSomebodyPayForThis... :-)
 */

template<typename T, typename U=int>
struct BindParams
{
	BindParams(T v, U n, fn cbk)
	:m_v(v),
	m_cbk(cbk),
	m_n(n)
	{}

	T m_v;
	fn m_cbk;
	U m_n;
};


struct blob : public BindParams<const void*>
{
	blob(const void* v, int n, fn cbk)
	:BindParams<const void*>(v, n, cbk)
	{}
};


struct blob64 : public BindParams<const void*, sqlite3_uint64>
{
	blob64(const void* v, sqlite3_uint64 n, fn cbk)
	:BindParams<const void*, sqlite3_uint64>(v, n, cbk)
	{}
};


struct text : public BindParams<const char*>
{
	text(const char* v, int n, fn cbk)
	:BindParams<const char*>(v, n, cbk)
	{}
};


struct text16 : public BindParams<const void*>
{
	text16(const void* v, int n, fn cbk)
	:BindParams<const void*>(v, n, cbk)
	{}
};


struct text64 : public BindParams<const char*, sqlite3_uint64>
{
	text64(const char* v, sqlite3_uint64 n, fn cbk, unsigned char encoding)
	:BindParams<const char*, sqlite3_uint64>(v, n, cbk),
	m_encoding(encoding)
	{}
	
	unsigned char m_encoding;
};


struct zeroblob
{
	explicit zeroblob(int n)
	: m_n(n)
	{}
	
	int m_n;
};


struct zeroblob64
{
	explicit zeroblob64(sqlite3_uint64 n)
	:m_n(n)
	{}
	
	sqlite3_uint64 m_n;
};


struct sqlite_ptr : public BindParams<void*, const char*>
{
	sqlite_ptr(void* v, const char* ch, fn cbk)
	:BindParams<void*, const char*>(v, ch, cbk)
	{}
};


template<typename T>
struct ColumnData
{};


template<>
struct ColumnData<int>
{
	typedef int returnType;
	typedef int(*ColFunc)(sqlite3_stmt*, int);
	static ColFunc getColumnData;
};


template<>
struct ColumnData<double>
{
	typedef double returnType;
	typedef double(*ColFunc)(sqlite3_stmt*, int);
	static ColFunc getColumnData;
};


template<>
struct ColumnData<std::string>
{
	typedef std::string returnType;
	typedef std::string(*ColFunc)(sqlite3_stmt*, int);
	static ColFunc getColumnData;
	static std::string columnToString(sqlite3_stmt* sqlitest, int){
		return reinterpret_cast<char const*>(sqlite3_column_text(sqlitest, 0));
	}
};


template<>
struct ColumnData<sqlite3_int64>
{
	typedef sqlite3_int64 returnType;
	typedef sqlite3_int64(*ColFunc)(sqlite3_stmt*, int);
	static ColFunc getColumnData;
};


template<>
struct ColumnData<sqlite3_value*>
{
	typedef sqlite3_value* returnType;
	typedef sqlite3_value*(*ColFunc)(sqlite3_stmt*, int);
	static ColFunc getColumnData;
};


template<>
struct ColumnData<blob>
{
	typedef const void* returnType;
	typedef const void*(*ColFunc)(sqlite3_stmt*, int);
	static ColFunc getColumnData;
};


template<>
struct ColumnData<const void*>
{
	typedef const void* returnType;
	typedef const void*(*ColFunc)(sqlite3_stmt*, int);
	static ColFunc getColumnData;
};


template<>
struct ColumnData<text>
{
	typedef const unsigned char* returnType;
	typedef const unsigned char*(*ColFunc)(sqlite3_stmt*, int);
	static ColFunc getColumnData;
};


template<>
struct ColumnData<text16>
{
	typedef const void* returnType;
	typedef const void*(*ColFunc)(sqlite3_stmt*, int);
	static ColFunc getColumnData;
};


template<>
struct ColumnData<bytes>
{
	typedef int returnType;
	typedef int(*ColFunc)(sqlite3_stmt*, int);
	static ColFunc getColumnData;
};


template<>
struct ColumnData<bytes16>
{
	typedef int returnType;
	typedef int(*ColFunc)(sqlite3_stmt*, int);
	static ColFunc getColumnData;
};


template<>
struct ColumnData<sqlite_type>
{
	typedef int returnType;
	typedef int(*ColFunc)(sqlite3_stmt*, int);
	static ColFunc getColumnData;
};

//======================================================================

template<typename T>
struct BindDataTrait
{};


template<>
struct BindDataTrait<int>
{
	typedef int(*BindFunc)(sqlite3_stmt*, int, int);
	static BindFunc bindData;
};


template<>
struct BindDataTrait<double>
{
	typedef double(*ColFunc)(sqlite3_stmt*, int);
	typedef int(*BindFunc)(sqlite3_stmt*, int, double);
	static ColFunc getColumnData;
	static BindFunc bindData;
};


template<>
struct BindDataTrait<std::string>
{
	typedef int(*BindFunc)(sqlite3_stmt*, int, const std::string&);
	static BindFunc bindData;
	static int bindStringData(sqlite3_stmt* statement, int t, const std::string& str){
		return sqlite3_bind_text(statement, t, str.c_str(), -1, NULL);;
	}
};


template<>
struct BindDataTrait<const char*>
{
	typedef int(*BindFunc)(sqlite3_stmt*, int, const char*);
	static BindFunc bindData;
	static int bindCstringData(sqlite3_stmt* statement, int t, const char* cstr){
		return sqlite3_bind_text(statement, t, cstr, -1, NULL);;
	}
};


template<>
struct BindDataTrait<sqlite3_int64>
{
	typedef int(*BindFunc)(sqlite3_stmt*, int, sqlite3_int64);
	static BindFunc bindData;
};


template<>
struct BindDataTrait<sqlite3_value*>
{
	typedef int(*BindFunc)(sqlite3_stmt*, int, const sqlite3_value*);
	static BindFunc bindData;
};


template<>
struct BindDataTrait<blob>
{
	typedef int(*BindFunc)(sqlite3_stmt*, int, blob);
	static BindFunc bindData;
	static int bindBlobData(sqlite3_stmt* sqlitest, int t, blob blobData){		
		return sqlite3_bind_blob(sqlitest, t, blobData.m_v, blobData.m_n, blobData.m_cbk);
	}
};


template<>
struct BindDataTrait<blob64>
{
	typedef int(*BindFunc)(sqlite3_stmt*, int, blob64);
	static BindFunc bindData;
	static int bindBlob64Data(sqlite3_stmt* stmt, int t, blob64 blob64Data){
		return sqlite3_bind_blob64(stmt, t, blob64Data.m_v, blob64Data.m_n, blob64Data.m_cbk);
	}
};


template<>
struct BindDataTrait<text>
{
	typedef int(*BindFunc)(sqlite3_stmt*, int, text);
	static BindFunc bindData;
	static int bindTextData(sqlite3_stmt* stmt, int t, text textData){
		return sqlite3_bind_text(stmt, t, textData.m_v, textData.m_n, textData.m_cbk);
	}
};


template<>
struct BindDataTrait<text16>
{
	typedef int(*BindFunc)(sqlite3_stmt*, int, text16);
	static BindFunc bindData;
	static int bindText16Data(sqlite3_stmt* stmt, int t, text16 textData){
		return sqlite3_bind_text16(stmt, t, textData.m_v, textData.m_n, textData.m_cbk);
	}
};


template<>
struct BindDataTrait<text64>
{
	typedef int(*BindFunc)(sqlite3_stmt*, int, text64);
	static BindFunc bindData;
	static int bindText64Data(sqlite3_stmt* stmt, int t, text64 text64Data){
		return sqlite3_bind_text64(stmt, t, text64Data.m_v, text64Data.m_n, text64Data.m_cbk, text64Data.m_encoding);
	}
};


template<>
struct BindDataTrait<zeroblob>
{
	typedef int(*BindFunc)(sqlite3_stmt*, int, zeroblob);
	static BindFunc bindData;
	static int bindZeroBlobData(sqlite3_stmt* stmt, int t, zeroblob zeroBlobData){
		return sqlite3_bind_zeroblob(stmt, t, zeroBlobData.m_n);
	}
};


template<>
struct BindDataTrait<zeroblob64>
{
	typedef int(*BindFunc)(sqlite3_stmt*, int, zeroblob64);
	static BindFunc bindData;
	static int bindZeroBlob64Data(sqlite3_stmt* stmt, int t, zeroblob64 zeroBlob64Data){
		return sqlite3_bind_zeroblob64(stmt, t, zeroBlob64Data.m_n);
	}
};


template<>
struct BindDataTrait<sqlite_ptr>
{
	typedef int(*BindFunc)(sqlite3_stmt*, int, sqlite_ptr);
	static BindFunc bindData;
	static int bindPtrData(sqlite3_stmt* stmt, int t, sqlite_ptr ptrData){
		return sqlite3_bind_pointer(stmt, t, ptrData.m_v, ptrData.m_n, ptrData.m_cbk);
	}
};


template<>
struct BindDataTrait<null_data>
{
	typedef int(*BindFunc)(sqlite3_stmt*, int, null_data);
	static BindFunc bindData;
	static int bindNullData(sqlite3_stmt* stmt, int t, null_data){
		return sqlite3_bind_null(stmt, t);
	}
};

//########################################################################

template<typename T>
int binding(sqlite3_stmt* statement, int r, T t){
	return BindDataTrait<T>::bindData(statement, r+1, std::forward<T>(t));
}

template<typename T, typename... Args>
int binding(sqlite3_stmt* statement, int r, T t, Args ...args){
	if(SQLITE_OK!=BindDataTrait<T>::bindData(statement, r+1, std::forward<T>(t))){
		return SQLITE_ERROR;
	}
	return binding(statement, r+1, std::forward<Args>(args)...);
}

//######################################################################

typedef const char* UTF8;
typedef const void* UTF16;

template<typename B>
class DB_CONNECT
{
	enum {is_valid=false};
};


template<>
struct DB_CONNECT<UTF8>
{
	enum {is_valid=true, 
		 is_utf8=true};
	typedef const char* zSqlPtr;	
	typedef UTF8(*SqliteColumnName)(sqlite3_stmt*, int N);
	static SqliteColumnName sqliteColumnName;
	static int strLength(UTF8 str){
		int n=std::strlen(str);
		if(n==0){
			return 0;
		}
		return n+1;
	}
};


template<>
struct DB_CONNECT<UTF16>
{
	enum {is_valid=true,
		is_utf8=false};
	typedef const void* zSqlPtr;
	typedef UTF16(*SqliteColumnName)(sqlite3_stmt*, int N);
	static SqliteColumnName sqliteColumnName;
	static int strLength(UTF8 str){
		return -1;
	}
};

template<>
struct DB_CONNECT<UTF8*>
{
	enum {is_valid=true,
		is_utf8=false};
};

template<>
struct DB_CONNECT<UTF16*>
{
	enum {is_valid=true,
		is_utf8=false};
};


enum class SQLite_v : bool
{
	v2=true, 
	v3=false
};


template<SQLite_v B>
struct SQLITE3_PREP
{};

template<>
struct SQLITE3_PREP<SQLite_v::v2>
{};

template<>
struct SQLITE3_PREP<SQLite_v::v3>
{};

template<SQLite_v A, typename B>
struct SQLITE3_PREPARE : public SQLITE3_PREP<A>, public DB_CONNECT<B>
{};

template<typename B>
struct SQLITE3_PREPARE<SQLite_v::v2, B>
{};


template<>
struct SQLITE3_PREPARE<SQLite_v::v2, UTF8>
{
	typedef int(*PrepareStatementV)( 
		sqlite3* db,            //* Database handle 
		typename DB_CONNECT<UTF8>::zSqlPtr zSql,       //* SQL statement, UTF-8 encoded 
		int nByte,              //* Maximum length of zSql in bytes. 
		sqlite3_stmt **ppStmt,  //* OUT: Statement handle 
		typename DB_CONNECT<UTF8>::zSqlPtr* pzTail     //* OUT: Pointer to unused portion of zSql 
	);

	static PrepareStatementV prepareStatement;
};


template<>
struct SQLITE3_PREPARE<SQLite_v::v2, UTF16>
{
	typedef int(*PrepareStatementV)( 
		sqlite3* db,            //* Database handle 
		typename DB_CONNECT<UTF16>::zSqlPtr zSql,       //* SQL statement, UTF-8 encoded 
		int nByte,              //* Maximum length of zSql in bytes. 
		sqlite3_stmt **ppStmt,  //* OUT: Statement handle 
		typename DB_CONNECT<UTF16>::zSqlPtr* pzTail     //* OUT: Pointer to unused portion of zSql 
	);

	static PrepareStatementV prepareStatement;
};


template<typename B>
struct SQLITE3_PREPARE<SQLite_v::v3, B>
{};

template<>
struct SQLITE3_PREPARE<SQLite_v::v3, UTF8>
{
	typedef int(*PrepareStatementV)( 
		sqlite3* db,            //* Database handle 
		typename DB_CONNECT<UTF8>::zSqlPtr zSql,       //* SQL statement, UTF-8 encoded 
		int nByte,              //* Maximum length of zSql in bytes. 
		unsigned int prepFlags, //* Zero or more SQLITE3_CONNECTION_ flags 
		sqlite3_stmt **ppStmt,  //* OUT: Statement handle * /
		typename DB_CONNECT<UTF8>::zSqlPtr* pzTail     //* OUT: Pointer to unused portion of zSql 
	);

	static PrepareStatementV prepareStatement;
};


template<>
struct SQLITE3_PREPARE<SQLite_v::v3, UTF16>
{
	typedef int(*PrepareStatementV)( 
		sqlite3* db,            //* Database handle * /
		typename DB_CONNECT<UTF16>::zSqlPtr zSql,       //* SQL statement, UTF-8 encoded * /
		int nByte,              //* Maximum length of zSql in bytes. * /
		unsigned int prepFlags, //* Zero or more SQLITE3_CONNECTION_ flags * /
		sqlite3_stmt **ppStmt,  //* OUT: Statement handle * /
		typename DB_CONNECT<UTF16>::zSqlPtr* pzTail     //* OUT: Pointer to unused portion of zSql * /
	);
	static PrepareStatementV prepareStatement;
};

//======================================================================
//======================================================================
//======================================================================

struct QParams
{
	QParams()=delete;

	explicit QParams(int nbytes)
	:m_pzTail(nullptr),
	m_nByte(nbytes),
	m_prepFlags(0),
	m_isUTF8(false)
	{}

	QParams(int nbytes, bool isUTF8)
	:m_pzTail(nullptr),
	m_nByte(nbytes),
	m_prepFlags(0),
	m_isUTF8(isUTF8)
	{}

	QParams(int nbytes, const char** tail)
	:m_pzTail(tail),
	m_nByte(nbytes),
	m_prepFlags(0),
	m_isUTF8(true)
	{}

	QParams(int nbytes, const void** tail)
	:m_pzTail(reinterpret_cast<const char**>(tail)),
	m_nByte(nbytes),
	m_prepFlags(0),
	m_isUTF8(false)
	{}

	QParams(int nbytes, bool isUTF8, unsigned int prepFlags)
	: m_pzTail(nullptr),
	m_nByte(nbytes),
	m_prepFlags(prepFlags),
	m_isUTF8(isUTF8)
	{};

	QParams(int nbytes, const char** tail, unsigned int prepFlags)
	: m_pzTail(tail),
	m_nByte(nbytes),
	m_prepFlags(prepFlags),
	m_isUTF8(true)
	{};

	QParams(int nbytes, const void** tail, unsigned int prepFlags)
	: m_pzTail(reinterpret_cast<const char**>(tail)),
	m_nByte(nbytes),
	m_prepFlags(prepFlags),
	m_isUTF8(false)
	{};
	
	template<typename UTF>
	typename DB_CONNECT<UTF>::zSqlPtr* getTail(){
		if(!m_pzTail){
			return nullptr;
		}
		return reinterpret_cast<typename DB_CONNECT<UTF>::zSqlPtr*>(m_pzTail);	
	}

	const char** m_pzTail;
	const int m_nByte;
	const unsigned int m_prepFlags;
	bool m_isUTF8;
};


template<typename UTF>
inline int sqlite3Prepare(
	sqlite3* db,
	UTF zSql,
	sqlite3_stmt** ppStmt,
	QParams& qParams
)
{
	if(qParams.m_prepFlags==0){
		return SQLITE3_PREPARE<SQLite_v::v2, UTF>::prepareStatement(db, zSql, qParams.m_nByte, ppStmt, qParams.getTail<UTF>());
	}
	return SQLITE3_PREPARE<SQLite_v::v3, UTF>::prepareStatement(db, zSql, qParams.m_nByte, qParams.m_prepFlags, ppStmt, qParams.getTail<UTF>());
}


template<typename UTF>
int sqlite3Prepare(
	sqlite3* db,
	UTF zSql,
	sqlite3_stmt** ppStmt,
	unsigned int prepFlags
)
{
	if(prepFlags==0){
		return SQLITE3_PREPARE<SQLite_v::v2, UTF>::prepareStatement(db, zSql, DB_CONNECT<UTF>::strLength(zSql), ppStmt, nullptr);
	}
	return SQLITE3_PREPARE<SQLite_v::v3, UTF>::prepareStatement(db, zSql, -1, prepFlags, ppStmt, nullptr);
}

template<typename S>
struct IS_QParam
{
	enum {is_valid=false};
};

template<>
struct IS_QParam<QParams>
{
	enum {is_valid=true};
};

template<>
struct IS_QParam<unsigned int>
{
	enum {is_valid=true};
};


//======================================================================


#endif
