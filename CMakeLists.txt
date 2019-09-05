cmake_minimum_required(VERSION 3.10)

project(buaac)
set(CMAKE_CXX_STANDARD 14)


set(SRC_LIST main.cpp)
message(STATUS "This is BINARY dir " ${BUAAC_BINARY_DIR})
message(STATUS "This is SOURCE dir " ${BUAAC_SOURCE_DIR})

project(untitled)


add_executable(buaac main.cpp)

