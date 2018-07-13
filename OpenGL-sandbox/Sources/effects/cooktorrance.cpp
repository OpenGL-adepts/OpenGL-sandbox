#include "cooktorrance.hpp"
#include "../config.hpp"


CookTorrance::CookTorrance()
{
	loadShader(RELATIVE_PATH_ROOT "/resources/shaders/cooktorrance.vert", RELATIVE_PATH_ROOT "/resources/shaders/cooktorrance.frag");
}


std::string CookTorrance::getName() const //override
{
	return "Cook-Torrance";
}

std::string CookTorrance::getTutorialPath() const //override
{
	return RELATIVE_PATH_ROOT "/resources/tutorials/Cook-Torrance.pdf";

}

void CookTorrance::doConfig() //override
{
	ImGui::SliderFloat("Ambient", &m_ambientStrength, 0.f, 1.f);
	ImGui::SliderFloat("Specular", &m_specularStrength, 0.f, 1.f);
	ImGui::Checkbox("Normal mapping", &m_bEnableNormalMapping);
}


void CookTorrance::doRender(const Scene& _scene, const Camera& _camera, const glm::mat4& _perspective) const //override
{
	m_shader.activate();
	m_shader.bind("uAmbientStrength", m_ambientStrength);
	m_shader.bind("uSpecularStrength", m_specularStrength);
	m_shader.bind("uEnableNormalMapping", (int)m_bEnableNormalMapping);
	_scene.bindLights(m_shader);
	_scene.draw(m_shader, _camera, _perspective);
}
