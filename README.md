# Crimson

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/23732aa65d3041cfbb2551d82c977b1d)](https://app.codacy.com/gh/georgelam6/Crimson?utm_source=github.com&utm_medium=referral&utm_content=georgelam6/Crimson&utm_campaign=Badge_Grade)

A WIP 3D game engine written in C++ 17

If you have any questions or comments - I will respond quicker on Discord rather than GitHub or email: https://discord.gg/6pUUArRWhK

![Screenshot](https://raw.githubusercontent.com/georgelam6/Crimson/master/Screenshots/05.png)

Master branch is stable. Development branch may have bugs, or not build at all.

## Current Features
 - 3D rendering
 - ECS
 - Physics
 - Lua-based mesh & material loading (Allows for custom shaders without needing to edit C++)
 - Asset management (loads assets from a folder during debug, and from an archive in debug, never loading a single asset twice)
 - AngelScript scripting
 - Input system
 - Asset packer
 - Level editor

## Work-in-Progress
 - Audio
 - Web & Android runtime support
 - Node-based scripting
 - Node-based shader creation
 - Physics-based rendering
 - Prefab system
 - Scriptable editor extensions

## Building
Installation of dependencies isn't required, they are all built and linked on the fly

### Unix-based systems (make)
```
~/Crimson$ ./Scripts/Unix/Build.sh
```

### Windows (Visual Studio & Clang)
**MSVC is not supported. Use Clang instead**

Run `Scripts/Windows/Build.bat` to generate a Visual Studio project (includes dependencies).

The game executable must be copied from the `bin` folder into the Editor working directory. On Unix this happens automatically via the `cp` command

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
 - [Entt](https://github.com/skypjack/entt)
 - [Miniz](https://github.com/richgel999/miniz)
