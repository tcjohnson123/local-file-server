cmake_minimum_required(VERSION 2.8.9)
project(FileServer)

set(CMAKE_CXX_STANDARD 17)
 
#Can manually add the sources using the set command as follows:
#set(SOURCES src/mainapp.cpp src/Student.cpp)

include_directories("../HttpLib")

file(GLOB SOURCES "*.cpp")
 
add_executable(FileServer ${SOURCES})

target_precompile_headers(FileServer PRIVATE pch.h)

if (NOT MSVC)
  target_link_libraries(${PROJECT_NAME} HttpLib stdc++fs pthread)
endif()

