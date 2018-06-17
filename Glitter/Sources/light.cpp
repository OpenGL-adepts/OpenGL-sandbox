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
	ImGui::Text("Attenuation");
	ImGui::SliderFloat("constant",  &m_attConstant,  0.1f, 2.f);
	ImGui::SliderFloat("linear",    &m_attLinear,    0.0f, 2.f);
	ImGui::SliderFloat("quadratic", &m_attQuadratic, 0.0f, 2.f);
}


void Light::fromJSON(const nlohmann::json& _json) //override
{
	SceneObject::fromJSON(_json);

	m_color = JSON::loadVector3(_json, "lightColor", glm::vec3(1.f));

	try { m_attConstant  = _json.at("attConstant");  } catch(...) { m_attConstant  = DefaultAttConstant;  }
	try { m_attLinear    = _json.at("attLinear");    } catch(...) { m_attLinear    = DefaultAttLinear;    }
	try { m_attQuadratic = _json.at("attQuadratic"); } catch(...) { m_attQuadratic = DefaultAttQuadratic; }
}


glm::vec3 Light::getColor() const
{
	return m_color;
}


float Light::getAttenuationConstant() const
{
	return m_attConstant;
}


float Light::getAttenuationLinear() const
{
	return m_attLinear;
}


float Light::getAttenuationQuadratic() const
{
	return m_attQuadratic;
}


void Light::saveToJSON(nlohmann::json& _json, const std::string& _savePath) const //override
{
	_json["lightColor"]   = {m_color.x, m_color.y, m_color.z};
	_json["attConstant"]  = m_attConstant;
	_json["attLinear"]    = m_attLinear;
	_json["attQuadratic"] = m_attQuadratic;
}
