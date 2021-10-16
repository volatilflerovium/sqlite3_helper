/*********************************************************************
* TypeToFunction traits                                              *
*                                                                    *
* Version: 2.0                                                       *
* Date:    16-10-2021                                                *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/
#include "sqlite_db_traits.h"

ColumnData<int>::ColFunc ColumnData<int>::getColumnData= &sqlite3_column_int;
ColumnData<double>::ColFunc ColumnData<double>::getColumnData= &sqlite3_column_double;
ColumnData<sqlite3_int64>::ColFunc ColumnData<sqlite3_int64>::getColumnData= &sqlite3_column_int64;
ColumnData<sqlite3_value*>::ColFunc ColumnData<sqlite3_value*>::getColumnData = &sqlite3_column_value;
ColumnData<std::string>::ColFunc ColumnData<std::string>::getColumnData = &ColumnData<std::string>::columnToString;
ColumnData<const void*>::ColFunc ColumnData<const void*>::getColumnData= &sqlite3_column_blob;
ColumnData<blob>::ColFunc ColumnData<blob>::getColumnData= &sqlite3_column_blob;
ColumnData<text>::ColFunc ColumnData<text>::getColumnData= &sqlite3_column_text;
ColumnData<text16>::ColFunc ColumnData<text16>::getColumnData= &sqlite3_column_text16;
ColumnData<bytes>::ColFunc ColumnData<bytes>::getColumnData= &sqlite3_column_bytes;
ColumnData<bytes16>::ColFunc ColumnData<bytes16>::getColumnData= &sqlite3_column_bytes16;
ColumnData<sqlite_type>::ColFunc ColumnData<sqlite_type>::getColumnData= &sqlite3_column_type;

//======================================================================

BindDataTrait<int>::BindFunc BindDataTrait<int>::bindData = &sqlite3_bind_int;
BindDataTrait<double>::BindFunc BindDataTrait<double>::bindData = &sqlite3_bind_double;
BindDataTrait<sqlite3_int64>::BindFunc BindDataTrait<sqlite3_int64>::bindData = &sqlite3_bind_int64;
BindDataTrait<sqlite3_value*>::BindFunc BindDataTrait<sqlite3_value*>::bindData = &sqlite3_bind_value;
BindDataTrait<std::string>::BindFunc BindDataTrait<std::string>::bindData = &BindDataTrait<std::string>::bindStringData;
BindDataTrait<const char*>::BindFunc BindDataTrait<const char*>::bindData = &BindDataTrait<const char*>::bindCstringData;
BindDataTrait<blob>::BindFunc BindDataTrait<blob>::bindData= &BindDataTrait<blob>::bindBlobData;
BindDataTrait<blob64>::BindFunc BindDataTrait<blob64>::bindData= &BindDataTrait<blob64>::bindBlob64Data;
BindDataTrait<text>::BindFunc BindDataTrait<text>::bindData= &BindDataTrait<text>::bindTextData; 
BindDataTrait<text16>::BindFunc BindDataTrait<text16>::bindData= &BindDataTrait<text16>::bindText16Data;
BindDataTrait<text64>::BindFunc BindDataTrait<text64>::bindData= &BindDataTrait<text64>::bindText64Data;
BindDataTrait<zeroblob>::BindFunc BindDataTrait<zeroblob>::bindData= &BindDataTrait<zeroblob>::bindZeroBlobData;
BindDataTrait<zeroblob64>::BindFunc BindDataTrait<zeroblob64>::bindData= &BindDataTrait<zeroblob64>::bindZeroBlob64Data;
BindDataTrait<sqlite_ptr>::BindFunc BindDataTrait<sqlite_ptr>::bindData= &BindDataTrait<sqlite_ptr>::bindPtrData;
BindDataTrait<null_data>::BindFunc BindDataTrait<null_data>::bindData= &BindDataTrait<null_data>::bindNullData; 

//======================================================================

DB_CONNECT<UTF8>::SqliteColumnName DB_CONNECT<UTF8>::sqliteColumnName=&sqlite3_column_name;
DB_CONNECT<UTF16>::SqliteColumnName DB_CONNECT<UTF16>::sqliteColumnName=&sqlite3_column_name16;

//======================================================================

SQLITE3_PREPARE<SQLite_v::v2, UTF8>::PrepareStatementV SQLITE3_PREPARE<SQLite_v::v2, UTF8>::prepareStatement= &sqlite3_prepare_v2;
SQLITE3_PREPARE<SQLite_v::v2, UTF16>::PrepareStatementV SQLITE3_PREPARE<SQLite_v::v2, UTF16>::prepareStatement= &sqlite3_prepare16_v2;

SQLITE3_PREPARE<SQLite_v::v3, UTF8>::PrepareStatementV SQLITE3_PREPARE<SQLite_v::v3, UTF8>::prepareStatement= &sqlite3_prepare_v3;
SQLITE3_PREPARE<SQLite_v::v3, UTF16>::PrepareStatementV SQLITE3_PREPARE<SQLite_v::v3, UTF16>::prepareStatement= &sqlite3_prepare16_v3;
