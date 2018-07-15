#include "lambert.hpp"
#include "../config.hpp"


Lambert::Lambert()
{
	loadShader(RELATIVE_PATH_ROOT "/resources/shaders/lambert.vert", RELATIVE_PATH_ROOT "/resources/shaders/lambert.frag");
}


std::string Lambert::getName() const //override
{
	return "Lambert";
}

std::string Lambert::getTutorialPath() const //override
{
	return RELATIVE_PATH_ROOT "/resources/tutorials/Lambert.pdf";

}

void Lambert::doConfig() //override
{
	ImGui::SliderFloat("Ambient", &m_ambientStrength, 0.f, 1.f);
	ImGui::SliderFloat("Diffuse", &m_diffuseStrength, 0.f, 1.f);
	ImGui::Checkbox("Normal mapping", &m_bEnableNormalMapping);
}


void Lambert::doRender(const Scene& _scene, const Camera& _camera, const glm::mat4& _perspective) const //override
{
	m_shader.activate();
	m_shader.bind("uAmbientStrength", m_ambientStrength);
	m_shader.bind("uDiffuseStrength", m_diffuseStrength);
	m_shader.bind("uEnableNormalMapping", (int)m_bEnableNormalMapping);
	_scene.bindLights(m_shader);
	_scene.draw(m_shader, _camera, _perspective);
}
