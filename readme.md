# Portable Engine

This game engine was created over my Fall 2020 semester. I had two goals: 

- Create a "game engine" that had tools and and affordances (i.e. clicking on objects to select them, 3D gizmos)
- Have the engine support multiple platforms and Graphics APIs

As you may know, adding features and supporting multiple platforms are often in direct conflict with each other. However, I was able to have the codebase support OpenGL-Windows, OpenGL-Web, and DirectX-Windows (check the DirectXImpl branch for that) as well as have cool stuff like an inspector and an entity-component-system architecture via [entt](https://github.com/skypjack/entt).
Other features I want to add are:
- Support for Android
- Support for DirectX 12 and Vulkan
- Have textures, instead of solid colors
- VR and AR support

You can see an earlier version of the engine through a web build [here](https://people.rit.edu/cnk5777/EmscriptenDemo/)