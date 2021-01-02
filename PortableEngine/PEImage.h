#pragma once
#include "IImageGraphicsData.h"
struct PEImage
{
	int width;
	int height;
	int numChannels;
	const char* path;
	unsigned char* data;
	IImageGraphicsData* imageGraphicsData;
};