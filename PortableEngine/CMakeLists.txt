cmake_minimum_required(VERSION 3.16)
project(EmscriptenPortTest)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z --preload-file ../../Assets/Models --preload-file ../../Shaders/GLSL-ES")
add_executable(EmscriptenPortTest Main.cpp esUtil.c PortableGame.cpp		
GameWindow.cpp		
EmscriptenPlatform.cpp
EmscriptenInputSystem.cpp
Mesh.cpp
OpenGLAPI.cpp			
OpenGLESContext.cpp
Renderer.cpp
Camera.cpp
Shader.cpp
Timer.cpp
Transform.cpp)
include_directories(../../../common/glm)
include_directories(../../entt/src/entt)
set(CMAKE_EXECUTABLE_SUFFIX ".html")
target_link_libraries(EmscriptenPortTest GLESv2 EGL m X11)