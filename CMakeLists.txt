cmake_minimum_required (VERSION 3.5)
project (sudokusolver)

set(CMAKE_CXXFLAGS "${CMAKE_CXXFLAGS} -O2 -g")
add_definitions(-std=c++11)

add_executable(sudokusolver Cell.cpp main.cpp Grid.cpp)
