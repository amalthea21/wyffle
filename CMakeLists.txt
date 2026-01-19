cmake_minimum_required(VERSION 3.31.6)
project(wyffle)

set(CMAKE_CXX_STANDARD 26)

file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "include/*.h" "include/*.hpp")

add_executable(wyffle
        main.cpp
        ${SOURCES}
        ${HEADERS}
)