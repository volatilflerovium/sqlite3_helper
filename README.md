# sqlite3_helper

A C++ Class wrapper for SQLite C++ Interface.

The purpose is to simplifies quering a database using the [SQLite C++ Interface](http://www.sqlite.org/)
by selecting the appropiate version to use and parameter to pass.

This project has two elements:

- SQLiteDB: a classs to wrap the [SQLite](http://www.sqlite.org/) C++ interface.

- SqlRows: a class lass to iterate through the rows in the result of a SQL query.

# Content
 
- [Features](#features)
- [Quick Start](#quick-start)
- [Documentation](#documentation)
   - [SQLiteDB](#sqlitedb)
      - [QParams](#qparams)
      - [BindData](#binddata)
   - [SqlRows](#sqlrows)
- [License](#license)

# Features

- Automatic selection of sqlite3_prepare_v2, sqlite3_prepare16_v2, sqlite3_prepare_v3, sqlite3_prepare16_v3

- Set specific parameters as needed.

- Get value of a column/field in the result by name.

# Quick Start

```
    SQLiteDB dbConnection("my_database_file.db");
    const char* query2="select ID, Name, utf16 from COMPANY where ID>'20' and ID<'25'";

    SqlRows rows=dbConnection.getResultRows(query2);
    while(rows.yield()){
       std::cout<<"ID: "<<rows.as_int("ID")<<" | Name: "<<rows.as_text("Name")<<" | utf16: "<<reinterpret_cast<const char*>(rows.data_as<const void*>("utf16"))<<"\n";
    }

    SqlRows rows2=dbConnection.executeSecureQueryNf("select ID, Name from COMPANY where ID=? and Name=?", 3, "wmMV52HO");
    if(dbConnection.lastErrorCode()==0){
       while(rows2.yield()){
	       std::cout<<"ID: "<<rows2.as_int("ID")<<" | Name: "<<rows2.as_text("Name")<<"\n";
       }
    }
```


# Documentation:

The simplest way to execute SQL queries can be done using one of the 
follwoing methods:
```
    template<typename UTF>
    bool SQLiteDB::executeQuery(UTF query);

    template<typename UTF>
    SqlRows SQLiteDB::getResultRows(UTF query);

    template<typename UTF>
    void SQLiteDB::applyToRows(UTF query, SqlRowFunc callback);
```
Where query is a SQL statement UTF-8 or UTF-16 encoded (const char* or const void* 
respectively), therefore the template parameter is used to choose between 
sqlite3_prepare_v2 or sqlite3_prepare16_v2 (sqlite3_prepare_v3 or sqlite3_prepare16_v3).

These methods have overloaded versions, accepting an extra parameter:
- unsigned int or
- QParam qParam. 
Thus we can do:
```
    getResultRows("some query");
```
or
```
    getResultRows("some query", prepFlags);// where prepFlags is a valid flag for sqlite3_prepare_v3 (or sqlite3_prepare16_v3)
```
or
```
    getResultRows("some query", QParam(bytes, tail, flags);
```

# QParams

The easy way to describe QParam structure is as follows:
observe the signature for sqlite3_prepare_v2 and sqlite3_prepare_v3
```
    int sqlite3_prepare_v2 // or sqlite3_prepare16_v2
    (
     sqlite3\*, UTF, int nByte, sqlite3_stmt**, UTF \*pzTail
    );

    int sqlite3_prepare_v3 // or sqlite3_prepare16_v3
    (
     sqlite3\*, UTF, int nByte, unsigned int prepFlags, sqlite3_stmt\*\*, UTF \*pzTail
    );
```
The named parameters: nBytes, pzTail and prepFlags in sqlite3_prepare_v2 (or sqlite3_prepare16_v2)
and in qlite3_prepare_v3 (or sqlite3_prepare16_v3) can be wrapped using
QParams.

```
    QParams(int nBytes) // will use for sqlite3_prepare_v2 or sqlite3_prepare16_v2

    QParams(int nBytes, const char** pzTail) // will use sqlite3_prepare_v2

    QParams(int nBytes, const void** pzTail) // will use sqlite3_prepare16_v2

    QParam(int nBytes, unsigned int prepFlags), // will use sqlite3_prepare_v3 or sqlite3_prepare16_v3

    QParams(int nBytes, unsigded int prepFlags, const char** pzTail). // will use sqlite3_prepare_v3

    QParams(int nBytes, unsigded int prepFlags, const void** pzTail). // will use sqlite3_prepare16_v3
````
Where, nBytes, prepFlags and \*\*pzTail have the same meaning describe in [prepare statement.](https://www3.sqlite.org/c3ref/prepare.html)

Example:

```
    bool SQLiteDB::executeQuery("some sql query", QParams(123, tail)); // or

    const char* tail;
    SqlRows SQLiteDB::getResultRows("some query", QParam(123, SQLITE_PREPARE_NO_VTAB, &tail));

    void SQLiteDB::applyToRows("some query", [](SqlRows& row){
       //do something
    }, QParam(123));
```

Notice that SQLiteDB will use sqlite3_prepare_v2 or sqlite3_prepare16_v2
base on the type of the query passed (const char* or const void) respectively
the same apply to sqlite3_prepare_v3 or sqlite3_prepare16_v3. 

A more interesting situation arises for execute secure SQL statements 
(bind values to statements). For example:
```
    "SELECT * from Users where userID=? AND password=?"

    "update COMPANY set Data=? where ID=?"

    "insert into COMPANY values (?,?,?,?,?), (?,?,?,?,?)";
```
In these situations we can use:
```
    template<typename UTF, typename... Args>
    SqlRows executeSecureQuery(QParams qParams, UTF query, Args ...args);
```
Thus we can do:
```
    dbConnection.executeSecureQuery(qParams, "insert into COMPANY values (?,?,?,?,?), (?,?,?,?,?)", 16, "heello6!", 6, "Yes!!", 3.1419, 17, "heello7!", 7, "Yes!!", 3.1420);

    dbConnection.executeSecureQuery(0, "select ID, Name from COMPANY where ID=? and Name=?", 3, "wmMV52HO");
```
In general we can write:
```
    dbConnection.executeSecureQuery(qParams, "insert into COMPANY values (?,?,?,?,?)", valType1, valType2, ..., valTypeN);
```
The following types can be bind straight forward:

- int
- double
- sqlite3_int64
- const char*
- std::string
- sqlite3_value*

However in order to bind any of the follwing types

- zeroblob as in sqlite3_bind_zeroblob
- zeroblob64 as in sqlite3_bind_zeroblob64
- text as in sqlite3_bind_text
- text16 as in sqlite3_bind_text16
- text64 as in sqlite3_bind_text64
- blob as in sqlite3_bind_blob
- blob64 as in sqlite3_bind_blob64
- pointer as in sqlite3_bind_pointer
- null as in sqlite3_bind_null

we shold use specific structures to encapsulate some of the parameters
needed by respective sqlite3 binding routine. Thus to bind:

- zeroblob we use: zeroblob(int n)

- zeroblob64 we use zeroblob64(sqlite3_uint64 n)

- text use text(const char* v, int n, fn cbk);

- text16 use text16(const void* v, int n, fn cbk)

- text64 use text64(const char* v, sqlite3_uint64 n, fn cbk, unsigned char encoding)

- blob use blob(const void* v, int n, fn cbk)

- blob64 use blob64(const void* v, sqlite3_uint64 n, fn cbk)

- pointer use sqlite_ptr(void* v, const char* ch, fn cbk)

- null use null_data

So the obvious question is what is fn cbk in the parameter list of most 
of these structures? This is the pointer to function parameter in
```
    sqlite3_bind_XXXX(sqlite3_stmt*, int, const void*, int, void(*)(void*), ...);
```
as in the documentation https://www3.sqlite.org/c3ref/bind_blob.html

Example:
```
    dbConnection.executeSecureQuery(0, "update COMPANY set Data=? where ID=?", blob(reinterpret_cast<const void*>(memblock), size, SQLITE_TRANSIENT), 5);
```

# SqlRows

Another element of SQLiteDB is SqlRows, a class to iterate through 
the rows in the result of a prepare statement. At the core of it is
a template method SqlRows::data_as<typename>. This method returns
the value in the column in the current row. The types of the values
to be return can be:

- blob as in sqlite3_column_blob
- double as in sqlite3_column_double
- int as int sqlite3_column_int
- sqlite3_int64 as in sqlite3_column_int64
- text as in sqlite3_column_text
- text16 as in sqlite3_column_text16
- sqlite3_value* as in  sqlite3_column_value
- bytes as in sqlite3_column_bytes
- bytes16 as in sqlite3_column_bytes16
- sqlite_type as in sqlite3_column_type

Therefore we can do:
```
    SqlRows::data_as<blob>
    SqlRows::data_as<double>
    SqlRows::data_as<int>
    SqlRows::data_as<sqlite3_int64>
    SqlRows::data_as<text>
    SqlRows::data_as<text16>
    SqlRows::data_as<sqlite3_value*>
    SqlRows::data_as<bytes>
    SqlRows::data_as<bytes16>
    SqlRows::data_as<sqlite_type>
```
respectively. Example:
```
    const char* query2="select ID, Name, utf16, Data from COMPANY where ID='5'";

    SqlRows rows=dbConnection.getResultRows(query2);
    while(rows.yield()){
        std::cout<<"ID: "<<rows.as_int("ID")<<\
            " | Name: "<<rows.data_as<std::string>("Name")<<\
            " | utf16: "<<reinterpret_cast<const char*>(rows.data_as<text>("utf16"))<<"\n";

        int fileSize=rows.data_as<bytes>("Data");
        outfile.write(static_cast<const char*>(rows.data_as<blob>("Data")), fileSize);
        outfile.close();
    }
```




