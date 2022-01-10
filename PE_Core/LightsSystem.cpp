#include "LightsSystem.h"
#include "Relationship.h"
#include "MeshMaker.h"
#include "BillboardSystem.h"
#include "misc_components.h"
#include "AABBSystem.h"
namespace LightsSystem
{
	void LightsSystem::ExtractLightsFromRegistry(entt::registry& reg, DirectionalLight& dirLight, PointLight* pointLights)
	{
		const auto dirLightView = reg.view<DirectionalLight>();
		dirLight = dirLightView.get<DirectionalLight>(dirLightView.front());

		auto pointLightView = reg.view<PointLight>();
		int i = 0;
		for (auto it = pointLightView.begin(); it != pointLightView.end() && i < MAX_POINT_LIGHTS; ++it, ++i)
		{
			pointLights[i] = pointLightView.get<PointLight>(*it);
		}
	}
	entt::entity CreatePointLight(entt::registry& reg, Tree<MeshCreateInfo> billBoardCreateInfo, IRenderSystem* renderSystem, IAssetManager* assetManager)
	{
		entt::entity res = reg.create();
		PointLight& pl = reg.emplace<PointLight>(res);
		pl.DiffuseColor = glm::vec4(1, 1, 1, 1);
		pl.AmbientColor = glm::vec4(.3, .2, .3, 1);
		reg.emplace<Name>(res, "Light Test");
		Transform& plTransform = reg.emplace<Transform>(res);
		reg.emplace<Relationship>(res);
		entt::entity billBoardEntity = MakeMesh_Recursive(reg, billBoardCreateInfo, res, false);
		reg.emplace<Billboard>(billBoardEntity);
		Relationship& billBoardRel = reg.get<Relationship>(billBoardEntity);
		for (auto it = billBoardRel.children.begin(); it != billBoardRel.children.end(); ++it)
		{
			AttachRenderers(reg, renderSystem, assetManager->GetAssetPath("../../Assets/Images/light_bulb.jpg").c_str(), it->second, ShaderType::Unlit_Textured);
		}
		Mesh planeMesh = billBoardCreateInfo.children[0].data.m;
		AABB bb = {};
		AABBSystem::UpdateAABB(bb, planeMesh, plTransform);
		reg.emplace<AABB>(res, bb);
		return res;
	}
	void LoadPointLightPositions(entt::basic_view<entt::entity, entt::exclude_t<>, PointLight, Transform> pointLights)
	{
		for (entt::entity entity : pointLights)
		{
			auto [pointLight, pointLightTransform] = pointLights.get(entity);
			pointLight.Position = pointLightTransform.worldMatrix[3];
		}
	}
}