#pragma once
#include <entt/entt.hpp>
#include "Tree.h"
#include "MeshLoaderSystem.h"
#include "IRenderSystem.h"
#include "IAssetManager.h"
#include "glm/common.hpp"

entt::entity MakeMesh(Tree<MeshCreateInfo> meshScene, entt::registry& registry, entt::entity meshRoot);
entt::entity MakeMesh_Recursive(entt::registry& registry, Tree<MeshCreateInfo> scene, entt::entity parent, bool attachAABBs = true);
