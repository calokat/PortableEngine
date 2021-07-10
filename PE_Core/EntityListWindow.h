#pragma once
#include <entt.hpp>
#include "misc_components.h"
#include "Transform.h"
#include "Tree.h"

class EntityListWindow
{
public:
	void Render(Tree<entt::entity> namedEntities, entt::basic_view<entt::entity, entt::exclude_t<>, Name> nameView);
private:
	void SetUpGuiTree(Tree<entt::entity> entityTree, entt::basic_view<entt::entity, entt::exclude_t<>, Name> nameView);
};

