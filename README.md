# VisualRay3D
VisualRay3D is a ECS game engine with focus on low-end devices. For now it has been implemented as a dynamic library and still very minimalistic.
The primary goal of this project is to make an individual creative tool.

The engine uses [GLFW](https://github.com/glfw/glfw) and OpenGL 4.6 for context/rendering, ReactPhysics3D for physics and [EnTT](https://github.com/skypjack/entt) for an ECS implementation.
![screenshot](https://i.ibb.co/VcD4Mv4p/image.png)

# Build
This project uses MSBuild. To build it, locate the solution file (.sln) in the repo root directory and open it with Visual Studio.
Then press Ctrl+Shift+B to build all projects in this solution.

# Requirements
 - OS: Windows 10 or higher
 - Visual Studio 2022
 - C++20+