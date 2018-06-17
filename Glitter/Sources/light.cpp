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
	if(!m_lights.empty())
	{
		_shader.bind("uLightPos",   m_lights[0].getPosition());
		_shader.bind("uLightColor", m_lights[0].getColor());
	}
}


//static
std::unique_ptr<Mesh> LightContainer::buildCube()
{
	//auto lamp = std::make_unique<Mesh>(PROJECT_SOURCE_DIR "/resources/models/sheep.obj");
	//return lamp;

	glm::vec3 positions[] =
	{
		{-0.5f, 0.5f,  0.5f}, {0.5f, 0.5f,  0.5f}, {0.5f, -0.5f,  0.5f}, {-0.5f, -0.5f,  0.5f},
		{-0.5f, 0.5f, -0.5f}, {0.5f, 0.5f, -0.5f}, {0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f},
	};

	std::vector<Vertex> vertices;
	vertices.resize(8);
	
	for(int i = 0; i < 8; ++i)
		vertices[i].position = positions[i];

	return std::unique_ptr<Mesh>(new Mesh(std::move(vertices),
		{
			0, 1, 2, 0, 2, 3, // front
			0, 4, 1, 4, 5, 1, // top
			1, 5, 6, 1, 6, 2, // right
			2, 6, 7, 2, 7, 3, // bottom
			0, 3, 7, 0, 7, 4, // left
			//4, 7, 6, 4, 6, 5  // back
			6, 7, 4, 5, 6, 4  // back
		},
		{}));
}
