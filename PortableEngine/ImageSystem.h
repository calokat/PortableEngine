#pragma once
#include "PEImage.h"
namespace ImageSystem
{
	bool CreateImage(const char* path, PEImage& img);
	void DestroyImage(PEImage& img);
};