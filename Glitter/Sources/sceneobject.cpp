#include "SceneObject.hpp"
#include <glm/gtc/matrix_transform.hpp>


void SceneObject::draw(GLuint _shader)
{
	m_model.draw(_shader);
}


bool SceneObject::loadFromFile(const std::string& _path)
{
	m_modelPath = _path;
	return m_model.loadFromFile(m_modelPath);
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


const std::string& SceneObject::getModelPath() const
{
	return m_modelPath;
}


glm::mat4 SceneObject::getModelMatrix() const
{
	glm::mat4 model = m_model.centerAtAndNormalize(m_position);
	//model = glm::translate(model, m_position);
	model = glm::rotate(model, m_rotation.x, glm::vec3(1.f, 0.f, 0.f));
	model = glm::rotate(model, m_rotation.y, glm::vec3(0.f, 1.f, 0.f));
	model = glm::rotate(model, m_rotation.z, glm::vec3(0.f, 0.f, 1.f));
	model = glm::scale(model, m_scale);
	return model;
}
