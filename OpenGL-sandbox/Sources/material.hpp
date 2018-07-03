#pragma once
#include "shader.hpp"
#include "json.hpp"
#include <glm/glm.hpp>


class Material
{
public:
	void bind(const Shader& _shader) const;
	void config();

	nlohmann::json toJSON() const;
	void fromJSON(const nlohmann::json& _json);

	glm::vec3 getColor() const;
	glm::vec3 getAmbient() const;
	glm::vec3 getDiffuse() const;
	glm::vec3 getSpecular() const;
	float getShininess() const;
	void setColor(const glm::vec3& _color);
	void setAmbient(const glm::vec3& _color);
	void setDiffuse(const glm::vec3& _color);
	void setSpecular(const glm::vec3& _color);
	void setShininess(float _shin);

protected:
	glm::vec3 m_color    = glm::vec3(1.f);
	glm::vec3 m_ambient  = glm::vec3(1.f);
	glm::vec3 m_diffuse  = glm::vec3(1.f);
	glm::vec3 m_specular = glm::vec3(1.f);
	float m_shininess    = 32;
	float m_roughness    = 0.1f;
};
