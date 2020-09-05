cmake_minimum_required(VERSION 3.0.0)

project(glad)

include_directories(include)
add_library(glad src/glad.c)

target_link_libraries(glad libdl.so)
