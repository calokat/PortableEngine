#include "Pathfinder.h"

Pathfinder Pathfinder::staticThis;

void Pathfinder::Configure(PathfinderConfig cfg)
{
	if (staticThis.config.pathToRuntime != nullptr)
	{
		throw "Pathfinder has already been configured";
		return;
	}
	staticThis.config = cfg;
}

PE::String Pathfinder::GetRuntimePath()
{
	return PE::String(staticThis.config.pathToRuntime);
}

Pathfinder::Pathfinder()
{

}