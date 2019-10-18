cmake_minimum_required(VERSION 2.8.9)
project(BPlusTree)


#Bring the headers, such as Student.h into the project
include_directories(Include)
 
#Can manually add the sources using the set command as follows:
#set(SOURCES src/mainapp.cpp src/Student.cpp)
 
#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.c")
 
add_executable(BPlusTree ${SOURCES})  
target_link_libraries(BPlusTree ${CMAKE_SOURCE_DIR}/Include/BF_64.a)
target_link_libraries(BPlusTree ${CMAKE_SOURCE_DIR}/Include/BF.lib)

