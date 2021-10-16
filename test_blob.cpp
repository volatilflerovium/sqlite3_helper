#include <iostream>
#include <stdio.h>
#include <ctime>
#include <stdlib.h>
#include <vector>
#include <codecvt>
#include <locale>
#include <string>
#include "sqlite_db.h"

#include <fstream>

#include <cassert>
//######################################################################

/*
COMPANY table schema:

"ID INT PRIMARY KEY     NOT NULL," \
"Name           TEXT    NOT NULL," \
"Age           INT     NOT NULL," \
"Address        CHAR(50)," \
"Salary         REAL,"\
"utf16         TEXT,"\
"Data         BLOB NULL,"\
"Ħφ             int";
*/

//######################################################################

int main() {

	SQLiteDB dbConnection("database_test.db");

	std::cout<<"* * * * * * * Example 1* * * * * * *\n";

	const char* query="select ID, Name, Ħφ, Salary from COMPANY where ID<'20' AND ID>'15'";

	dbConnection.applyToRows(query, [](SqlRows& row){
		std::cout<<"ID: " << row.as_int("ID")<<" | Name: " << row.as_text("Name") << " | Salary:" << row.data_as<double>("Salary") << " | Ħφ: " << row.data_as<int>("Ħφ") <<"\n";
	});

	if(dbConnection.lastErrorCode()>0){
		std::cout<<dbConnection.lastErrorMsg()<< " " << query<<"\n";
	}

	std::cout<<"\n* * * * * * * Example 2* * * * * * *\n";

	const char* pictureIn="tree.jpg";
	const char* pictureOut="tree_output.jpg";
	std::ifstream infile(pictureIn, std::ios::in|std::ios::binary|std::ios::ate);
	std::ofstream outfile(pictureOut, std::ios::out|std::ios::binary);

	if (infile.is_open()){
		int size = infile.tellg();
		char* memblock = new char[size];

		infile.seekg(0, std::ios::beg); 
		infile.read(memblock, size); 
		infile.close(); 

		SqlRows rows2=dbConnection.executeSecureQuery(0, "update COMPANY set Data=? where ID=?", blob(reinterpret_cast<const void*>(memblock), size, SQLITE_TRANSIENT), 5);
		if(dbConnection.dataChanged()){
			std::cout<<"Record updated!\n";

			const char* query2="select ID, Name, utf16, Data from COMPANY where ID='5'";

			SqlRows rows=dbConnection.getResultRows(query2);
			while(rows.yield()){
				std::cout<<"ID: "<<rows.as_int("ID")<<" | Name: "<<rows.as_text("Name")<<" | utf16: "<<reinterpret_cast<const char*>(rows.data_as<text>("utf16"))<<"\n";
				std::cout<<"size: "<<rows.data_as<bytes>("Data")<<"\n";
				int fileSize=rows.data_as<bytes>("Data");
				outfile.write(static_cast<const char*>(rows.data_as<blob>("Data")), fileSize);
				outfile.close();
			}
		}
		else{
			std::cout<<dbConnection.lastErrorMsg()<<"?\n";
		}
		delete[] memblock;
	}

	std::cout<<"\n* * * * * * * Example 3* * * * * * *\n";
	const char* query2="select ID, Name, utf16 from COMPANY where ID>'20' and ID<'25'";

	SqlRows rows=dbConnection.getResultRows(query2);
	while(rows.yield()){
		std::cout<<"ID: "<<rows.as_int("ID")<<" | Name: "<<rows.as_text("Name")<<" | utf16: "<<reinterpret_cast<const char*>(rows.data_as<const void*>("utf16"))<<"\n";
	}
	std::cout<<"reset rows\n";
	rows.reset();
	while(rows.yield()){
		std::cout<<"ID: "<<rows.as_int("ID")<<" | Name: "<<rows.as_text("Name")<<" | utf16: "<<reinterpret_cast<const char*>(rows.data_as<const void*>("utf16"))<<"\n";
	}

	std::cout<<"\n* * * * * * * Example 4* * * * * * *\n";
	std::string resultName("");
	if(dbConnection.uniqueAsString("select Name from COMPANY where ID='3'", resultName)){
		std::cout<<"Name: "<<resultName<<"\n";
	}

	std::cout<<"\n* * * * * * * Example 5* * * * * * *\n";
	int resultID;
	if(dbConnection.uniqueAsInt("select ID from COMPANY where ID='2' and Name='bjrQhWwnhbsmT2qvQYVBe9RmXwQnWUv1'", resultID)){
		std::cout<<"ID: "<<resultID<<"\n";
	}
	else{
		std::cout<<"No record found.\n";
	}

	std::cout<<"\n* * * * * * * Example 6* * * * * * *\n";
	
	SqlRows rows2=dbConnection.executeSecureQueryNf("select ID, Name from COMPANY where ID=? and Name=?", 3, "wmMV52HO");
	if(dbConnection.lastErrorCode()==0){
		while(rows2.yield()){
			std::cout<<"ID: "<<rows2.as_int("ID")<<" | Name: "<<rows2.as_text("Name")<<"\n";
		}
	}
	else{
		std::cout<<dbConnection.lastErrorMsg()<<"?\n";
	}
	
	std::cout<<"\n* * * * * * * Example 7* * * * * * *\n";
	const char* query3="select Name from COMPANY where ID>'30' and ID<'32';select Name from COMPANY where ID>'33' and ID<'35';";

	const char* query5;
	QParams(51);
	SqlRows rows3=dbConnection.getResultRows(query3, QParams(51, &query5));
	while(rows3.yield()){
		std::cout<<"Name: "<<rows3.as_text("Name")<<"\n";
	}

	std::cout<<"Tail: "<<query5<<"\n";
	


	
	return 0;
}

