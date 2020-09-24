#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "GL/glew.h"

class Shader
{
private:
	std::string path;
	GLint shaderType;
	GLint id;
public:
	Shader(std::string pathArg, GLint typeArg);
	~Shader();
	void Compile();
	GLint GetId();
};

