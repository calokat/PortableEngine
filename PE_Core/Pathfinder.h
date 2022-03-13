#pragma once
#include "PEString.h"

struct PathfinderConfig
{
	const char* pathToRuntime = nullptr;
};

class Pathfinder
{
public:
	static void Configure(PathfinderConfig cfg);
	static PE::String GetRuntimePath();
private:
	Pathfinder(PathfinderConfig);
	Pathfinder();
	static Pathfinder staticThis;
	PathfinderConfig config;
};