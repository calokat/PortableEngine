#include "LightsSystem.h"
#include "Relationship.h"
#include "MeshMaker.h"
#include "BillboardSystem.h"
#include "misc_components.h"
#include "AABBSystem.h"
namespace LightsSystem
{
	void ExtractLightsFromRegistry(entt::registry& reg, DirectionalLight* dirLights, PointLight* pointLights)
	{
		const auto dirLightView = reg.view<DirectionalLight>();
		int i = 0;
		for (auto it = dirLightView.begin(); it != dirLightView.end() && i < MAX_DIR_LIGHTS; ++it, ++i)
		{
			dirLights[i] = dirLightView.get<DirectionalLight>(*it);
		}

		auto pointLightView = reg.view<PointLight>();
		i = 0;
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
		reg.emplace<Name>(res, "Point Light");
		return res;
	}
	entt::entity CreateDirectionalLight(entt::registry& reg)
	{
		entt::entity res = reg.create();
		DirectionalLight& dl = reg.emplace<DirectionalLight>(res);
		dl.AmbientColor = glm::vec4(0);
		dl.DiffuseColor = glm::vec4(1);
		reg.emplace<Name>(res, "Dir Light");
		return res;
	}
	void LoadPointLightPositions(entt::basic_view<entt::entity, entt::get_t<PointLight, Transform>, entt::exclude_t<>, void> pointLights)
	{
		for (entt::entity entity : pointLights)
		{
			auto [pointLight, pointLightTransform] = pointLights.get(entity);
			pointLight.Position = pointLightTransform.worldMatrix[3];
		}
	}
	void LoadDirLightDirections(entt::basic_view<entt::entity, entt::get_t<DirectionalLight, Transform>, entt::exclude_t<>, void> dirLights)
	{
		for (entt::entity entity : dirLights)
		{
			auto [dirLight, dirLightTransform] = dirLights.get(entity);
			dirLight.Direction = glm::vec4(dirLightTransform.worldMatrix[0][2], dirLightTransform.worldMatrix[1][2], dirLightTransform.worldMatrix[2][2], 0);
		}
	}
}