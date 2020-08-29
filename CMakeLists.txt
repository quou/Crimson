cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_BINARY_DIR bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

add_subdirectory(Extern/SDL)
add_subdirectory(Extern/glad)
add_subdirectory(Extern/glm)


set(CMAKE_CXX_STANDARD 14)

# ENGINE
project(Crimson)

set(CRIMSON_EXT_INCLUDES
   Extern/SDL/include
   Extern/glad/include
   Extern/glm
   Extern/imgui
   Extern/imgui/examples
)

set(CRIMSON_INCLUDE_DIR Crimson)

set(IMGUI_SOURCE
   Extern/imgui/imgui.cpp
   Extern/imgui/imgui_demo.cpp
   Extern/imgui/imgui_draw.cpp
   Extern/imgui/imgui_widgets.cpp
   Extern/imgui/examples/imgui_impl_sdl.cpp
   Extern/imgui/examples/imgui_impl_opengl3.cpp
)

set(CRIMSON_SOURCE
   ${IMGUI_SOURCE}
   Crimson/Graphics/Display.cpp
   Crimson/Graphics/Renderer.cpp
   Crimson/Graphics/Shader.cpp
   Crimson/Graphics/Model.cpp
   Crimson/Graphics/Texture.cpp
   Crimson/Graphics/Lighting/Light.cpp
   Crimson/Graphics/Lighting/Directional.cpp
   Crimson/Graphics/Material.cpp
   Crimson/Utils/obj_loader.cpp
   Crimson/Utils/stb_image.c
   Crimson/Utils/tinyxml2.cpp
   Crimson/Utils/ImGuizmo.cpp
   Crimson/SceneManagement/SceneManager.cpp
   Crimson/Application.cpp
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
target_include_directories(Crimson PUBLIC ${CRIMSON_EXT_INCLUDES} Crimson)
target_link_libraries(Crimson ${CRIMSON_DEPS})


# SANDBOX
# project(Sandbox)
#
# set(SANDBOX_SOURCE
#    Sandbox/Application.cpp
# )
#
# add_executable(Sandbox ${SANDBOX_SOURCE})
# target_include_directories(Sandbox PUBLIC ${CRIMSON_EXT_INCLUDES} ${CRIMSON_INCLUDE_DIR})
# target_link_libraries(Sandbox ${CRIMSON_LIBS} Crimson)

# EDITOR
project(Editor)

set(EDITOR_SOURCE
   Editor/Application.cpp
   Editor/GUI.cpp
)

add_executable(Editor ${EDITOR_SOURCE})
target_include_directories(Editor PUBLIC ${CRIMSON_EXT_INCLUDES} ${CRIMSON_INCLUDE_DIR})
target_link_libraries(Editor ${CRIMSON_LIBS} Crimson)
