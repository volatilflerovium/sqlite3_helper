cmake_minimum_required(VERSION 3.13.4)

# Project's name
project(SQLite3Test)

# Requires
#FIND_PACKAGE(SQLite3 REQUIRED) 

# Show a message with the opencv version detected 
MESSAGE("SQLite3 version? : ${SQLite3_INCLUDE_DIRS}") 

# Add the paths to the include directories/to the header files
include_directories(${SQLite3_INCLUDE_DIRS}) 

# Add the paths to the compiled libraries/objects
link_directories(${SQLite3_LIBRARIES}) 

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCES 
	#test.cpp
	#test2.cpp
	#test3.cpp
	test_blob.cpp
	sqlite_db_traits.cpp
)

add_executable(sqlite_test ${SOURCES})

#target_link_libraries(sqlite_test ${SQLite3_LIBRARIES})
target_link_libraries(sqlite_test -lsqlite3)
