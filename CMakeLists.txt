cmake_minimum_required(VERSION 3.10)

project(buaac)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -g -rdynamic")
#include_directories(3rdparty)
#include_directories(include src)

#add_subdirectory(src)
add_executable(buaac main.cpp)
#target_link_libraries(buaac fmt)

enable_testing()
add_subdirectory(tests)
