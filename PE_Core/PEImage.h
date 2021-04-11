#pragma once
#include "IImageGraphicsData.h"
#include <string>
#include <memory>
struct PEImage
{
	int width;
	int height;
	int numChannels;
	std::string path;
	unsigned char* data;
	std::shared_ptr<IImageGraphicsData> imageGraphicsData;
	PEImage();
	PEImage(std::string p);
	PEImage(PEImage& other);
	PEImage& operator=(PEImage& other);
	PEImage(PEImage&& other);
	PEImage& operator=(PEImage&& other);
	//~PEImage();
};