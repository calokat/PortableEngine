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
template<class T>
void DrawIteration(Camera& camera, entt::entity selected, entt::registry& registry, IRenderSystem* renderSystem);
void Loop(IPlatform* plat, IGraphicsAPI* graph, IRenderSystem* renderSystem, IXRAPI* xr, GameWindow* window, entt::registry& registry, Options options, Tree<entt::entity> entityGraph);
void ComputeTransformHeirarchy(Tree<entt::entity> tree, entt::registry& registry, Transform cumulativeTransform = Transform());