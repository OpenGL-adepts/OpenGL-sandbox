#include "light.hpp"
#include <imgui.h>


Light::Light()
{
	m_displayName = "point light";
}


void Light::config()
{
	ImGui::ColorEdit3("Color", (float*)&m_color);
	ImGui::SliderFloat("X", &m_position.x, -20.f, 20.f);
	ImGui::SliderFloat("Y", &m_position.y, -20.f, 20.f);
	ImGui::SliderFloat("Z", &m_position.z, -20.f, 20.f);
}


void Light::fromJSON(const nlohmann::json& _json) //override
{
	SceneObject::fromJSON(_json);

	m_color = JSON::loadVector3(_json, "lightColor", glm::vec3(1.f));
}


glm::vec3 Light::getColor() const
{
	return m_color;
}


void Light::saveToJSON(nlohmann::json& _json, const std::string& _savePath) const //override
{
	_json["lightColor"] = {m_color.x, m_color.y, m_color.z};
}
