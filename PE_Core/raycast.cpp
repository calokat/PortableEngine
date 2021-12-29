#include "raycast.h"
#include "GizmoSystem.h"
#include "AABBSystem.h"

void RaycastAgainstAABB(glm::vec3 rayOrigin, glm::vec3 rayDir, entt::basic_view<entt::entity, entt::exclude_t<>, AABB, Transform> aabbView)
{
	for (auto& ae : aabbView)
	{
		AABB aabb = aabbView.get<AABB>(ae);
		Transform t = aabbView.get<Transform>(ae);

		aabb.min = aabb.min * t.scale;
		aabb.max = aabb.max * t.scale;

		if (t.rotation != glm::vec3(0))
		{
			Mesh m = AABBSystem::GenerateMeshFromAABB(aabb);
			for (auto it = m.rawVertices.begin(); it != m.rawVertices.end(); ++it)
			{
				it->Position = it->Position * t.orientation;
			}
			AABBSystem::UpdateAABB(aabb, m, t);
		}

		aabb.min = aabb.min + t.position;
		aabb.max = aabb.max + t.position;

		// Thank you https://gdbooks.gitbooks.io/3dcollisions/content/Chapter3/raycast_aabb.html
		float t1 = (aabb.min.x - rayOrigin.x) / rayDir.x;
		float t2 = (aabb.max.x - rayOrigin.x) / rayDir.x;
		float t3 = (aabb.min.y - rayOrigin.y) / rayDir.y;
		float t4 = (aabb.max.y - rayOrigin.y) / rayDir.y;
		float t5 = (aabb.min.z - rayOrigin.z) / rayDir.z;
		float t6 = (aabb.max.z - rayOrigin.z) / rayDir.z;


		float tmin = glm::max(glm::max(glm::min(t1, t2), glm::min(t3, t4)), glm::min(t5, t6));
		float tmax = glm::min(glm::min(glm::max(t1, t2), glm::max(t3, t4)), glm::max(t5, t6));

		// if tmax < 0, ray (line) is intersecting AABB, but whole AABB is behind us
		if (tmax >= 0 && tmin <= tmax)
		{
			GizmoSystem::DeselectAll();
			GizmoSystem::Select(ae);
			return;
		}

	}
}

void MakeRayFromCamera(Camera camera, entt::basic_view<entt::entity, entt::exclude_t<>, AABB, Transform> aabbs, GameWindow* window, glm::vec2 screenCoordinates)
{
	//auto camView = registry.view<Camera>();
	
	// Blessed be this code taken from https://gamedev.stackexchange.com/questions/157674/simple-mouseray-picking-in-opengl
	glm::vec3 mouse_world_nearplane = glm::unProject(
		glm::vec3(screenCoordinates.x, window->height - screenCoordinates.y, 0.0f),
		camera.view, //view matrix
		camera.projection,
		glm::ivec4(0, 0, window->width, window->height));

	glm::vec3 mouse_world_farplane = glm::unProject(
		glm::vec3(screenCoordinates.x, window->height - screenCoordinates.y, 1.0f),
		camera.view, //view matrix
		camera.projection,
		glm::ivec4(0, 0, window->width, window->height));

	glm::vec3 camray = glm::normalize(mouse_world_farplane - mouse_world_nearplane);
	camray *= 30;
	glm::vec3 newMeshLocation = mouse_world_nearplane + camray;

	RaycastAgainstAABB(mouse_world_nearplane, mouse_world_farplane - mouse_world_nearplane, aabbs);
	return;
}
