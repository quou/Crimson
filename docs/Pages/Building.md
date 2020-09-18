# Building
How to build the engine from source.

### Prerequisites
 - A modern C++ compiler (ie. supporting C++ 17 and above)
 - A GPU supporting OpenGL 3.3 or above
 - CMake

**Note:** using MSVC to compile Crimson will work, but I don't recommend it, as in my testing the build can crash unexpectedly and for no apparent reason. To compile on Windows I recommend MinGW.

### Instructions
 - Acquire the source from [the GitHub repository](https://github.com/georgelam6/Crimson)
 - Build the CMake project (using the CMake GUI or using `cmake .`)
 - Build the project
 - The editor executable, as well as the Crimson library will be located in the `bin` folder.
