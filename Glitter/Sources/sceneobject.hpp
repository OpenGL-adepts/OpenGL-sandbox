#pragma once
#include "mesh.hpp"
#include "material.hpp"
#include <glm/glm.hpp>
#include <string>
#include <glm/gtc/matrix_transform.hpp>


class SceneObject
{
public:
	void draw(const Shader& _shader);
	bool loadFromFile(const std::string& _path);
	void config();

	void setRotation(glm::vec3 _angles);
	void setPosition(glm::vec3 _pos);
	void setScale(glm::vec3 _scale);
	void setDisplayName(std::string _name);
	void setEnabled(bool _enabled);
	void setEnableTextures(bool _bTextures);
	const std::string& getModelPath() const;
	const std::string& getDisplayName() const;
	glm::mat4 getModelMatrix() const;
	glm::vec3 getPosition() const;
	glm::vec3 getRotation() const;
	glm::vec3 getScale() const;
	glm::vec3 getColor() const;
	bool isEnabled() const;
	bool isTextureEnabled() const;

protected:
	Mesh m_model;
	Material m_material;
	bool m_bEnabled = true;
	bool m_bEnableTextures = true;
	std::string m_modelPath;
	std::string m_displayName;
	glm::vec3 m_position = glm::vec3(0.f);
	glm::vec3 m_scale	 = glm::vec3(1.f);
	glm::vec3 m_rotation = glm::vec3(0.f);
};
