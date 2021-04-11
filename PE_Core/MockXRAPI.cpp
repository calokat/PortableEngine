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

void MockXRAPI::RenderFrame(entt::registry& reg, IRenderSystem* renderSystem)
{
}
