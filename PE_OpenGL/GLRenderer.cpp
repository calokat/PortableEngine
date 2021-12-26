#include "GLRenderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
GLRenderer::GLRenderer(std::string vertexShaderPathParam, std::string fragmentShaderPathParam, ShaderType type)
{
	shaderProgram.vertex = Shader(vertexShaderPathParam, GL_VERTEX_SHADER);
	shaderProgram.pixel = Shader(fragmentShaderPathParam, GL_FRAGMENT_SHADER);
	shaderProgram.vertex.Compile();
	shaderProgram.pixel.Compile();
	shaderProgram.attributes = { {"in_position", {} }, {"aTexCoord", {} } };
	shaderProgram.uniforms = { {"projection", {} }, {"view", {} }, {"model", {} }, {"in_color", {} } };
	shaderProgram.propertyFlags = type;
}

GLRenderer::GLRenderer()
{
	numVertices = -1;
	numIndices = 0;
}

