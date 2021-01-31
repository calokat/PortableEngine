#pragma once
#include "IImageGraphicsData.h"
#include <string>
struct PEImage
{
	int width;
	int height;
	int numChannels;
	std::string path;
	unsigned char* data;
	IImageGraphicsData* imageGraphicsData;
	PEImage();
	PEImage(std::string p);
	PEImage(PEImage&& other);
	PEImage& operator=(PEImage&& other);
	~PEImage();
};