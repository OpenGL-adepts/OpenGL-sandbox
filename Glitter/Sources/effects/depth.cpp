#include "depth.hpp"
#include <imgui.h>


Depth::Depth()
{
	m_shader.attach(PROJECT_SOURCE_DIR "/resources/shaders/depth.vert");
	m_shader.attach(PROJECT_SOURCE_DIR "/resources/shaders/depth.frag");
	m_shader.link();
}


const char* Depth::getName() const //override
{
	return "Depth";
}


void Depth::config() //override
{
	ImGui::SliderInt("Depth exponent", &m_exponent, 1, 64);
}


void Depth::render(const Scene& _scene, const Camera& _camera, const glm::mat4& _perspective) const //override
{
	m_shader.activate();
	m_shader.bind("uProjection", _perspective);
	m_shader.bind("uView", _camera.getViewMatrix());
	m_shader.bind("uDepthExponent", m_exponent);
	_scene.draw(m_shader);
}
