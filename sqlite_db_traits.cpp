/*********************************************************************
* TypeToFunction traits                                              *
*                                                                    *
* Version: 1.0                                                       *
* Date:    23-01-2021                                                *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/
#include "sqlite_db_traits.h"

const int TypeToFunction<int>::defaultValue(0);
TypeToFunction<int>::ColFunc TypeToFunction<int>::getColumnData = &sqlite3_column_int;
TypeToFunction<int>::BindFunc TypeToFunction<int>::bindData = &sqlite3_bind_int;

const double TypeToFunction<double>::defaultValue(0);
TypeToFunction<double>::ColFunc TypeToFunction<double>::getColumnData = &sqlite3_column_double;
TypeToFunction<double>::BindFunc TypeToFunction<double>::bindData = &sqlite3_bind_double;

const int TypeToFunction<sqlite3_int64>::defaultValue(0);
TypeToFunction<sqlite3_int64>::ColFunc TypeToFunction<sqlite3_int64>::getColumnData = &sqlite3_column_int64;
TypeToFunction<sqlite3_int64>::BindFunc TypeToFunction<sqlite3_int64>::bindData = &sqlite3_bind_int64;

const int TypeToFunction<sqlite3_value*>::defaultValue(0);
TypeToFunction<sqlite3_value*>::ColFunc TypeToFunction<sqlite3_value*>::getColumnData = &sqlite3_column_value;
TypeToFunction<sqlite3_value*>::BindFunc TypeToFunction<sqlite3_value*>::bindData = &sqlite3_bind_value;

std::string TypeToFunction<std::string>::defaultValue("");
TypeToFunction<std::string>::ColFunc TypeToFunction<std::string>::getColumnData = &TypeToFunction<std::string>::columnToString;
TypeToFunction<std::string>::BindFunc TypeToFunction<std::string>::bindData = &sqlite3_bind_text;

const int TypeToFunction<const void*>::defaultValue(0);
TypeToFunction<const void*>::ColFunc TypeToFunction<const void*>::getColumnData = &sqlite3_column_blob;
TypeToFunction<const void*>::BindFunc TypeToFunction<const void*>::bindData = &sqlite3_bind_blob;
