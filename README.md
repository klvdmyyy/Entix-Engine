# Entix Engine - 3D Engine written in C++

Entix Engine - my hobby 3D Game Engine written in C++23.

I would keep support for only Windows and Linux for now.

## Main features

- Supports both Windows and Linux
- 3D rendering powered by OpenGL only
- ECS-based architecture
- Good Resource Management (i think it's good)
    - Built-in loaders for Shaders, Textures and OBJ mesh
- Two different modules (Runtime and GameFramework)
- C++ scripting
- Good profiling powered by [Tracy](https://github.com/wolfpld/tracy)

## Structure of the project

- `CMake/` - CMake modules/scripts
- `Libraries/` - Engine libraries
    - `Libraries/Runtime/` - Core runtime library of the engine
    - `Libraries/GameFramework/` - Game dev related library of the engine
    - `Libraries/Vendor/` - Third-Party libraries (not all, a lot of libraries installed via CMake FetchContent)
- `Projects/` - Projects written with Entix Engine
