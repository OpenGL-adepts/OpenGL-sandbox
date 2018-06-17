#pragma once
#include "sceneobject.hpp"
#include <glm/glm.hpp>


class Light
	: public SceneObject
{
public:
	Light();
	void config() override;
	void fromJSON(const nlohmann::json& _json) override;
	glm::vec3 getColor() const;

protected:
	void saveToJSON(nlohmann::json& _json, const std::string& _savePath) const override;

protected:
	glm::vec3 m_color = glm::vec3(1.f);
};

