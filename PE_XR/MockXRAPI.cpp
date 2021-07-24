#include "MockXRAPI.h"

MockXRAPI::MockXRAPI()
{
	
}

void MockXRAPI::PollEvents()
{
}

bool MockXRAPI::IsSessionRunning()
{
	return true;
}

void MockXRAPI::Frame(entt::registry& reg, IRenderSystem* renderSystem)
{
}
