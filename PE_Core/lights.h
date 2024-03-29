#pragma once
#include "glm/common.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"

#define MAX_POINT_LIGHTS 8
#define MAX_DIR_LIGHTS 1
#define MAX_SPOT_LIGHTS 4

struct DirectionalLight
{
	glm::vec4 AmbientColor;
	glm::vec4 DiffuseColor;
	glm::vec4 Direction;
};

struct PointLight
{
	glm::vec4 AmbientColor;
	glm::vec4 DiffuseColor;
	glm::vec3 Position;
	float intensity;
};

struct SpotLight
{
	glm::vec4 AmbientColor;
	glm::vec4 DiffuseColor;
	glm::vec3 LightFwd;
	float Intensity;
	glm::vec3 Position;
	float Angle;
	float Range;
	// For DirectX alignment requirements
	glm::vec3 padding;
};

struct LightBufferData {
	DirectionalLight dirLight;
	PointLight pointLight;
	SpotLight spotLight;
	glm::vec3 cameraPos;
	float specularIntensity;
};