#include "LightsSystem.h"
#include "Relationship.h"
#include "MeshMaker.h"
#include "BillboardSystem.h"
#include "misc_components.h"
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
		reg.emplace<Name>(res, "Light Test");
		Transform& plTransform = reg.emplace<Transform>(res);
		reg.emplace<Relationship>(res);
		entt::entity billBoardEntity = MakeMesh(billBoardCreateInfo, reg, res);
		reg.emplace<Billboard>(billBoardEntity);
		Relationship& billBoardRel = reg.get<Relationship>(billBoardEntity);
		for (auto it = billBoardRel.children.begin(); it != billBoardRel.children.end(); ++it)
		{
			AttachRenderers(reg, renderSystem, assetManager->GetAssetPath("../../Assets/Images/light_bulb.jpg").c_str(), it->second, ShaderType::Unlit_Textured);
		}
		return res;
	}
}