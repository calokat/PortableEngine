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
	shaderProgram.propertyFlags = type;

	shaderProgram.attributes[(unsigned long)ShaderAttributes::Position].name = "in_position";
	if (type & ShaderProgramProperties::Lit)
	{
		shaderProgram.attributes[(unsigned long)ShaderAttributes::Normal].name = "in_normal";
		shaderProgram.fragmentUniforms[(unsigned long)FragmentUniforms::NormalMatrix].name = "normalMatrix";
	}
	shaderProgram.fragmentUniforms[(unsigned long)FragmentUniforms::CameraPosition].name = "cameraPos";
	if (type & ShaderProgramProperties::Textured)
	{
		shaderProgram.attributes[(unsigned long)ShaderAttributes::TextureCoordinate].name = "aTexCoord";
		shaderProgram.fragmentUniforms[(unsigned long)FragmentUniforms::DiffuseTextureID].name = "ourTexture";
	}
	if (type & ShaderProgramProperties::Normal)
	{
		shaderProgram.attributes[(unsigned long)ShaderAttributes::Tangent].name = "in_tangent";
		shaderProgram.fragmentUniforms[(unsigned long)FragmentUniforms::NormalTextureID].name = "in_normal";
	}
	shaderProgram.vertexUniforms[(unsigned long)VertexUniforms::Projection].name = "projection";
	shaderProgram.vertexUniforms[(unsigned long)VertexUniforms::View].name = "view";
	shaderProgram.vertexUniforms[(unsigned long)VertexUniforms::Model].name = "model";
	shaderProgram.vertexUniforms[(unsigned long)VertexUniforms::Color].name = "in_color";
	if (type & ShaderProgramProperties::Lit)
	{
		shaderProgram.pointLightData[0][(unsigned long)PointLightUniforms::AmbientColor].name = POINT_LIGHT_NAME("0", "AmbientColor");
		shaderProgram.pointLightData[0][(unsigned long)PointLightUniforms::DiffuseColor].name = POINT_LIGHT_NAME("0", "DiffuseColor");
		shaderProgram.pointLightData[0][(unsigned long)PointLightUniforms::Position].name = POINT_LIGHT_NAME("0", "Position");
		shaderProgram.dirLightLightData[(unsigned long)DirectionalLightUniforms::AmbientColor].name = "dirLight.AmbientColor";
		shaderProgram.dirLightLightData[(unsigned long)DirectionalLightUniforms::DiffuseColor].name = "dirLight.DiffuseColor";
		shaderProgram.dirLightLightData[(unsigned long)DirectionalLightUniforms::Direction].name = "dirLight.Direction";
		shaderProgram.fragmentUniforms[(unsigned long)FragmentUniforms::SpecularIntensity].name = "specularIntensity";
	}
}

GLRenderer::GLRenderer()
{
	numVertices = -1;
	numIndices = 0;
}

