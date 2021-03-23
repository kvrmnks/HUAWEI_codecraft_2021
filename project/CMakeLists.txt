# cmake_minimum_required(VERSION <specify CMake version here>)
project(project)

set(CMAKE_CXX_STANDARD 11)

add_executable(project main.cpp input.h DataStructure.h output.h BaselineSolver.h SegmentTree.h BlockList.h)