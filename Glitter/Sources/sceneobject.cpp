#include "SceneObject.hpp"
#include <imgui.h>
#include <filesystem>


void SceneObject::config()
{
	if(ImGui::TreeNode("Position"))
	{
		ImGui::SliderFloat("X", &m_position.x, -20.f, 20.f);
		ImGui::SliderFloat("Y", &m_position.y, -20.f, 20.f);
		ImGui::SliderFloat("Z", &m_position.z, -20.f, 20.f);
		ImGui::TreePop();
	}

	if(ImGui::TreeNode("Rotation"))
	{
		ImGui::SliderAngle("X", &m_rotation.x);
		ImGui::SliderAngle("Y", &m_rotation.y);
		ImGui::SliderAngle("Z", &m_rotation.z);
		ImGui::TreePop();
	}

	if(ImGui::TreeNode("Scale"))
	{
		ImGui::SliderFloat("X", &m_scale.x, 0.01f, 20.f);
		ImGui::SliderFloat("Y", &m_scale.y, 0.01f, 20.f);
		ImGui::SliderFloat("Z", &m_scale.z, 0.01f, 20.f);

		float objScale = m_scale.x;

		if(ImGui::SliderFloat("uniform", &objScale, 0.01f, 20.f))
			m_scale = glm::vec3(objScale);

		ImGui::TreePop();
	}

	doConfig();
}


nlohmann::json SceneObject::toJSON(const std::string& _savePath) const
{
	nlohmann::json tmp;
	
	tmp["name"] = m_displayName;
	tmp["enabled"] = m_bEnabled;
	tmp["position"] = {m_position.x, m_position.y, m_position.z};
	tmp["rotation"] = {m_rotation.x, m_rotation.y, m_rotation.z};
	tmp["scale"]	= {m_scale.x,    m_scale.y,	   m_scale.z};
	saveToJSON(tmp, _savePath);

	return tmp;
}


void SceneObject::fromJSON(const nlohmann::json& _json)
{
	try { setDisplayName(_json.at("name").get<std::string>()); } catch(...) {}
	try { setEnabled(_json.at("enabled")); } catch (...) {}
	
	setPosition (JSON::loadVector3(_json, "position"));
	setRotation (JSON::loadVector3(_json, "rotation"));
	setScale	(JSON::loadVector3(_json, "scale", glm::vec3(1.f)));
}


void SceneObject::setRotation(glm::vec3 _angles)
{
	m_rotation = _angles;
}


void SceneObject::setPosition(glm::vec3 _pos)
{
	m_position = _pos;
}


void SceneObject::setScale(glm::vec3 _scale)
{
	m_scale = _scale;
}


void SceneObject::setDisplayName(std::string _name)
{
	m_displayName = std::move(_name);
}


void SceneObject::setEnabled(bool _enabled)
{
	m_bEnabled = _enabled;
}


const std::string& SceneObject::getDisplayName() const
{
	return m_displayName;
}


glm::mat4 SceneObject::getModelMatrix() const
{
	glm::mat4 model = doGetModelMatrix();
	glm::mat4 rot = glm::rotate(glm::mat4(1.f), m_rotation.x, glm::vec3(1.f, 0.f, 0.f));
	rot = glm::rotate(rot, m_rotation.y, glm::vec3(0.f, 1.f, 0.f));
	rot = glm::rotate(rot, m_rotation.z, glm::vec3(0.f, 0.f, 1.f));
	return glm::translate(glm::mat4(1.f), m_position) * glm::scale(rot, m_scale) * model;
}


glm::vec3 SceneObject::getPosition() const
{
	return m_position;
}


glm::vec3 SceneObject::getRotation() const
{
	return m_rotation;
}


glm::vec3 SceneObject::getScale() const
{
	return m_scale;
}


bool SceneObject::isEnabled() const
{
	return m_bEnabled;
}


glm::mat4 SceneObject::doGetModelMatrix() const
{
	return glm::mat4(1.f);
}
