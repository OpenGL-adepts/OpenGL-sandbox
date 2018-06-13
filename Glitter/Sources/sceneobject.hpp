#pragma once
#include "mesh.hpp"
#include <glm/glm.hpp>
#include <string>


class SceneObject
{
public:
	void draw(GLuint _shader);
	bool loadFromFile(const std::string& _path);

	void setRotation(glm::vec3 _angles);
	void setPosition(glm::vec3 _pos);
	void setScale(glm::vec3 _scale);
	const std::string& getModelPath() const;
	glm::mat4 getModelMatrix() const;

protected:
	Mesh m_model;
	std::string m_modelPath;
	glm::vec3 m_position = glm::vec3(0.f);
	glm::vec3 m_scale = glm::vec3(1.f);
	glm::vec3 m_rotation = glm::vec3(0.f);
};
