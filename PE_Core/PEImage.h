#pragma once
#include "IImageGraphicsData.h"
#include <string>
#include <memory>
struct PEImage
{
	std::string path;
	int width;
	int height;
	int numChannels;
	std::shared_ptr<IImageGraphicsData> imageGraphicsData;
	bool pathChanged;
	//~PEImage();
};