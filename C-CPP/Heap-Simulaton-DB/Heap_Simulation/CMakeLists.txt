cmake_minimum_required(VERSION 2.8.9)
project(HeapSimulation)

include_directories(Include)
 
file(GLOB SOURCES "src/*.c")
 
add_executable(HeapSimulation ${SOURCES})  
target_link_libraries(HeapSimulation ${CMAKE_SOURCE_DIR}/Include/BF_64.a)


