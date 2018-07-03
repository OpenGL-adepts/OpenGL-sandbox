#include "material.hpp"
#include <imgui.h>


void Material::bind(const Shader& _shader) const
{
	_shader.bind("uMaterial.color", m_color);
	_shader.bind("uMaterial.ambient", m_ambient);
	_shader.bind("uMaterial.diffuse", m_diffuse);
	_shader.bind("uMaterial.specular", m_specular);
	_shader.bind("uMaterial.shininess", m_shininess);
	_shader.bind("uMaterial.roughness", m_roughness);
}


void Material::config()
{
	ImGui::ColorEdit3("Base color", (float*)&m_color);
	ImGui::ColorEdit3("Ambient",    (float*)&m_ambient);
	ImGui::ColorEdit3("Diffuse",    (float*)&m_diffuse);
	ImGui::ColorEdit3("Specular",   (float*)&m_specular);
	ImGui::SliderFloat("Shininess", &m_shininess, 2, 256);
	ImGui::SliderFloat("Roughness (Oren-Nayar)", &m_roughness, 0, 1);
}


nlohmann::json Material::toJSON() const
{
	nlohmann::json mat;
	mat["baseColor"] = {m_color.x,	  m_color.y,    m_color.z};
	mat["ambient"]	 = {m_ambient.x,  m_ambient.y,  m_ambient.z};
	mat["diffuse"]	 = {m_diffuse.x,  m_diffuse.y,  m_diffuse.z};
	mat["specular"]	 = {m_specular.x, m_specular.y, m_specular.z};
	mat["shininess"] = m_shininess;
	mat["roughness"] = m_roughness;
	return mat;
}


void Material::fromJSON(const nlohmann::json& _json)
{
	m_color		= JSON::loadVector3(_json, "baseColor", glm::vec3(1.f));
	m_ambient	= JSON::loadVector3(_json, "ambient",   glm::vec3(1.f));
	m_diffuse	= JSON::loadVector3(_json, "diffuse",   glm::vec3(1.f));
	m_specular  = JSON::loadVector3(_json, "specular",  glm::vec3(1.f));
	try { m_shininess = _json.at("shininess"); } catch(...) { m_shininess = 32; }
	try { m_roughness = _json.at("roughness"); } catch(...) { m_roughness = 0; }
}


glm::vec3 Material::getColor() const
{
	return m_color;
}


glm::vec3 Material::getAmbient() const
{
	return m_ambient;
}


glm::vec3 Material::getDiffuse() const
{
	return m_diffuse;
}


glm::vec3 Material::getSpecular() const
{
	return m_specular;
}


float Material::getShininess() const
{
	return m_shininess;
}


void Material::setColor(const glm::vec3& _color)
{
	m_color = _color;
}


void Material::setAmbient(const glm::vec3& _color)
{
	m_ambient = _color;
}


void Material::setDiffuse(const glm::vec3& _color)
{
	m_diffuse = _color;
}


void Material::setSpecular(const glm::vec3& _color)
{
	m_specular = _color;
}


void Material::setShininess(float _shin)
{
	m_shininess = _shin;
}

