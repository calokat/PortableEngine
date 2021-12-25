#include "Shader.h"

Shader::Shader()
{
	shaderType = -1;
	id = -1;
}

Shader::Shader(std::string pathArg, GLint typeArg)
{
	shaderType = typeArg;
	id = glCreateShader(shaderType);
	//path = pathArg;
	std::ifstream stream;
	stream.open(pathArg, std::ios::binary);
	if (!stream.is_open())
	{
		printf("No file found\n");
		return;
	}
	stream.seekg(0, std::ios::end);
	int shaderLength = (int)stream.tellg();
	char* shaderSource = new char[shaderLength + 1];
	stream.seekg(0, std::ios::beg);
	stream.read(shaderSource, shaderLength);
	shaderSource[shaderLength] = '\0';
	stream.close();
	glShaderSource(id, 1, &shaderSource, 0);
	delete[] shaderSource;
}

Shader::~Shader()
{
}

void Shader::Compile()
{
	glCompileShader(id);
	int checkStatus;
	glGetShaderiv(id, GL_COMPILE_STATUS, &checkStatus);
	if (checkStatus == 0)
	{
		GLint length = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		GLchar* log = new char[length + 2];
		glGetShaderInfoLog(id, length, 0, log);
		log[length - 1] = '\n';
		log[length] = '\0';
		printf(log);
		glDeleteShader(id);
		delete[] log;

		printf("Shader compilation failed\n");
		return;
	}

}

GLint Shader::GetId()
{
	return id;
}

Shader& Shader::operator=(Shader&& other)
{
	if (this != &other)
	{
		this->shaderType = other.shaderType;
		this->id = other.id;
	}
	return *this;
}

Shader& Shader::operator=(Shader& other)
{
	if (this != &other)
	{
		this->shaderType = other.shaderType;
		this->id = other.id;
	}
	return *this;
}

Shader::Shader(Shader&& other) noexcept
{
	*this = std::move(other);
}

Shader::Shader(Shader& other)
{
	*this = std::move(other);
}
