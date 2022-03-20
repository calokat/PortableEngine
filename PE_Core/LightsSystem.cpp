#include "LightsSystem.h"
#include "Relationship.h"
#include "MeshMaker.h"
#include "BillboardSystem.h"
#include "misc_components.h"
#include "AABBSystem.h"
namespace LightsSystem
{
	void ExtractLightsFromRegistry(entt::registry& reg, DirectionalLight* dirLights, PointLight* pointLights, SpotLight* spotLights)
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

		auto spotLightView = reg.view<SpotLight>();
		i = 0;
		for (auto it = spotLightView.begin(); it != spotLightView.end() && i < MAX_SPOT_LIGHTS; ++it, ++i)
		{
			spotLights[i] = spotLightView.get<SpotLight>(*it);
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
	entt::entity CreateSpotLight(entt::registry& reg)
	{
		entt::entity res = reg.create();
		SpotLight& sl = reg.emplace<SpotLight>(res);
		sl.AmbientColor = glm::vec4(0);
		sl.DiffuseColor = glm::vec4(0, 1, 0, 1);
		sl.Angle = 30;
		reg.emplace<Name>(res, "Spot Light");
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
	void LoadSpotLightTransform(entt::basic_view<entt::entity, entt::get_t<SpotLight, Transform>, entt::exclude_t<>, void> spotLights)
	{
		for (entt::entity entity : spotLights)
		{
			auto [spotLight, spotLightTransform] = spotLights.get(entity);
			spotLight.Position = spotLightTransform.worldMatrix[3];
			spotLight.LightFwd = spotLightTransform.worldMatrix[2];
		}
	}
}