# sqlite3_helper

This project has two elements:

- A wrapper for the SQLite C++ interface.

- A class to read the result of a database query.

Documentation:

Class SQLiteDB

**SQLiteDB::SQLiteDB(const char\* dbName)**

*Description:*

   Constructor, opens a connection to an SQLite database file.

*Parameters:*

   dbName database file name.

**int SQLiteDB::lastErrorCode()**

*Description:*

   Return the error code of the last error ocurred.

**const char* SQLiteDB::lastErrorMsg()**

*Description:*

   Return a description of the last error.


**bool SQLiteDB::executeQuery(const char\* query)**

*Description:*

   Execute a SQL query.

*Parameters:*

   query SQL query statement to be executed.

*Return value:*

   true if the SQL query executes successfully, false otherwise.

**bool SQLiteDB::dataChanged()**

*Description:*

   return true if last query changed the database data;

**void SQLiteDB::applyToRows(const char* query, SqlRowFunc callback)**

*Description:*

   execute a SQL query and apply a callback function on each row of the result. 

*Parameters:*

   query a SQL statement to be executed.
   callback a function of type SqlRowFunc which will be applied to each row of the result.

**bool SQLiteDB::uniqueAsInt(const char\* query, int& defaultValue)**

*Description:*

   execute a SQL statement and return the first column of the first row of the results. The column value return should be of type integer.

*Parameters:*

	@param query: SQL statement
	@param defaultValue: if the query executes and returns any columns, this value will be override the value passed.
	@return bool: true if the executed query return any query, false otherwise

**bool SQLiteDB::uniqueAsDouble(const char\* query, double& returnValue)**

*Description:*

   execute a SQL statement and return the first column of the first row of the results. The column value return should be of type double.

*Parameters:*

	@param query: SQL statement
	@param defaultValue: if the query executes and returns any columns, this value will be override the value passed.
	@return bool: true if the executed query return any query, false otherwise

**bool SQLiteDB::uniqueAsText(const char\* query, std::string& returnValue)**

*Description:*

   execute a SQL statement and return the first column of the first row of the results. The column value return should be of text type.

*Parameters:*

	@param query: SQL statement
	@param defaultValue: if the query executes and returns any columns, this value will be override the value passed.
	@return bool: true if the executed query return any query, false otherwise

**sqlite3_int64 SQLiteDB::lastInsertID()**

*Description:*

   Wrapper for sqlite3_last_insert_rowid

*Parameters:*

	no parameters
	@return sqlite3_int64: the ID of the last inserted value

**int prepareExecuteQuery(const char* query, Args ...args)**

*Description:*

   Binding values to prepared query. Example:

    dbConnection.prepareExecuteQuery("insert into COMPANY values (?,?,?,?,?), (?,?,?,?,?)", 16, "heello6!", 6, "Yes!!", 3.1419, 17, "heello7!", 7, "Yes!!", 3.1420);


**SqlRows SQLiteDB::getRows(const char\* query)**

*Description:*

   Returns a **SqlRows** object with the result of the query (see below).



**class SqlRows**

*Description:*

   

*Parameters:*

statement

   a pointer to a sqlite3_stmt statement handle object that has been used 
   as argument for any of the following routines:

    int sqlite3_prepare(...);
    int sqlite3_prepare_v2(...);
    int sqlite3_prepare_v3(...);
    int sqlite3_prepare16(...);
    int sqlite3_prepare16_v2(...);
    int sqlite3_prepare16_v3(...);

**bool SqlRows::yield()**

*Description:*

   Calling yield will retrieve the next row in the result. 

*Parameters:*

   no parameters

*Return value:*

   bool, it returns true if there is a new row, false when there is not more rows in the result.

**int SqlRows::AS_INT(const char\* columnName)**

*Description:*

   Retrieves the value of the column columnName in the current row of the result.

*Parameters:*

columnName
   const char* the name of the column
   
*Return value:*

   int 

**double SqlRows::AS_DOUBLE(const char\* columnName)**

*Description:*

   Retrieves the value of the column columnName in the current row of the result.

*Parameters:*

columnName
   const char* the name of the column
   
*Return value:*

   double

**const unsigned char\* SqlRows::AS_TEXT(const char\* field)**

*Description:*

   Retrieves the value of the column columnName in the current row of the result.

*Parameters:*

columnName
   const char* the name of the column
   
*Return value:*

   const unsigned char\*
   
**Example**

    SQLiteDB dbConnection("Test.db");
	
    std::string query("select ID, Name, Salary from COMPANY where ID<'20'");

    SqlRow rows=dbConnection.getRows(query.c_str());
    while(rows.yield()){
       std::cout<<"ID: "<<rows.AS_INT("ID")<<" | Name: "<<rows.AS_TEXT("Name")<<" | Salary:"<<rows.AS_DOUBLE("Salary")<<"\n";
    }

**template<typename T>\
T DATA_AS(const char\* field)**

Template as above.

