#include "schlick.hpp"
#include "../config.hpp"


Schlick::Schlick()
{
	loadShader(RELATIVE_PATH_ROOT "/resources/shaders/schlick.vert", RELATIVE_PATH_ROOT "/resources/shaders/schlick.frag");
}


std::string Schlick::getName() const //override
{
	return "Schlick";
}

std::string Schlick::getTutorialPath() const //override
{
	return "";

}

void Schlick::doConfig() //override
{
	ImGui::SliderFloat("Ambient", &m_ambientStrength, 0.f, 1.f);
	ImGui::SliderFloat("Diffuse", &m_diffuseStrength, 0.f, 1.f);
	ImGui::SliderFloat("Specular", &m_specularStrength, 0.f, 1.f);
	ImGui::Checkbox("Normal mapping", &m_bEnableNormalMapping);
}


void Schlick::doRender(const Scene& _scene, const Camera& _camera, const glm::mat4& _perspective) const //override
{
	m_shader.activate();
	m_shader.bind("uAmbientStrength", m_ambientStrength);
	m_shader.bind("uDiffuseStrength", m_diffuseStrength);
	m_shader.bind("uSpecularStrength", m_specularStrength);
	m_shader.bind("uEnableNormalMapping", (int)m_bEnableNormalMapping);
	_scene.bindLights(m_shader);
	_scene.draw(m_shader, _camera, _perspective);
}
