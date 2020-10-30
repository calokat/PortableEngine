#pragma once
#include <entt.hpp>
#include <json.hpp>
// Huge thanks to https://skypjack.github.io/2019-06-25-ecs-baf-part-4/
struct Relationship 
{
	//std::size_t children{};
	entt::entity first{ entt::null };
	entt::entity prev{ entt::null };
	entt::entity next{ entt::null };
	entt::entity parent{ entt::null };
};