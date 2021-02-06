#pragma once
#include "PEImage.h"

enum AssetType { File, Image, Model, Directory };
class PEAsset
{
public:
	PEImage* thumbnail;
	std::string path;
	AssetType assetType;
	std::string name;
	~PEAsset()
	{
		delete thumbnail;
	}
};
