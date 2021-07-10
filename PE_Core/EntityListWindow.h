#pragma once
#include <entt.hpp>
#include "misc_components.h"
#include "Transform.h"
#include "Scene.h"

class EntityListWindow
{
public:
	void Render(Scene<entt::entity> namedEntities, entt::basic_view<entt::entity, entt::exclude_t<>, Name> nameView);
private:
	void SetUpGuiTree(Scene<entt::entity> entityTree, entt::basic_view<entt::entity, entt::exclude_t<>, Name> nameView);
};

