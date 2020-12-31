#pragma once
#include "PEImage.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
class ImageSystem
{
	bool LoadImage(const char* path, PEImage& img);
};