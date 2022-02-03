#pragma once
#include <entt.hpp>
#include "Tree.h"
#include "MeshLoaderSystem.h"
#include "IRenderSystem.h"
#include "IAssetManager.h"
#include "glm/common.hpp"

entt::entity MakeMesh(Tree<MeshCreateInfo> meshScene, entt::registry& registry, entt::entity meshRoot);
entt::entity MakeMesh_Recursive(entt::registry& registry, Tree<MeshCreateInfo> scene, entt::entity parent, bool attachAABBs = true);
void AttachRenderers(entt::registry& registry, IRenderSystem* renderSystem, std::map<const char*, const char*> texturePaths, entt::entity rootEntity, ShaderType shaderType);