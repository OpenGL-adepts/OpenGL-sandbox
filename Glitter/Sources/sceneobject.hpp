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
	void setDisplayName(std::string _name);
	void setEnabled(bool _enabled);
	const std::string& getModelPath() const;
	const std::string& getDisplayName() const;
	glm::mat4 getModelMatrix() const;
	glm::vec3 getPosition() const;
	glm::vec3 getRotation() const;
	glm::vec3 getScale() const;
	bool isEnabled() const;

protected:
	Mesh m_model;
	bool m_bEnabled = true;
	std::string m_modelPath;
	std::string m_displayName;
	glm::vec3 m_position = glm::vec3(0.f);
	glm::vec3 m_scale = glm::vec3(1.f);
	glm::vec3 m_rotation = glm::vec3(0.f);
};
