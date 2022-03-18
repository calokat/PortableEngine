# Portable Engine

This game engine was created over my Fall 2020 semester. I had two goals: 

- Create a "game engine" that had tools and affordances (i.e. clicking on objects to select them, 3D gizmos)
- Have the engine support multiple platforms and Graphics APIs

Adding features and supporting multiple platforms are often in direct conflict with each other. However, I was able to have the codebase support OpenGL-Windows, OpenGL-Web, and DirectX-Windows (check the DirectXImpl branch for that) as well as have cool stuff like an inspector and an entity-component-system architecture via [entt](https://github.com/skypjack/entt).
Other features I want to add are:
- Support for OpenGL-Android
- Support for DirectX 12 and Vulkan
- (Done) Have textures, instead of solid colors
- (Done) VR and AR support

You can see an earlier version of the engine through a web build [here](https://calokat.github.io/PortableEngine)

## Building

PortableEngine uses CMake as a build system and vcpkg as a package manager.

You will need at least the following packages from vcpkg:

### All platforms
- glew
- glm
- entt
- assimp
- imgui (with opengl3, SDL2, DirectX11, docking-experimental, and Win32 features enabled)
- stb

### Windows 
- openxr-loader
- directxtk

### Linux
- SDL2
- Additional packages may be needed from your OS' package manager (ex. apt)

Once all packages are downloaded from vcpkg, please run

```
cmake -S . -B build/ -DCMAKE_TOOLCHAIN_FILE={path_to_vcpkg_root}/scripts/buildsystems/vcpkg.cmake
```
