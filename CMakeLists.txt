cmake_minimum_required(VERSION 3.0.0)

project(Crimson)

add_subdirectory(Extern/SDL)
add_subdirectory(Extern/glad)

set(CRIMSON_SOURCE
   Source/Application.cpp
)

include_directories(Extern/SDL/include Extern/glad/include)

add_executable(Crimson ${CRIMSON_SOURCE})

if (WIN32)
   set(GL_LIB opengl32)
else()
   set(GL_LIB libGL.so)
endif()

target_link_libraries(Crimson ${GL_LIB} glad SDL2main SDL2)
