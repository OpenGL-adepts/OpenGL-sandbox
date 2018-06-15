#include "normal.hpp"
#include <imgui.h>


Normal::Normal()
{
	loadShader(PROJECT_SOURCE_DIR "/resources/shaders/normals.vert", PROJECT_SOURCE_DIR "/resources/shaders/normals.frag");
}


std::string Normal::getName() const //override
{
	return "Normal";
}


void Normal::config() //override
{
	ImGui::Checkbox("Normal mapping", &m_bEnableNormalMapping);
}


void Normal::doRender(const Scene& _scene, const Camera& _camera, const glm::mat4& _perspective) const //override
{
	m_shader.activate();
	m_shader.bind("uProjection", _perspective);
	m_shader.bind("uView", _camera.getViewMatrix());
	m_shader.bind("uEnableNormalMapping", (int)m_bEnableNormalMapping);
	_scene.draw(m_shader);
}
