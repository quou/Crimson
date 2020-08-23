cmake_minimum_required(VERSION 3.0.0)


add_subdirectory(Extern/SDL)
add_subdirectory(Extern/glad)


# ENGINE
project(Crimson)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

set(CRIMSON_EXT_INCLUDES
   Extern/SDL/include
   Extern/glad/include
)

set(CRIMSON_INCLUDE_DIR Engine)

set(CRIMSON_SOURCE
   Engine/Graphics/Display.cpp
)

if (WIN32)
   set(GL_LIB opengl32)
else()
   set(GL_LIB libGL.so)
endif()

set(CRIMSON_DEPS
   ${GL_LIB}
   glad
   SDL2main
   SDL2
)

add_library(Crimson ${CRIMSON_SOURCE})
target_include_directories(Crimson PUBLIC ${CRIMSON_EXT_INCLUDES} Engine)
target_link_libraries(Crimson ${CRIMSON_DEPS})


# SANDBOX
project(Sandbox)

set(SANDBOX_SOURCE
   Source/Application.cpp
)

add_executable(Sandbox ${SANDBOX_SOURCE})
target_include_directories(Sandbox PUBLIC ${CRIMSON_EXT_INCLUDES} ${CRIMSON_INCLUDE_DIR})
target_link_libraries(Sandbox ${CRIMSON_LIBS} Crimson)
