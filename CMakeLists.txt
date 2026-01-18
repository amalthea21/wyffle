cmake_minimum_required(VERSION 3.31.6)
project(wyffle)

set(CMAKE_CXX_STANDARD 26)

add_executable(wyffle main.cpp
        include/Formatting.h
        include/printFunctions.h
        src/printFunctions.cpp
        include/fileFunctions.h
        src/fileFunctions.cpp)
