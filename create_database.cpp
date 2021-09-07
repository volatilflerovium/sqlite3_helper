#include <iostream>
#include <stdio.h>
#include <ctime>
#include <stdlib.h>
#include <vector>
#include <sqlite3.h> 

#include "sqlite_db.h"

//g++ -std=c++11 test.cpp -o test -l sqlite3

std::string randomStr(int maxLength){
	static std::string pool("abcdefghijklmnopqrstuvwxyzABCDEFGHIJLMNOPQRSTUVWXYZ1234567890");
	std::string result("");
	int k=rand()%maxLength+1;
	for(int i=0; i<k; i++){
		result+=pool[rand()%pool.length()];
	}
	return result;
}

int main() {
	srand(time(0));
	
	SQLiteDB  dbConnection("Test.db");

	std::string query("");
	query="CREATE TABLE COMPANY("  \
      "ID INT PRIMARY KEY     NOT NULL," \
      "Name           TEXT    NOT NULL," \
      "Age           INT     NOT NULL," \
      "Address        CHAR(50)," \
      "Salary         REAL );";
	
	if(!dbConnection.executeQuery(query.c_str())){
			std::cout<<"Fail: "<<"\n";
	}

	for(int i=0; i<10; i++){
		query="insert into COMPANY values ('";
		query+=std::to_string(i)+"',";        // ID
		query+="'"+randomStr(5+rand()%51)+"',";// Name
		query+="'"+std::to_string(5+rand()%53)+"',"; //Age
		query+="'"+randomStr(5+rand()%61)+"',";   //Address
		query+="'"+std::to_string(5.27+rand()%100)+"')";   //Salary
		
		if(!dbConnection.executeQuery(query.c_str())){
			std::cout<<"Fail: "<<i<<"\n";
			break;
		}
	}

	return 0;
}
