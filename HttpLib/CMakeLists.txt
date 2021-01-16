cmake_minimum_required(VERSION 2.8.9)
project(HttpLib)

set(CMAKE_CXX_STANDARD 17)
 
#Can manually add the sources using the set command as follows:
#set(SOURCES src/mainapp.cpp src/Student.cpp)

include_directories(".")

file(GLOB SOURCES "*.cpp" "linux/*.cpp")
 
add_library(HttpLib ${SOURCES})

target_precompile_headers(HttpLib PRIVATE pch.h)

if (NOT MSVC)
  target_link_libraries(${PROJECT_NAME} stdc++fs pthread)
endif()

