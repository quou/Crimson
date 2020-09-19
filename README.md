# Crimson
A WIP 3D game engine written in C++ 17

Compiles with GCC, MinGW and Clang using CMake. MSVC is not supported.

![Screenshot](https://github.com/georgelam6/Crimson/blob/master/Screenshots/screenshot.png)

## Building
How to build the engine from source.

### Prerequisites
 - A modern C++ compiler (ie. supporting C++ 17 and above)
 - A GPU supporting OpenGL 3.3 or above
 - CMake

**Note to Windows users:** using MSVC to compile Crimson will work, but I don't recommend it, as in my testing the build can crash unexpectedly and for no apparent reason. To compile on Windows I recommend MinGW.

### Instructions
 - Acquire the source from [the GitHub repository](https://github.com/georgelam6/Crimson)
 - Build the CMake project (using the CMake GUI or by running `cmake .` from the root folder)
 - Build the project
 - The editor and runtime executables, as well as the Crimson library will be located in the `bin` folder.

## Documentation
View documentation on [the website](https://georgelam6.github.io/Crimson)
