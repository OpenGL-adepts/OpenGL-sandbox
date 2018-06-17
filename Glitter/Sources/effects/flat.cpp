#include "flat.hpp"


Flat::Flat()
{
	loadShader(PROJECT_SOURCE_DIR "/resources/shaders/flat.vert", PROJECT_SOURCE_DIR "/resources/shaders/flat.frag");
}


std::string Flat::getName() const //override
{
	return "Flat";
}

std::string Flat::getTutorialPath() const //override
{
	return PROJECT_SOURCE_DIR "/resources/tutorials/Flat.pdf";
}


void Flat::doConfig() //override
{
	ImGui::SliderFloat("Ambient", &m_ambientStrength, 0.f, 1.f);
	ImGui::SliderFloat("Diffuse", &m_diffuseStrength, 0.f, 1.f);
	ImGui::SliderFloat("Specular", &m_specularStrength, 0.f, 1.f);
}


void Flat::doRender(const Scene& _scene, const Camera& _camera, const glm::mat4& _perspective) const //override
{
	m_shader.activate();
	m_shader.bind("uAmbientStrength", m_ambientStrength);
	m_shader.bind("uDiffuseStrength", m_diffuseStrength);
	m_shader.bind("uSpecularStrength", m_specularStrength);
	m_shader.bind("uViewPos", _camera.getPosition());
	_scene.bindLights(m_shader);
	_scene.draw(m_shader, _camera, _perspective);
}