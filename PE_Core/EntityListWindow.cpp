#include "EntityListWindow.h"
#include <imgui.h>
#include "GizmoSystem.h"
#include "TransformSystem.h"
#include <glm/common.hpp>
void EntityListWindow::Render(Relationship& rootRel, entt::basic_view<entt::entity, entt::exclude_t<>, Name, Relationship, Transform> nameView)
{
	ImGui::Begin("Entity List");
	ImGui::SetWindowPos({ 0, 20 });
	ImGui::SetWindowSize({ 200, 780 });
	// the root entity likely does not have a name, start with its children
	for (auto it = rootRel.children.begin(); it != rootRel.children.end(); ++it)
	{
		SetUpGuiTree(it->second, nameView);
	}

	ImGui::End();
}

void EntityListWindow::SetUpGuiTree(entt::entity parent, entt::basic_view<entt::entity, entt::exclude_t<>, Name, Relationship, Transform> nameView)
{
	Name entityName = nameView.get<Name>(parent);
	Relationship& parentRel = nameView.get<Relationship>(parent);
	if (ImGui::TreeNodeEx(entityName.nameString.c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | (parentRel.children.size() == 0 ? ImGuiTreeNodeFlags_Leaf : 0)))
	{
		if (ImGui::IsItemClicked())
		{
			GizmoSystem::DeselectAll();
			GizmoSystem::Select(parent);
		}
		ImGui::PushID((int)parent);
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
		{
			ChildMoveInfo info = { parentRel, parent };
			ImGui::SetDragDropPayload("CHILD_MOVE", &info, sizeof(ChildMoveInfo));
			ImGui::EndDragDropSource();
		}
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CHILD_MOVE"))
			{
				ChildMoveInfo* infoPayload = (ChildMoveInfo*)payload->Data;
				Relationship& childRel = nameView.get<Relationship>(infoPayload->child);
				if (childRel.parent != parent)
				{
					parentRel.children.insert(std::pair((int)infoPayload->child, infoPayload->child));
					childRel.parent = parent;
					Relationship& formerParentRel = nameView.get<Relationship>(infoPayload->oldChildRel.parent);
					formerParentRel.children.erase((int)infoPayload->child);


					Transform& childTransform = nameView.get<Transform>(infoPayload->child);
					Transform& parentTransform = nameView.get<Transform>(parent);
					const Transform& oldParentTransform = nameView.get<Transform>(infoPayload->oldChildRel.parent);
					glm::vec3 oldParDeg = glm::degrees(oldParentTransform.rotation);
					glm::vec3 parDeg = glm::degrees(parentTransform.rotation);
					glm::vec3 childDeg = glm::degrees(childTransform.rotation);
					Transform transientTransform;
					transientTransform.worldMatrix = glm::inverse(parentTransform.worldMatrix) * childTransform.worldMatrix;
					TransformSystem::DecomposeTransform(transientTransform, transientTransform);
					glm::vec3 transientDeg = glm::degrees(transientTransform.rotation);
					childTransform.position = transientTransform.position;
					//childTransform.orientation = glm::inverse(parentTransform.orientation) * childTransform.orientation * oldParentTransform.orientation;
					childTransform.orientation = glm::inverse(transientTransform.orientation);
					childTransform.rotation = glm::eulerAngles(childTransform.orientation);
					//childTransform.rotation = -transientTransform.rotation;
					//childTransform.rotation = glm::eulerAngles(childTransform.orientation);
					childTransform.scale = transientTransform.scale;
				}
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::PopID();
		for (auto it = parentRel.children.begin(); it != parentRel.children.end(); ++it)
		{
			SetUpGuiTree(it->second, nameView);
		}
		ImGui::TreePop();
	}
}
