#include "ImageSystem.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace ImageSystem
{
	bool ImageSystem::CreateImage(const char* path, PEImage& img)
	{
		img.data = stbi_load(path, &img.width, &img.height, &img.numChannels, 0);
		return img.data != nullptr;
	}

	void ImageSystem::DestroyImage(PEImage& img)
	{
		stbi_image_free(img.data);
	}
}