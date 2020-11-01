# Crimson
A WIP 3D game engine written in C++ 17

![Screenshot](https://raw.githubusercontent.com/georgelam6/Crimson/master/Screenshots/02.png)

## Current Features
 - 3D rendering
 - ECS
 - Physics
 - Lua-based mesh & material loading (Allows for custom shaders without needing to edit C++)
 - Asset management, loads assets from a folder during debug, and from an archive in debug
 - AngelScript scripting
 - Input system

## Work-in-Progress
 - Level editor
 - Asset packer
 - Web & Android runtime support
 - Node-based scripting
 - Node-based shader creation

## Dependencies
All dependencies are included in [`Crimson/Dependencies`](https://github.com/georgelam6/Crimson/tree/master/Crimson/Dependencies) and are automatically built by Premake
 - [AngelScript](https://www.angelcode.com/angelscript/)
 - [Dear ImGui](https://github.com/ocornut/imgui)
 - [GLFW](https://www.glfw.org/)
 - [GLM](https://glm.g-truc.net)
 - [Glad](https://glad.dav1d.de/)
 - [Lua](https://www.lua.org/)
 - [Physics FS](https://icculus.org/physfs/)
 - [React Physics 3D](https://www.reactphysics3d.com/)
