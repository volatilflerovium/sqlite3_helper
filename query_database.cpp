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
	std::string query("select ID, Name, Salary from COMPANY where ID<'20'");

	dbConnection.applyToRows(query.c_str(), [](SqlRows& row){
		std::cout<<"ID: "<<row.AS_INT("ID")<<" | Name: "<<row.AS_TEXT("Name")<<" | Salary:"<<row.DATA_AS<double>("Salary")<<"\n";
	});


	SqlRows rows=dbConnection.getResultRows(query.c_str());
	while(rows.yield()){
		std::cout<<"ID: "<<rows.AS_INT("ID")<<" | Name: "<<rows.AS_TEXT("Name")<<" | Salary:"<<rows.DATA_AS<double>("Salary")<<"\n";
	}

	std::string resultName("");
	if(dbConnection.uniqueAsText("select Name from COMPANY where ID='3'", resultName)){
		std::cout<<"Name: "<<resultName<<"\n";
	}

	int resultID;
	if(dbConnection.uniqueAsInt("select ID from COMPANY where ID='2' and Name='DVf'", resultID)){
		std::cout<<"ID: "<<resultID<<"\n";
	}
	else{
		std::cout<<dbConnection.lastErrorMsg()<<"\n";
	}


	SqlRows rows2=dbConnection.executeSecureQuery("select ID, Name from COMPANY where ID<? and Name=?", 4, "DVf");
	if(dbConnection.lastErrorCode()==0){
		while(rows2.yield()){
			std::cout<<"ID: "<<rows2.AS_INT("ID")<<" | Name: "<<rows2.AS_TEXT("Name")<<"\n";
		}
	}
	else{
		std::cout<<dbConnection.lastErrorMsg()<<"\n";
	}

	return 0;
}

