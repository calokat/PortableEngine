#pragma once
#include "PEImage.h"
namespace ImageSystem
{
	bool CreateImage(PEImage& img, unsigned char** data);
	void DestroyImageData(unsigned char* data);
};