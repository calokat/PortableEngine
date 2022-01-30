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

namespace PE
{
	enum Platform { Win32, Web, Android };
	enum GraphicsAPI { OpenGL, DirectX11 };
	enum XrPlatform { None, OpenXR };
}
struct Options
{
	PE::Platform platform;
	PE::GraphicsAPI graphicsAPI;
	PE::XrPlatform xr = PE::XrPlatform::None;
};
