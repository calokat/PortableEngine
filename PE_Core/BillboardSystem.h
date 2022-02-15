#pragma once
#include "Transform.h"
#include "Camera.h"
#include "entt/entt.hpp"

struct Billboard
{
private:
	bool dontIgnore;
};

namespace BillboardSystem
{
	void SetBillboardTransforms(entt::basic_view<entt::entity, entt::get_t<Billboard, Transform>, entt::exclude_t<>, void> billboardView, Camera renderCamera);
}