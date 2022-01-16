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
	entt::entity CreatePointLight(entt::registry& reg)
	{
		entt::entity res = reg.create();
		PointLight& pl = reg.emplace<PointLight>(res);
		pl.DiffuseColor = glm::vec4(1, 1, 1, 1);
		pl.AmbientColor = glm::vec4(.3, .2, .3, 1);
		reg.emplace<Name>(res, "Light Test");
		Transform& plTransform = reg.emplace<Transform>(res);
		reg.emplace<Relationship>(res);
		return res;
	}
	entt::entity CreateDirectionalLight(entt::registry& reg)
	{
		entt::entity res = reg.create();
		DirectionalLight& dl = reg.emplace<DirectionalLight>(res);
		dl.AmbientColor = glm::vec4(0);
		dl.DiffuseColor = glm::vec4(1);
		reg.emplace<Name>(res, "Dir Light");
		Transform& dlTransform = reg.emplace<Transform>(res);
		reg.emplace<Relationship>(res);
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
	void LoadDirLightDirections(entt::basic_view<entt::entity, entt::exclude_t<>, DirectionalLight, Transform> dirLights)
	{
		for (entt::entity entity : dirLights)
		{
			auto [dirLight, dirLightTransform] = dirLights.get(entity);
			dirLight.Direction = glm::vec4(dirLightTransform.worldMatrix[0][2], dirLightTransform.worldMatrix[1][2], dirLightTransform.worldMatrix[2][2], 0);
		}
	}
}