#include "XRGraphicsPlugin_OpenGL.h"

std::vector<const char*> XRGraphicsPlugin_OpenGL::GetGraphicsExtensions()
{
    return { "XR_KHR_opengl_enable" };
}
