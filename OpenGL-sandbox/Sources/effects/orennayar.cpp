#include "orennayar.hpp"
#include "../config.hpp"


OrenNayar::OrenNayar()
{
	loadShader(RELATIVE_PATH_ROOT "/resources/shaders/oren-nayar.vert", RELATIVE_PATH_ROOT "/resources/shaders/oren-nayar.frag");
}


std::string OrenNayar::getName() const //override
{
	return "Oren-Nayar";
}

std::string OrenNayar::getTutorialPath() const //override
{
	return "";

}

void OrenNayar::doConfig() //override
{
	ImGui::SliderFloat("Ambient", &m_ambientStrength, 0.f, 1.f);
	ImGui::SliderFloat("Diffuse", &m_diffuseStrength, 0.f, 1.f);
	ImGui::Checkbox("Normal mapping", &m_bEnableNormalMapping);
}


void OrenNayar::doRender(const Scene& _scene, const Camera& _camera, const glm::mat4& _perspective) const //override
{
	m_shader.activate();
	m_shader.bind("uAmbientStrength", m_ambientStrength);
	m_shader.bind("uDiffuseStrength", m_diffuseStrength);
	m_shader.bind("uEnableNormalMapping", (int)m_bEnableNormalMapping);
	_scene.bindLights(m_shader);
	_scene.draw(m_shader, _camera, _perspective);
}
