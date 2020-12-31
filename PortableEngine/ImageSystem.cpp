#include "ImageSystem.h"

bool ImageSystem::LoadImage(const char* path, PEImage& img)
{
	img.data = stbi_load(path, &img.width, &img.height, &img.numChannels, 0);
	return img.data != nullptr;
}
