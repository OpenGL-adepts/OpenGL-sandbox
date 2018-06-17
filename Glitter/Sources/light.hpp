#pragma once
#include "shader.hpp"
#include "mesh.hpp"
#include "camera.hpp"
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
	// Number of light uniforms in shader
	static constexpr int MaxLights = 16;

	LightContainer();
	void draw(const Camera& _camera, const glm::mat4& _perspective) const;
	void config();
	void bind(const Shader& _shader) const;

protected:
	Shader m_lampShader;
	std::unique_ptr<Mesh> m_lamp;
	std::vector<Light> m_lights;
};
