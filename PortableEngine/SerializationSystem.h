#pragma once
#include <json.hpp>
#include <glm/glm.hpp>
#include "Transform.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Camera.h"
#include "misc_components.h"

void to_json(nlohmann::json& j, const glm::vec3& vec)
{
	j = nlohmann::json{ {"x", vec.x}, {"y", vec.y}, {"z", vec.z} };
}

void from_json(const nlohmann::json& j, glm::vec3& vec)
{
	j.at("x").get_to(vec.x);
	j.at("y").get_to(vec.y);
	j.at("z").get_to(vec.z);
}

void to_json(nlohmann::json& j, const glm::vec4& vec)
{
	j = nlohmann::json{ {"x", vec.x}, {"y", vec.y}, {"z", vec.z}, {"w", vec.w} };
}

void from_json(const nlohmann::json& j, glm::vec4& vec)
{
	j.at("x").get_to(vec.x);
	j.at("y").get_to(vec.y);
	j.at("z").get_to(vec.z);
	j.at("w").get_to(vec.w);
}


void to_json(nlohmann::json& j, const glm::mat4& mat)
{
	j = nlohmann::json
	{
		{"00", mat[0][0]}, {"01", mat[0][1]}, {"02", mat[0][2]}, {"03", mat[0][3]},
		{"10", mat[1][0]}, {"11", mat[1][1]}, {"12", mat[1][2]}, {"13", mat[1][3]},
		{"20", mat[2][0]}, {"21", mat[2][1]}, {"22", mat[2][2]}, {"23", mat[2][3]},
		{"30", mat[3][0]}, {"31", mat[3][1]}, {"32", mat[3][2]}, {"33", mat[3][3]},
	};
}

void from_json(const nlohmann::json& j, glm::mat4& mat)
{
	j.at("00").get_to(mat[0][0]);
	j.at("01").get_to(mat[0][1]);
	j.at("02").get_to(mat[0][2]);
	j.at("03").get_to(mat[0][3]);
	j.at("10").get_to(mat[1][0]);
	j.at("11").get_to(mat[1][1]);
	j.at("12").get_to(mat[1][2]);
	j.at("13").get_to(mat[1][3]);
	j.at("20").get_to(mat[2][0]);
	j.at("21").get_to(mat[2][1]);
	j.at("22").get_to(mat[2][2]);
	j.at("23").get_to(mat[2][3]);
	j.at("30").get_to(mat[3][0]);
	j.at("31").get_to(mat[3][1]);
	j.at("32").get_to(mat[3][2]);
	j.at("33").get_to(mat[3][3]);
}

void to_json(nlohmann::json& j, const Transform& t)
{
	nlohmann::json worldJson, posJson, rotJson, scaleJson;
	to_json(worldJson, t.worldMatrix);
	to_json(posJson, t.position);
	to_json(rotJson, t.rotation);
	to_json(scaleJson, t.scale);
	j["worldMatrix"] = worldJson;
	j["position"] = posJson;
	j["rotation"] = rotJson;
	j["scale"] = scaleJson;
}

void from_json(const nlohmann::json& j, Transform& t)
{
	from_json(j["worldMatrix"], t.worldMatrix);
	from_json(j["position"], t.position);
	from_json(j["rotation"], t.rotation);
	from_json(j["scale"], t.scale);
}

void to_json(nlohmann::json& j, const Renderer& r)
{
	j["vertexShaderPath"] = r.vertexShaderPath;
	j["fragmentShaderPath"] = r.fragmentShaderPath;
	nlohmann::json colorJson;
	to_json(colorJson, r.vertexColor);
	j["vertexColor"] = colorJson;
}

void from_json(const nlohmann::json& j, Renderer& r)
{
	r = Renderer(j["vertexShaderPath"], j["fragmentShaderPath"]);
	r.vertexShaderPath = j["vertexShaderPath"];
	r.fragmentShaderPath = j["fragmentShaderPath"];
	from_json(j["vertexColor"], r.vertexColor);
}

void to_json(nlohmann::json& j, const Mesh& m)
{
	j["path"] = m.path;
}

void from_json(const nlohmann::json& j, Mesh& m)
{
	std::string path = j["path"];
	m = Mesh(j["path"].get<std::string>().c_str());
}

void to_json(nlohmann::json& j, const Camera& c)
{
	to_json(j["view"], c.view);
	to_json(j["projection"], c.projection);
	j["fieldOfView"] = c.fieldOfView;
	j["nearPlaneDistance"] = c.nearPlaneDistance;
	j["farPlaneDistance"] = c.farPlaneDistance;
	j["movementSpeed"] = c.movementSpeed;
}

void from_json(const nlohmann::json& j, Camera& c)
{
	from_json(j["view"], c.view);
	from_json(j["projection"], c.projection);
	c.fieldOfView = j["fieldOfView"];
	c.nearPlaneDistance = j["nearPlaneDistance"];
	c.farPlaneDistance = j["farPlaneDistance"];
	c.movementSpeed = j["movementSpeed"];
}

void to_json(nlohmann::json& j, const Name& n)
{
	j["nameString"] = n.nameString;
}

void from_json(const nlohmann::json& j, Name& n)
{
	n.nameString = j["nameString"];
}

void to_json(nlohmann::json& j, const Rotator& r)
{
	to_json(j["deltaRot"], r.deltaRot);
}

void from_json(const nlohmann::json& j, Rotator& r)
{
	from_json(j["deltaRot"], r.deltaRot);
}