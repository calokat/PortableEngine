#pragma once
#include "glm/common.hpp"

struct DirectionalLight
{
	glm::vec3 AmbientColor;
	float padding1;
	glm::vec3 DiffuseColor;
	float padding2;
	glm::vec3 Direction;
	float padding3;
};

struct PointLight
{
	glm::vec3 AmbientColor;
	float padding1;
	glm::vec3 DiffuseColor;
	float padding2;
	glm::vec3 Position;
	float padding3;
};

struct LightBufferData {
	DirectionalLight dirLight;
	glm::vec3 cameraPos;
	float padding1;
	float specularIntensity;
	glm::vec3 padding2;
	PointLight pointLight;
};