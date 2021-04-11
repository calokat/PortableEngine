#ifdef _WIN64
#pragma once
#include <d3d11.h>
#include "IImageGraphicsData.h"
struct DirectX11ImageGraphicsData : public IImageGraphicsData
{
	ID3D11ShaderResourceView* srv;
	void* GetData()
	{
		return srv;
	}
};
#endif