cmake_minimum_required(VERSION 2.8.9)
project(HeapManagment)


#Bring the headers, such as Student.h into the project
include_directories(Include)
 
#Can manually add the sources using the set command as follows:
#set(SOURCES src/mainapp.cpp src/Student.cpp)
 
#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.c")
 
add_executable(HeapManagment ${SOURCES})  
target_link_libraries(HeapManagment ${CMAKE_SOURCE_DIR}/Include/BF_64.a)
target_link_libraries(HeapManagment ${CMAKE_SOURCE_DIR}/Include/BF.lib)

