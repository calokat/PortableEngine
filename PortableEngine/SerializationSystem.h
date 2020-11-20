#pragma once
#include <json.hpp>
#include <glm/glm.hpp>
#include "Transform.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Camera.h"

class SerializationSystem
{
	static void to_json(nlohmann::json& j, const glm::vec3& vec);
	static void from_json(const nlohmann::json& j, glm::vec3& vec);
	static void to_json(nlohmann::json& j, const glm::mat4& mat);
	static void from_json(const nlohmann::json& j, glm::mat4& mat);
	static void to_json(nlohmann::json& j, const Transform& t);
	static void from_json(const nlohmann::json& j, Transform& t);
	static void to_json(nlohmann::json& j, const Renderer& r);
	static void from_json(const nlohmann::json& j, Renderer& r);
	static void to_json(nlohmann::json& j, const Mesh& m);
	static void from_json(const nlohmann::json& j, Mesh& m);
	static void to_json(nlohmann::json& j, const Camera& c);
	static void from_json(const nlohmann::json& j, Camera& c);
};

