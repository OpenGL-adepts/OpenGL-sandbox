#include "normal.hpp"
#include <imgui.h>


Normal::Normal()
{
	m_shader.attach(PROJECT_SOURCE_DIR "/resources/shaders/normals.vert");
	m_shader.attach(PROJECT_SOURCE_DIR "/resources/shaders/normals.frag");
	m_shader.link();
}


const char* Normal::getName() const //override
{
	return "Normal";
}


void Normal::render(const Scene& _scene, const Camera& _camera, const glm::mat4& _perspective) const //override
{
	m_shader.activate();
	m_shader.bind("uProjection", _perspective);
	m_shader.bind("uView", _camera.getViewMatrix());
	_scene.draw(m_shader);
}
