#pragma once
#include "IImageGraphicsData.h"
#include <string>
#include <memory>
#include "PEString.h"
struct PEImage
{
	PE::String path;
	int width;
	int height;
	int numChannels;
	std::shared_ptr<IImageGraphicsData> imageGraphicsData;
	bool pathChanged;
	//~PEImage();
};