#include "normal.hpp"
#include "../config.hpp"


Normal::Normal()
{
	loadShader(RELATIVE_PATH_ROOT "/resources/shaders/normals.vert", RELATIVE_PATH_ROOT "/resources/shaders/normals.frag");
}


std::string Normal::getName() const //override
{
	return "Normal";
}

std::string Normal::getTutorialPath() const //override
{
	return RELATIVE_PATH_ROOT "/resources/tutorials/Normal.pdf";
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
