cmake_minimum_required(VERSION 3.16)
project(EmscriptenPortTest)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")
add_executable(EmscriptenPortTest Main.cpp esUtil.c PortableGame.cpp		
GameWindow.cpp		
EmscriptenPlatform.cpp
OpenGLAPI.cpp			
OpenGLESContext.cpp)
include_directories(../../../common/glm)
include_directories(../../entt/src/entt)
set(CMAKE_EXECUTABLE_SUFFIX ".html")
target_link_libraries(EmscriptenPortTest GLESv2 EGL m X11)