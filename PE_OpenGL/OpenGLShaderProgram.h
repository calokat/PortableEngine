#pragma once
#include <string>
#include <map>
#include "Shader.h"

enum ShaderProgramProperties {Fully_3D = 1, Colored = 2, Textured = 4};

union SignedOrUnsigned
{
	GLuint u;
	GLint s;
};

struct OpenGLShaderField
{
	SignedOrUnsigned value;
};

class OpenGLShaderProgram
{
public:
	SignedOrUnsigned vao;
	SignedOrUnsigned vbo;
	SignedOrUnsigned ibo;
	unsigned int programID;
	Shader vertex;
	Shader pixel;
	std::map<const char*, OpenGLShaderField> attributes;
	std::map<const char*, OpenGLShaderField> uniforms;
	int propertyFlags;
};
