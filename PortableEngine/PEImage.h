#pragma once
struct PEImage
{
	int width;
	int height;
	int numChannels;
	const char* path;
	unsigned char* data;
};