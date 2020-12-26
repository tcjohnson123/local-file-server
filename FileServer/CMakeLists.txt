cmake_minimum_required(VERSION 2.8.9)
project(FileServer)

 
#if (MSVC)
#   #To do
#else()
#   add_compile_options(-std=c++17)
#endif()
set(CMAKE_CXX_STANDARD 17)
 
#Can manually add the sources using the set command as follows:
#set(SOURCES src/mainapp.cpp src/Student.cpp)

include_directories(".")

file(GLOB SOURCES "*.cpp" "linux/*.cpp")
 
add_executable(FileServer ${SOURCES})

if (NOT MSVC)
  target_link_libraries(${PROJECT_NAME} stdc++fs pthread)
endif()
