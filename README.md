![](https://raw.githubusercontent.com/georgelam6/Crimson/master/docs/media/transparentlogo.png)


[![Codacy Badge](https://api.codacy.com/project/badge/Grade/23732aa65d3041cfbb2551d82c977b1d)](https://app.codacy.com/gh/georgelam6/Crimson?utm_source=github.com&utm_medium=referral&utm_content=georgelam6/Crimson&utm_campaign=Badge_Grade)


A WIP 3D game engine

![](https://github.com/georgelam6/Crimson/blob/master/docs/media/ss/Screenshot_2021-01-27_17-28-49.png)

# Current Features
 - 3D rendering
 - Scripting with AngelScript
 - Editor
 - Scene serialisation

# Building
### Prerequisites
 - Premake5
 - A C++11 compiler
 - A GPU with OpenGL 4.2+

### Unix
The `run.sh` file will build the project. If the build is successful, then it will run the editor executable.
```bash
./run.sh
```
MacOS is currently untested, but it likely will not work due to Apple's shaky support of OpenGL.

### Windows
Generate Visual Studio/CodeLite project using Premake5, open the project and click "build".
```bash
premake5 vs2019
```
