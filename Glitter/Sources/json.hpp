#pragma once
#include <json.hpp>
#include <glm/glm.hpp>


// Helpers for nlohmann::json
struct JSON
{
	static glm::vec3 loadVector3(const nlohmann::json& _obj, const std::string& _vectName, const glm::vec3& _default = glm::vec3(0.f));
};
