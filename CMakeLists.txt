cmake_minimum_required(VERSION 3.0.0)

project(Crimson)

add_subdirectory(Extern/SDL)

set(CRIMSON_SOURCE
   Source/Application.cpp
)

include_directories(Extern/SDL/include)

add_executable(Crimson ${CRIMSON_SOURCE})

if (MSVC)
   target_link_libraries(Crimson opengl32 SDL2main SDL2)
else()
   target_link_libraries(Crimson libGL.so SDL2main SDL2)
endif()
