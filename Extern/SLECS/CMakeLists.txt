cmake_minimum_required(VERSION 3.11)

project(SLECS)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_BUILD_TYPE Debug)

set(SOURCE
   Test.cpp
)

add_executable(SLECS ${SOURCE})
