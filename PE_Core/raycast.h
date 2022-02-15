#pragma once
#include <entt/entt.hpp>
#include "AABB.h"
#include "Camera.h"
#include "Transform.h"
#include "GameWindow.h"

void RaycastAgainstAABB(glm::vec3 rayOrigin, glm::vec3 rayDir, entt::basic_view<entt::entity, entt::get_t<AABB, Transform>, entt::exclude_t<>, void> aabbView);
void MakeRayFromCamera(Camera camera, entt::basic_view<entt::entity, entt::get_t<AABB, Transform>, entt::exclude_t<>, void> aabbs, GameWindow* window, glm::vec2 screenCoordinates);