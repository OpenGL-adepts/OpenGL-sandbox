#include "SceneObject.hpp"


void SceneObject::draw(const Shader& _shader)
{
	
	if(m_bEnabled)
		m_model.draw(_shader, m_color, m_bEnableTextures);
}


bool SceneObject::loadFromFile(const std::string& _path)
{
	m_modelPath = _path;

	auto i = m_modelPath.find_last_of("/\\");
	m_displayName = i != std::string::npos ? m_modelPath.substr(i + 1) : m_modelPath;

	return m_model.loadFromFile(m_modelPath);
}

bool SceneObject::setCustomTextureFromFile(const std::string& _path)
{
	auto path = std::filesystem::path(_path).make_preferred().string();
	return m_model.setCustomTextureFromFile(path);
}

void SceneObject::setRotation(glm::vec3 _angles)
{
	m_rotation = _angles;
}


void SceneObject::setPosition(glm::vec3 _pos)
{
	m_position = _pos;
}


void SceneObject::setScale(glm::vec3 _scale)
{
	m_scale = _scale;
}


void SceneObject::setDisplayName(std::string _name)
{
	m_displayName = std::move(_name);
}


void SceneObject::setEnabled(bool _enabled)
{
	m_bEnabled = _enabled;
}


void SceneObject::setColor(glm::vec3 _color)
{
	m_color = _color;
}


void SceneObject::setEnableTextures(bool _bTextures)
{
	m_bEnableTextures = _bTextures;
}


const std::string& SceneObject::getModelPath() const
{
	return m_modelPath;
}


const std::string& SceneObject::getDisplayName() const
{
	return m_displayName;
}


glm::mat4 SceneObject::getModelMatrix() const
{
	glm::mat4 model = m_model.centerAndNormalize();
	glm::mat4 rot = glm::rotate(glm::mat4(1.f), m_rotation.x, glm::vec3(1.f, 0.f, 0.f));
	rot = glm::rotate(rot, m_rotation.y, glm::vec3(0.f, 1.f, 0.f));
	rot = glm::rotate(rot, m_rotation.z, glm::vec3(0.f, 0.f, 1.f));
	return glm::translate(glm::mat4(1.f), m_position) * glm::scale(rot, m_scale) * model;
}


glm::vec3 SceneObject::getPosition() const
{
	return m_position;
}


glm::vec3 SceneObject::getRotation() const
{
	return m_rotation;
}


glm::vec3 SceneObject::getScale() const
{
	return m_scale;
}


glm::vec3 SceneObject::getColor() const
{
	return m_color;
}


bool SceneObject::isEnabled() const
{
	return m_bEnabled;
}


bool SceneObject::isTextureEnabled() const
{
	return m_bEnableTextures;
}
