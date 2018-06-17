#include "gouraud.hpp"


Gouraud::Gouraud()
{
	loadShader(PROJECT_SOURCE_DIR "/resources/shaders/gouraud.vert", PROJECT_SOURCE_DIR "/resources/shaders/gouraud.frag");
}


std::string Gouraud::getName() const //override
{
	return "Gouraud";
}

std::string Gouraud::getTutorialPath() const //override
{
	return PROJECT_SOURCE_DIR "/resources/tutorials/Gouraud.pdf";
}

void Gouraud::doConfig() //override
{
	ImGui::SliderFloat("Ambient", &m_ambientStrength, 0.f, 1.f);
	ImGui::SliderFloat("Diffuse", &m_diffuseStrength, 0.f, 1.f);
	ImGui::SliderFloat("Specular", &m_specularStrength, 0.f, 1.f);
}


void Gouraud::doRender(const Scene& _scene, const Camera& _camera, const glm::mat4& _perspective) const //override
{
	m_shader.activate();
	m_shader.bind("uAmbientStrength", m_ambientStrength);
	m_shader.bind("uDiffuseStrength", m_diffuseStrength);
	m_shader.bind("uSpecularStrength", m_specularStrength);
	m_shader.bind("uViewPos", _camera.getPosition());
	_scene.bindLights(m_shader);
	_scene.draw(m_shader, _camera, _perspective);
}