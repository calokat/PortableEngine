#pragma once
#include "glm/common.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#define MAX_POINT_LIGHTS 8


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
	glm::vec4 Position;
};

struct LightBufferData {
	DirectionalLight dirLight;
	PointLight pointLight;
	glm::vec3 cameraPos;
	float specularIntensity;
};