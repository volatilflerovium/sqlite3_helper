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

	@param dbName: database file name.
	@throws const char* thrown if it is not possible to open the connection to the database.  

**int SQLiteDB::lastErrorCode()**

*Description:*

   Return the error code of the last error ocurred (see)[http://www.sqlite.org/c3ref/errcode.html]

**const char* SQLiteDB::lastErrorMsg()**

*Description:*

   Return a description of the last error (see)[http://www.sqlite.org/c3ref/errcode.html].

**bool SQLiteDB::executeQuery(const char\* query)**

*Description:*

   Execute a SQL query.

*Parameters:*

    @param query a SQL statement to be executed.
    @return true on success, false otherwise

**bool SQLiteDB::dataChanged()**

*Description:*

   Return true if last query changed the database data.

**void SQLiteDB::applyToRows(const char* query, SqlRowFunc callback)**

*Description:*

   Execute a SQL query and apply a callback function on each row of the result. 

*Parameters:*

   @param query a SQL statement to be executed.
   @param callback a function of SqlRowFunc type which will be applied to each row of the result.
      (see below for SqlRowFunc type)

**bool SQLiteDB::uniqueAsInt(const char\* query, int& resultValue)**

*Description:*

   Execute a SQL query and get the value of the first column of the first row of the results.

*Parameters:*

	@param query a SQL query
	@param[out] resultValue if the query executes and returns any columns, this value will override the passed value
	@return bool true if the query executes succefully and the result has at least one row, false otherwise.

**bool SQLiteDB::uniqueAsDouble(const char\* query, double& resultValue)**

*Description:*

   Execute a SQL query and get the value of the first column of the first row of the results.

*Parameters:*

	@param query a SQL query
	@param[out] resultValue if the query executes and returns any columns, this value will override the passed value
	@return bool true if the query executes succefully and the result has at least one row, false otherwise.

**bool SQLiteDB::uniqueAsText(const char\* query, std::string& resultValue)**

*Description:*

   Execute a SQL query and get the value of the first column of the first row of the results.

*Parameters:*

	@param query a SQL query
	@param[out] resultValue if the query executes and returns any columns, this value will override the passed value
	@return bool true if the query executes succefully and the result has at least one row, false otherwise.

**sqlite3_int64 SQLiteDB::lastInsertID()**

*Description:*

   Wrapper for (sqlite3_last_insert_rowid)[http://www.sqlite.org/c3ref/last_insert_rowid.html]

**template<typename... Args>\
int executeSecureQuery(const char* query, Args ...args)**

*Description:*

   Bind values to prepared SQL statement and execute the prepared query. 
   Example:

    SqlRows rows=dbConnection.executeSecureQuery("select ID, Name from COMPANY where ID<? and Name=?", 4, "DVf");

	 // inserting a new record
    dbConnection.executeSecureQuery("insert into COMPANY values (?,?,?,?,?)", 16, "heello6!", 6, "Yes!!", 3.1419);

    // inserting multiple records at once
    dbConnection.executeSecudeQuery("insert into COMPANY values (?,?,?,?,?), (?,?,?,?,?)", 16, "heello6!", 6, "Yes!!", 3.1419, 17, "heello7!", 7, "Yes!!", 3.1420);

*Parameters:*

   @param query a SQL statement template with parameters '?' 
   @tparam Args variadic number of arguments one for each unspecified parameter '?' and in the same order as they will be applied.
   @return SqlRows object for the rows of the result.

**SqlRows SQLiteDB::getResultRows(const char\* query)**

*Description:*

	Execute a SQL query.

*Parameters:*

   @param query a SQL statement
   @return a SqlRows object for the rows of the result of the query.

**class SqlRows**

*Description:*

   Objects of this class hold a pointer to a preparared sqlite3_stmt statement.

**bool SqlRows::yield()**

*Description:*

   Step into the next row in the result. 

*Parameters:*

   no parameters
   @return true if there is a new row, false when there is not more rows in the result.

**int SqlRows::AS_INT(const char\* columnName)**

*Description:*

   Retrieves the value of a specific column in the current row of the result.

*Parameters:*

   @param columnName the name of a spacific column, the type of the column must be integer
   @return the value of column in the current row.
   
**double SqlRows::AS_DOUBLE(const char\* columnName)**

*Description:*

   Retrieves the value of a specific column in the current row of the result.

*Parameters:*

   @param columnName the name of a spacific column, the type of the column must be double
   @return the value of column in the current row.
   

**const unsigned char\* SqlRows::AS_TEXT(const char\* field)**

*Description:*

   Retrieves the value of a specific column in the current row of the result.

*Parameters:*

   @param columnName the name of a spacific column, the type of the column must be text
   @return the value of column in the current row.
   
**Example**

    SQLiteDB dbConnection("Test.db");
	
    std::string query("select ID, Name, Salary from COMPANY where ID<'20'");

    SqlRow rows=dbConnection.getRows(query.c_str());
    while(rows.yield()){
       std::cout<<"ID: "<<rows.AS_INT("ID")<<" | Name: "<<rows.AS_TEXT("Name")<<" | Salary:"<<rows.AS_DOUBLE("Salary")<<"\n";
    }


