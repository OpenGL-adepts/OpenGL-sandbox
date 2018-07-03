#include "phong.hpp"
#include "../config.hpp"


Phong::Phong()
{
	loadShader(RELATIVE_PATH_ROOT "/resources/shaders/phong.vert", RELATIVE_PATH_ROOT "/resources/shaders/phong.frag");
}


std::string Phong::getName() const //override
{
	return "Phong";
}

std::string Phong::getTutorialPath() const //override
{
	return RELATIVE_PATH_ROOT "/resources/tutorials/Phong.pdf";

}

void Phong::doConfig() //override
{
	ImGui::Checkbox("Blinn-Phong", &m_bUseBlinnPhong);
	ImGui::SliderFloat("Ambient", &m_ambientStrength, 0.f, 1.f);
	ImGui::SliderFloat("Diffuse", &m_diffuseStrength, 0.f, 1.f);
	ImGui::SliderFloat("Specular", &m_specularStrength, 0.f, 1.f);
	ImGui::Checkbox("Normal mapping", &m_bEnableNormalMapping);
}


void Phong::doRender(const Scene& _scene, const Camera& _camera, const glm::mat4& _perspective) const //override
{
	m_shader.activate();
	m_shader.bind("uAmbientStrength", m_ambientStrength);
	m_shader.bind("uDiffuseStrength", m_diffuseStrength);
	m_shader.bind("uSpecularStrength", m_specularStrength);
	m_shader.bind("uEnableNormalMapping", (int)m_bEnableNormalMapping);
	m_shader.bind("uUseBlinnPhong", (int)m_bUseBlinnPhong);
	_scene.bindLights(m_shader);
	_scene.draw(m_shader, _camera, _perspective);
}
