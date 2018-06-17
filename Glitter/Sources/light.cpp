#include "light.hpp"
#include <imgui.h>


glm::vec3 Light::getPosition() const
{
	return m_position;
}


glm::vec3 Light::getColor() const
{
	return m_color;
}


void Light::config()
{
	ImGui::ColorEdit3("Color", (float*)&m_color);
	ImGui::SliderFloat("X", &m_position.x, -20.f, 20.f);
	ImGui::SliderFloat("Y", &m_position.y, -20.f, 20.f);
	ImGui::SliderFloat("Z", &m_position.z, -20.f, 20.f);
}


LightContainer::LightContainer()
	: m_lamp(new Mesh(PROJECT_SOURCE_DIR "/resources/models/sheep.obj"))
{
	try
	{
		m_lampShader.attach(PROJECT_SOURCE_DIR "/resources/shaders/lamp.vert");
		m_lampShader.attach(PROJECT_SOURCE_DIR "/resources/shaders/lamp.frag");
		m_lampShader.link();
	}
	catch(...) {}

	//TODO:
	m_lights.emplace_back();
	m_lights.emplace_back();
}


void LightContainer::draw(const Camera& _camera, const glm::mat4& _perspective) const
{
	m_lampShader.activate();
	m_lampShader.bind("uProjection", _perspective);
	m_lampShader.bind("uView", _camera.getViewMatrix());

	for(auto& light : m_lights)
	{
		m_lampShader.bind("uModel", glm::scale(glm::translate(glm::mat4(1.f), light.getPosition()), glm::vec3(0.25f)));
		m_lampShader.bind("uLightColor", light.getColor());
		m_lamp->draw(m_lampShader);
	}
}


void LightContainer::config()
{
	if(!m_lights.empty())
		m_lights[0].config();
}


void LightContainer::bind(const Shader& _shader) const
{
	char tmp[64];

	for(size_t i = 0; i < m_lights.size(); ++i)
	{
		sprintf(tmp, "uLight[%u].active",   i); _shader.bind(tmp, 1);
		sprintf(tmp, "uLight[%u].position", i); _shader.bind(tmp, m_lights[i].getPosition());
		sprintf(tmp, "uLight[%u].color",    i); _shader.bind(tmp, m_lights[i].getColor());
	}

	for(size_t i = m_lights.size(); i < MaxLights; ++i)
	{
		sprintf(tmp, "uLight[%u].active",   i); _shader.bind(tmp, 0);
	}
}
