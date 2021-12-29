#pragma once
#include <entt.hpp>
#include "AABB.h"
#include "Camera.h"
#include "Transform.h"
#include "GameWindow.h"

void RaycastAgainstAABB(glm::vec3 rayOrigin, glm::vec3 rayDir, entt::basic_view<entt::entity, entt::exclude_t<>, AABB, Transform> aabbView);
void MakeRayFromCamera(Camera camera, entt::basic_view<entt::entity, entt::exclude_t<>, AABB, Transform> aabbs, GameWindow* window, glm::vec2 screenCoordinates);