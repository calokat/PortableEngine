#pragma once
#include <entt/entt.hpp>
#include "misc_components.h"
#include "Transform.h"
#include "Tree.h"
#include "Relationship.h"
struct ChildMoveInfo
{
	Relationship oldChildRel;
	entt::entity child;
};
class EntityListWindow
{
public:
	void Render(Relationship& rootRel, entt::basic_view<entt::entity, entt::get_t<Name, Relationship, Transform>, entt::exclude_t<>, void> nameView);
private:
	void SetUpGuiTree(entt::entity parent, entt::basic_view<entt::entity, entt::get_t<Name, Relationship, Transform>, entt::exclude_t<>, void> nameView);
};
