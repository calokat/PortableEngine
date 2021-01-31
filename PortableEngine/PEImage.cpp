#include "PEImage.h"

PEImage::PEImage() : PEImage("")
{
}

PEImage::PEImage(std::string p) : width(-1), height(-1), numChannels(-1), path(p), data(0), imageGraphicsData(0)
{
}

PEImage::PEImage(PEImage&& other)
{
	*this = std::move(other);
}

PEImage& PEImage::operator=(PEImage&& other)
{
	this->data = other.data;
	other.data = nullptr;
	this->height = other.height;
	this->imageGraphicsData = other.imageGraphicsData;
	other.imageGraphicsData = nullptr;
	this->numChannels = other.numChannels;
	this->path = other.path;
	this->width = other.width;
	return *this;
}

PEImage::~PEImage()
{
	delete imageGraphicsData;
}
