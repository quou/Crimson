cmake_minimum_required(VERSION 3.0.0)

project(Crimson)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

add_subdirectory(Extern/SDL)
add_subdirectory(Extern/glad)

set(CRIMSON_SOURCE
   Source/Application.cpp
   Source/Graphics/Display.cpp
)

set(EXT_INCLUDES
   Extern/SDL/include
   Extern/glad/include
)

include_directories(${EXT_INCLUDES} Source)

add_executable(Crimson ${CRIMSON_SOURCE})

if (WIN32)
   set(GL_LIB opengl32)
else()
   set(GL_LIB libGL.so)
endif()

target_link_libraries(Crimson ${GL_LIB} glad SDL2main SDL2)
