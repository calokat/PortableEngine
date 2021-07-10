#pragma once
#include <entt.hpp>
#include "misc_components.h"
#include "Transform.h"
#include "Tree.h"
#include "Relationship.h"

class EntityListWindow
{
public:
	void Render(Relationship& rootRel, entt::basic_view<entt::entity, entt::exclude_t<>, Name, Relationship> nameView);
private:
	void SetUpGuiTree(entt::entity parent, entt::basic_view<entt::entity, entt::exclude_t<>, Name, Relationship> nameView);
};
