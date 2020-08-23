cmake_minimum_required(VERSION 3.0.0)

project(Crimson)

add_subdirectory(Extern/SDL)

set(CRIMSON_SOURCE
   Source/Application.cpp
)

include_directories(Extern/SDL/include)

add_executable(Crimson ${CRIMSON_SOURCE})
target_link_libraries(Crimson SDL2main SDL2)
