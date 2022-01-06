#pragma once
#include "Transform.h"
#include "Camera.h"
#include <entt.hpp>

struct Billboard
{
private:
	bool dontIgnore;
};

namespace BillboardSystem
{
	void SetBillboardTransforms(entt::basic_view<entt::entity, entt::exclude_t<>, Billboard, Transform> billboardView, Camera renderCamera);
}