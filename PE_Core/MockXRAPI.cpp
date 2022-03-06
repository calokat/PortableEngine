#include "MockXRAPI.h"

MockXRAPI::MockXRAPI()
{
	
}

void MockXRAPI::PollEvents()
{
}

bool MockXRAPI::IsSessionRunning()
{
	return false;
}

void MockXRAPI::Frame(entt::registry& reg, IRenderSystem* renderSystem)
{
}
