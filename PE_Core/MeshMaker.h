#pragma once
#include <entt.hpp>
#include "Tree.h"
#include "MeshLoaderSystem.h"
#include "IRenderSystem.h"
#include "IAssetManager.h"
#include "glm/common.hpp"

entt::entity MakeMesh(Tree<MeshCreateInfo> meshScene, entt::registry& registry, IRenderSystem* renderSystem, IAssetManager* assetManager, entt::entity meshRoot, glm::vec3 pos = glm::vec3(0, 0, 0));
entt::entity MakeMesh_Recursive(entt::registry& registry, Tree<MeshCreateInfo> scene, IRenderSystem* renderSystem, IAssetManager* assetManager, Camera& renderingCam, Transform& renderingCamTransform, entt::entity parent);