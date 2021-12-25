#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "GL/glew.h"

class Shader
{
private:
	GLint shaderType;
	GLint id;
public:
	Shader();
	Shader(std::string pathArg, GLint typeArg);
	~Shader();
	void Compile();
	GLint GetId();
	Shader& operator=(Shader&& other);
	Shader& operator=(Shader& other);
	Shader(Shader&& other) noexcept;
	Shader(Shader& other);
};

