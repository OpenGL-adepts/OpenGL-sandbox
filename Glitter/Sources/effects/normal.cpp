#include "normal.hpp"


Normal::Normal()
{
	loadShader(PROJECT_SOURCE_DIR "/resources/shaders/normals.vert", PROJECT_SOURCE_DIR "/resources/shaders/normals.frag");
}


std::string Normal::getName() const //override
{
	return "Normal";
}

std::string Normal::getTutorialPath() const //override
{
	return PROJECT_SOURCE_DIR "/resources/tutorials/Normal.pdf";
}

void Normal::doConfig() //override
{
	ImGui::Checkbox("Normal mapping", &m_bEnableNormalMapping);
}


void Normal::doRender(const Scene& _scene, const Camera& _camera, const glm::mat4& _perspective) const //override
{
	m_shader.activate();
	m_shader.bind("uEnableNormalMapping", (int)m_bEnableNormalMapping);
	_scene.draw(m_shader, _camera, _perspective);
}
