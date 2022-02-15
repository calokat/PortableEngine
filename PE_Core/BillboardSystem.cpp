#include "BillboardSystem.h"
#include "TransformSystem.h"
#include "glm/gtc/quaternion.hpp"
void BillboardSystem::SetBillboardTransforms(entt::basic_view<entt::entity, entt::get_t<Billboard, Transform>, entt::exclude_t<>, void> billboardView, Camera renderCamera)
{
	glm::mat3 inverseCameraRot = glm::inverse((glm::mat3)renderCamera.view);
	for (auto& billboard : billboardView)
	{
		Transform& billboardTransform = billboardView.get<Transform>(billboard);
		billboardTransform.orientation = glm::quat_cast(inverseCameraRot);
		TransformSystem::CalculateWorldMatrix(&billboardTransform);
	}
}