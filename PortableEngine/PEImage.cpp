#include "PEImage.h"

PEImage::PEImage() : PEImage("")
{
}

PEImage::PEImage(std::string p) : width(-1), height(-1), numChannels(-1), path(p), data(0), imageGraphicsData(0)
{
}

PEImage::~PEImage()
{
	delete imageGraphicsData;
}
