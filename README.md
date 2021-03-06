# sqlite3_helper
A C++ class to query SQLITE3 databases

Simple C++ classes to query Sqlite3 databases.

Documentation:

**SqlRows::SqlRows(sqlite3_stmt\* statement)**

Constructor, SqlRows is a wrapper for sqlite3_stmt*

**bool SqlRows::yield()**

Calling yield will retrieve the next row in the result. Return true
if there is a new row, false when there is not more rows in the result.

For the next methods let's consider the following example:

    SQLiteDB dbConnection("Test.db");
	
    std::string query("select ID, Name, Salary from COMPANY where ID<'20'");

    SqlRow rows=dbConnection.getRows(query.c_str());
    while(rows.yield()){
       std::cout<<"ID: "<<rows.AS_INT("ID")<<" | Name: "<<rows.AS_TEXT("Name")<<" | Salary:"<<rows.AS_DOUBLE("Salary")<<"\n";
    }

**int SqlRows::AS_INT(const char\* column_name)**

**double SqlRows::AS_DOUBLE(const char\* field)**

**const unsigned char\* SqlRows::AS_TEXT(const char\* field)**

These methods get the specific column name in the current row of the result as int,
double and const unsigned char* respectively.

**template<typename T>
	T DATA_AS(const char* field)**

Template as above.

Class SQLiteDB

**SQLiteDB::SQLiteDB(const char\* database_name)**

Constructor; 

**int SQLiteDB::lastErrorCode()**

**const char* SQLiteDB::lastErrorMsg()**

**template<typename Func>
void SQLiteDB::applyToRows(const char* query, Func callback)**

Apply the callback function on each row of the result from the query. Therefore
the callback should take one parameter and return void.

**SqlRows SQLiteDB::getRows(const char\* query)**

Returns a **SqlRows** object with the result of the query.

**bool SQLiteDB::executeQuery(const char\* query)**

Return true if the query is successful false otherwise.

**template<typename Func>
bool SQLiteDB::executeQuery(const char* query, Func callback)**

**bool SQLiteDB::dataChanged()**

Return true if last query changed the database data;
	
**bool SQLiteDB::uniqueAsInt(const char\* query, int& returnValue)**

**bool SQLiteDB::uniqueAsDouble(const char\* query, double& returnValue)**

**bool SQLiteDB::uniqueAsText(const char\* query, std::string& returnValue)**

These methods will return true if the query was successful and a value was found,
it returns false otherwise. Notice that the return column in the query should
be the first in the list.

**sqlite3_int64 SQLiteDB::lastInsertID()**

Wrapper for sqlite3_last_insert_rowid


**int prepareQuery(const char* query, Args ...args)**

Binding values to prepared query. Example:

    dbConnection.prepareQuery("insert into COMPANY values (?,?,?,?,?), (?,?,?,?,?)", 16, "heello6!", 6, "Yes!!", 3.1419, 17, "heello7!", 7, "Yes!!", 3.1420);


