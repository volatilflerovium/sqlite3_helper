#include <iostream>
#include <stdio.h>
#include <ctime>
#include <stdlib.h>
#include <vector>
#include <sqlite3.h> 

#include "sqlite_db.h"

int main() {

	SQLiteDB dbConnection("Test.db");

	/*
	ID INT 
	Name varchar 
	Age INT
	Address varchar, 
	Salary real 
	*/
	std::string query("");
	query="select ID, Name, Salary from COMPANY where ID<'20'";

	dbConnection.applyToRows(query.c_str(), [](SqlRows& row){
		std::cout<<"ID: "<<row.AS_INT("ID")<<" | Name: "<<row.AS_TEXT("Name")<<" | Salary:"<<row.DATA_AS<double>("Salary")<<"\n";
	});


	SqlRows rows=dbConnection.getRows(query.c_str());
	while(rows.yield()){
		std::cout<<"ID: "<<rows.AS_INT("ID")<<" | Name: "<<rows.AS_TEXT("Name")<<" | Salary:"<<rows.DATA_AS<double>("Salary")<<"\n";
	}

	std::string resultName("");
	if(dbConnection.uniqueAsText("select Name from COMPANY where ID='3'", resultName)){
		std::cout<<"Name: "<<resultName<<"\n";
	}

	int resultID;
	if(dbConnection.uniqueAsInt("select ID from COMPANY where ID='2'", resultID)){
		std::cout<<"ID: "<<resultID<<"\n";
	}
	

	return 0;
}

