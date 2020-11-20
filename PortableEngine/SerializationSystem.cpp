#include "SerializationSystem.h"
using json = nlohmann::json;
void SerializationSystem::to_json(json& j, const glm::vec3& vec)
{
	j = json{ {"x", vec.x}, {"y", vec.y}, {"z", vec.z} };
}

void SerializationSystem::from_json(const json& j, glm::vec3& vec)
{
	j.at("x").get_to(vec.x);
	j.at("y").get_to(vec.y);
	j.at("z").get_to(vec.z);
}

void SerializationSystem::to_json(json& j, const glm::mat4& mat)
{
	j = json
	{
		{"00", mat[0][0]}, {"01", mat[0][1]}, {"02", mat[0][2]}, {"03", mat[0][3]},
		{"10", mat[1][0]}, {"11", mat[1][1]}, {"12", mat[1][2]}, {"13", mat[1][3]},
		{"20", mat[2][0]}, {"21", mat[2][1]}, {"22", mat[2][2]}, {"23", mat[2][3]},
		{"30", mat[3][0]}, {"31", mat[3][1]}, {"32", mat[3][2]}, {"33", mat[3][3]},
	};
}

void SerializationSystem::from_json(const json& j, glm::mat4& mat)
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

void SerializationSystem::to_json(json& j, const Transform& t)
{
	json worldJson, posJson, rotJson, scaleJson;
	to_json(worldJson, t.worldMatrix);
	to_json(posJson, t.position);
	to_json(rotJson, t.rotation);
	to_json(scaleJson, t.scale);
	j["worldMatrix"] = worldJson;
	j["position"] = posJson;
	j["rotation"] = rotJson;
	j["scale"] = scaleJson;
}

void SerializationSystem::from_json(const json& j, Transform& t)
{
	from_json(j["worldMatrix"], t.worldMatrix);
	from_json(j["position"], t.position);
	from_json(j["rotation"], t.rotation);
	from_json(j["scale"], t.scale);
}

void SerializationSystem::to_json(json& j, const Renderer& r)
{
	j["vertexShaderPath"] = r.vertexShaderPath;
	j["fragmentShaderPath"] = r.fragmentShaderPath;
}

void SerializationSystem::from_json(const json& j, Renderer& r)
{
	r = Renderer(j["vertexShaderPath"], j["fragmentShaderPath"]);
	r.vertexShaderPath = j["vertexShaderPath"];
	r.fragmentShaderPath = j["fragmentShaderPath"];
}

void SerializationSystem::to_json(json& j, const Mesh& m)
{
	j["path"] = m.path;
}

void SerializationSystem::from_json(const json& j, Mesh& m)
{
	std::string path = j["path"];
	m = Mesh(j["path"].get<std::string>().c_str());
}

void SerializationSystem::to_json(json& j, const Camera& c)
{
	to_json(j["view"], c.view);
	to_json(j["projection"], c.projection);
	j["fieldOfView"] = c.fieldOfView;
	j["nearPlaneDistance"] = c.nearPlaneDistance;
	j["farPlaneDistance"] = c.farPlaneDistance;
	j["movementSpeed"] = c.movementSpeed;
}

void SerializationSystem::from_json(const json& j, Camera& c)
{
	from_json(j["view"], c.view);
	from_json(j["projection"], c.projection);
	c.fieldOfView = j["fieldOfView"];
	c.nearPlaneDistance = j["nearPlaneDistance"];
	c.farPlaneDistance = j["farPlaneDistance"];
	c.movementSpeed = j["movementSpeed"];
}
