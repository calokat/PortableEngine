#pragma once
#include <entt.hpp>
#include "AABB.h"
#include "Camera.h"
#include "Transform.h"
#include "GameWindow.h"

void RaycastAgainstAABB(glm::vec3 rayOrigin, glm::vec3 rayDir, entt::basic_view<entt::entity, entt::exclude_t<>, AABB> aabbView);
void MakeRayFromCamera(entt::basic_view<entt::entity, entt::exclude_t<>, Camera, Transform> cameras, entt::basic_view<entt::entity, entt::exclude_t<>, AABB> aabbs, GameWindow* window);