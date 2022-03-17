#pragma once
#include <string>
#include <map>
#include "Shader.h"

union SignedOrUnsigned
{
	GLuint u;
	GLint s;
};

struct OpenGLShaderField
{
	const char* name;
	GLint value;
};

enum class ShaderAttributes:unsigned long {Position, Normal, TextureCoordinate, Tangent, __COUNT__};
enum class VertexUniforms:unsigned long {Projection, View, Model, Color, __COUNT__};
enum class FragmentUniforms:unsigned long {CameraPosition, SpecularIntensity, DiffuseTextureID, NormalTextureID, NormalMatrix, __COUNT__};
enum class PointLightUniforms:unsigned long {AmbientColor, DiffuseColor, Position, Intensity, __COUNT__};
enum class DirectionalLightUniforms :unsigned long { AmbientColor, DiffuseColor, Direction, __COUNT__ };
enum class SpotLightUniforms:unsigned long {AmbientColor, DiffuseColor, InverseOrientation, Position, Angle, Intensity, Range, __COUNT__};

class OpenGLShaderProgram
{
public:
	SignedOrUnsigned vao;
	SignedOrUnsigned vbo;
	SignedOrUnsigned ibo;
	unsigned int programID;
	Shader vertex;
	Shader pixel;
	OpenGLShaderField attributes[(unsigned long)ShaderAttributes::__COUNT__] = {};
	OpenGLShaderField vertexUniforms[(unsigned long)VertexUniforms::__COUNT__] = {};
	OpenGLShaderField fragmentUniforms[(unsigned long)FragmentUniforms::__COUNT__] = {};
	OpenGLShaderField pointLightData[8][(unsigned long)PointLightUniforms::__COUNT__] = {};
	OpenGLShaderField dirLightLightData[(unsigned long)DirectionalLightUniforms::__COUNT__] = {};
	OpenGLShaderField spotLightData[(unsigned long)SpotLightUniforms::__COUNT__] = {};

	int propertyFlags;
};

#define POINT_LIGHT_NAME(index, field) "pointLights[" index "]." field

#define DIR_LIGHT_NAME(field) "dirLight." field