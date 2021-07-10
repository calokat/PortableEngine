#pragma once
#include <entt.hpp>
#include <unordered_map>
struct Relationship
{
	entt::entity parent;
	std::unordered_map<int, entt::entity> children;
};