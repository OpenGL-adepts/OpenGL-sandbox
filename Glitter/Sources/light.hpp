#pragma once
#include "shader.hpp"
#include "mesh.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <memory>


class Light
{
public:
	glm::vec3 getPosition() const;
	glm::vec3 getColor() const;
	void config();

protected:
	glm::vec3 m_position = glm::vec3(5.f, 0, 0);
	glm::vec3 m_color	 = glm::vec3(1.f);
};


class LightContainer
{
public:
	LightContainer();
	void draw();

protected:
	static std::unique_ptr<Mesh> buildCube();

protected:
	Shader m_lampShader;
	std::unique_ptr<Mesh> m_lamp;
	std::vector<Light> m_lights;
};
