#pragma once
#include <string>
#include <map>
#include "Shader.h"

enum ShaderProgramProperties {Fully_3D, Colored, Textured};

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
};
