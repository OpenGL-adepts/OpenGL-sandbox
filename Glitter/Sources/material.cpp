#include "material.hpp"
#include <imgui.h>


void Material::bind(const Shader& _shader) const
{
	_shader.bind("uMaterial.ambient", m_ambient);
	_shader.bind("uMaterial.diffuse", m_diffuse);
	_shader.bind("uMaterial.specular", m_specular);
	_shader.bind("uMaterial.shininess", m_shininess);
}


void Material::config()
{
	ImGui::ColorEdit3("Ambient",  (float*)&m_ambient);
	ImGui::ColorEdit3("Diffuse",  (float*)&m_diffuse);
	ImGui::ColorEdit3("Specular", (float*)&m_specular);
	ImGui::SliderFloat("Shininess", &m_shininess, 2, 256);
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

