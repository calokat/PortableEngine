#pragma once
#include "IPlatform.h"
class EmscriptenPlatform :
	public IPlatform
{
	int InitWindow();
	long Run();
};

