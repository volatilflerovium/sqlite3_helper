#include <iostream>
#include <stdio.h>
#include <ctime>
#include <stdlib.h>
#include <vector>
#include <sqlite3.h> 

#include "sqlite_db.h"

//g++ -std=c++11 create_database.cpp -o test -l sqlite3

std::string randomStr(int maxLength){
	static std::string pool("abcdefghijklmnopqrstuvwxyzABCDEFGHIJLMNOPQRSTUVWXYZ1234567890");
	std::string result("");
	int k=rand()%maxLength+1;
	for(int i=0; i<k; i++){
		result+=pool[rand()%pool.length()];
	}
	return result;
}

void mkDBfile(const char* filePath){
	std::ifstream file;
	file.open(filePath);
	if(!file.is_open()){
		std::ofstream outfile(dataBaseFile, std::ios::out);
	}
	file.close();
}

//######################################################################

int main() {
	srand(time(0));
	const char* dataBaseFile="Test_blob2.db";

	mkDBfile(dataBaseFile);

	SQLiteDB  dbConnection(dataBaseFile);

	std::string query("");
	query="CREATE TABLE COMPANY("  \
      "ID INT PRIMARY KEY     NOT NULL," \
      "Name           TEXT    NOT NULL," \
      "Age           INT     NOT NULL," \
      "Address        CHAR(50)," \
      "Salary         REAL,"\
      "utf16         TEXT,"\
      "Data         BLOB NULL,"\
      "Ħφ             int);";
	
	if(!dbConnection.executeQuery(query.c_str())){
			std::cout<<"Fail: "<<"\n";
	}

	for(int i=0; i<40; i++){
		query="insert into COMPANY values ('";
		query+=std::to_string(i)+"',";        // ID
		query+="'"+randomStr(5+rand()%51)+"',";// Name
		query+="'"+std::to_string(5+rand()%53)+"',"; // Age
		query+="'"+randomStr(5+rand()%61)+"',";   // Address
		query+="'"+std::to_string(5.27+rand()%100)+"',";   // Salary		

		if(i%2==0){
			query+="'ψϬ࠽',";   // utf16
		}
		else{
			query+="'నಋሯᐮ',"; // utf16
		}
		query+="'',";   //Data
		query+="'"+std::to_string(rand()%10+rand()%100)+"')";   // Ħφ
		
		if(!dbConnection.executeQuery(query.c_str())){
			std::cout<<"Fail: "<<i<<"\n";
			break;
		}
	}

	return 0;
}
