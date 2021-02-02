#pragma once
#include <entt.hpp>
#include "misc_components.h"
#include "Transform.h"
class EntityListWindow
{
public:
	void Render(entt::basic_view<entt::entity, entt::exclude_t<>, Name, Transform> namedEntities);
};

