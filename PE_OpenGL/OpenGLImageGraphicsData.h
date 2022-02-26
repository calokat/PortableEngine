#pragma once
#include "IImageGraphicsData.h"
#include <GL/glew.h>
struct OpenGLImageGraphicsData : public IImageGraphicsData
{
	GLuint texture;
	GLuint GetData()
	{
		return texture;
	}
};