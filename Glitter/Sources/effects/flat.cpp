#include "flat.hpp"


Flat::Flat()
{
	loadShader(PROJECT_SOURCE_DIR "/resources/shaders/flat.vert", PROJECT_SOURCE_DIR "/resources/shaders/flat.frag");
}


std::string Flat::getName() const //override
{
	return "Flat";
}

std::string Flat::getTutorialPage() const //override
{
	return PROJECT_SOURCE_DIR "/resources/pages/flat.html";
}


void Flat::doConfig() //override
{
	ImGui::SliderFloat("Ambient", &m_ambientStrength, 0.f, 1.f);
	ImGui::SliderFloat("Diffuse", &m_diffuseStrength, 0.f, 1.f);
	ImGui::SliderFloat("Specular", &m_specularStrength, 0.f, 1.f);
	ImGui::SliderInt("Specular exponent", &m_specularExponent, 2, 256);
}


void Flat::doRender(const Scene& _scene, const Camera& _camera, const glm::mat4& _perspective) const //override
{
	m_shader.activate();
	m_shader.bind("uProjection", _perspective);
	m_shader.bind("uView", _camera.getViewMatrix());
	m_shader.bind("uAmbientStrength", m_ambientStrength);
	m_shader.bind("uDiffuseStrength", m_diffuseStrength);
	m_shader.bind("uSpecularStrength", m_specularStrength);
	m_shader.bind("uSpecularExponent", m_specularExponent);
	m_shader.bind("uLightPos", glm::vec3(5.f, 0, 0));
	m_shader.bind("uViewPos", _camera.getPosition());
	_scene.draw(m_shader);
}