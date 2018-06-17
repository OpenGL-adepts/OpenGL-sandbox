#pragma once
#include "shader.hpp"
#include <glm/glm.hpp>


class Material
{
public:
	void bind(const Shader& _shader) const;
	void config();

protected:
	glm::vec3 m_ambient  = glm::vec3(1.f);
	glm::vec3 m_diffuse  = glm::vec3(1.f);
	glm::vec3 m_specular = glm::vec3(1.f);
	float m_shininess    = 32;
};
