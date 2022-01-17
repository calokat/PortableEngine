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
	shaderProgram.attributes = { {"in_position", {} } };
	if (type & ShaderProgramProperties::Lit)
	{
		shaderProgram.attributes.emplace(std::pair<const char*, OpenGLShaderField>("in_normal", {}));
	}
	shaderProgram.vertexUniforms = { {"projection", {} }, {"view", {} }, {"model", {} }, {"in_color", {} } };
	if (type & ShaderProgramProperties::Lit)
	{
		shaderProgram.fragmentUniforms = { 
			{"dirLight.AmbientColor", {} }, {"dirLight.DiffuseColor", {} }, {"dirLight.Direction", {} },
			{"pointLights[0].AmbientColor", {} }, {"pointLights[0].DiffuseColor", {} }, {"pointLights[0].Position", {} },
			//{"pointLights[1].AmbientColor", {} }, {"pointLights[1].DiffuseColor", {} }, {"pointLights[1].Position", {} },
			//{"pointLights[2].AmbientColor", {} }, {"pointLights[2].DiffuseColor", {} }, {"pointLights[2].Position", {} },
			//{"pointLights[3].AmbientColor", {} }, {"pointLights[3].DiffuseColor", {} }, {"pointLights[3].Position", {} },
			//{"pointLights[4].AmbientColor", {} }, {"pointLights[4].DiffuseColor", {} }, {"pointLights[4].Position", {} },
			//{"pointLights[5].AmbientColor", {} }, {"pointLights[5].DiffuseColor", {} }, {"pointLights[5].Position", {} },
			//{"pointLights[6].AmbientColor", {} }, {"pointLights[6].DiffuseColor", {} }, {"pointLights[6].Position", {} },
			//{"pointLights[7].AmbientColor", {} }, {"pointLights[7].DiffuseColor", {} }, {"pointLights[7].Position", {} },
			{"cameraPos", {} }, {"specularIntensity", {} }
		};
	}
	shaderProgram.propertyFlags = type;
}

GLRenderer::GLRenderer()
{
	numVertices = -1;
	numIndices = 0;
}

