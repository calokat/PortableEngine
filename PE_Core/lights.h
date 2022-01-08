#pragma once
#include "glm/common.hpp"

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