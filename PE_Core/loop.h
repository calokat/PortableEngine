#pragma once
#include "Camera.h"
#include <entt.hpp>
#include "IRenderSystem.h"
#include "IPlatform.h"
#include "IGraphicsAPI.h"
#include "../PE_XR/IXRAPI.h"
#include "GameWindow.h"
#include "misc_components.h"
#include "Tree.h"
#include "AABB.h"
template<class T>
void DrawIteration(Camera& camera, entt::entity selected, entt::registry& registry, IRenderSystem* renderSystem);
void Loop(IPlatform* plat, IGraphicsAPI* graph, IRenderSystem* renderSystem, IXRAPI* xr, GameWindow* window, entt::registry& registry, Options options, entt::entity sceneRoot);
void ComputeTransformHeirarchy(entt::entity parent, entt::registry& registry, Transform cumulativeTransform = Transform());