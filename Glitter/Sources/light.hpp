#pragma once
#include "sceneobject.hpp"
#include <glm/glm.hpp>


class Light
	: public SceneObject
{
public:
	static constexpr float DefaultAttConstant  = 1.f;
	static constexpr float DefaultAttLinear    = 0.09f;
	static constexpr float DefaultAttQuadratic = 0.032f;

	Light();
	void config() override;
	void fromJSON(const nlohmann::json& _json) override;
	glm::vec3 getColor() const;
	float getAttenuationConstant() const;
	float getAttenuationLinear() const;
	float getAttenuationQuadratic() const;

protected:
	void saveToJSON(nlohmann::json& _json, const std::string& _savePath) const override;

protected:
	glm::vec3 m_color	 = glm::vec3(1.f);
	float m_attConstant	 = DefaultAttConstant;
	float m_attLinear	 = DefaultAttLinear;
	float m_attQuadratic = DefaultAttQuadratic;
};

