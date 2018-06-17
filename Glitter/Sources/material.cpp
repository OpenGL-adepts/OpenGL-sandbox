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
