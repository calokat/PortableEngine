#include "ImageSystem.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace ImageSystem
{
	bool CreateImage(PEImage& img, unsigned char** data)
	{
		*data = stbi_load(img.path.c_str(), &img.width, &img.height, &img.numChannels, 0);
		return data != nullptr;
	}

	void DestroyImageData(unsigned char* data)
	{
		stbi_image_free(data);
	}
}