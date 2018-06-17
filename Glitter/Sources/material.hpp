#pragma once
#include "shader.hpp"
#include <glm/glm.hpp>


class Material
{
public:
	void bind(const Shader& _shader) const;
	void config();

	glm::vec3 getAmbient() const;
	glm::vec3 getDiffuse() const;
	glm::vec3 getSpecular() const;
	float getShininess() const;
	void setAmbient(const glm::vec3& _color);
	void setDiffuse(const glm::vec3& _color);
	void setSpecular(const glm::vec3& _color);
	void setShininess(float _shin);

protected:
	glm::vec3 m_ambient  = glm::vec3(1.f);
	glm::vec3 m_diffuse  = glm::vec3(1.f);
	glm::vec3 m_specular = glm::vec3(1.f);
	float m_shininess    = 32;
};
