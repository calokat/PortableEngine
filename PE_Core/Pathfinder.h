#pragma once
#include "PEString.h"
#include "PEWideString.h"
struct PathfinderConfig
{
	const char* pathToRuntime = nullptr;
};

class Pathfinder
{
public:
	static void Configure(PathfinderConfig cfg);
	static PE::String GetRuntimePath();
	static PE::WideString GetRuntimePath_Wide();
private:
	Pathfinder(PathfinderConfig);
	Pathfinder();
	static Pathfinder staticThis;
	PathfinderConfig config;
};