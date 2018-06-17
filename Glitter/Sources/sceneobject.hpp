#pragma once
#include "mesh.hpp"
#include "material.hpp"
#include "json.hpp"
#include <glm/glm.hpp>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <filesystem>


// Base class for actors and lights
class SceneObject
{
public:
	virtual ~SceneObject() {}
	virtual void draw(const Shader& _shader) {}
	virtual void config();
	
	nlohmann::json toJSON(const std::string& _savePath) const;
	virtual void fromJSON(const nlohmann::json& _json);

	void setRotation(glm::vec3 _angles);
	void setPosition(glm::vec3 _pos);
	void setScale(glm::vec3 _scale);
	void setDisplayName(std::string _name);
	void setEnabled(bool _enabled);
	const std::string& getDisplayName() const;
	glm::mat4 getModelMatrix() const;
	glm::vec3 getPosition() const;
	glm::vec3 getRotation() const;
	glm::vec3 getScale() const;
	bool isEnabled() const;

protected:
	virtual void doConfig() {}
	virtual void saveToJSON(nlohmann::json& _json, const std::string& _savePath) const {}
	virtual glm::mat4 doGetModelMatrix() const;

protected:
	bool m_bEnabled = true;
	std::string m_displayName;
	glm::vec3 m_position = glm::vec3(0.f);
	glm::vec3 m_scale	 = glm::vec3(1.f);
	glm::vec3 m_rotation = glm::vec3(0.f);
};
