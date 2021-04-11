#pragma once
#include <string>
#include <glm/glm.hpp>
#include "PEImage.h"
#include "IRenderer.h"
struct Name
{
public:
	std::string nameString;
};

struct Rotator
{
	glm::vec3 deltaRot;
};

struct Thumbnail
{
	PEImage assetImage;
	IRenderer assetImageRenderer;
};
