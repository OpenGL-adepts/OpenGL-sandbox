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


void SceneObject::setRotation(float _angle)
{
	m_rotation = _angle;
}


void SceneObject::setPosition(glm::vec3 _pos)
{
	m_position = _pos;
}


const std::string& SceneObject::getModelPath() const
{
	return m_modelPath;
}


glm::mat4 SceneObject::getModelMatrix() const
{
	glm::mat4 model = m_model.centerAtAndNormalize(m_position);
	//model = glm::translate(model, m_position);
	model = glm::rotate(model, m_rotation, m_rotAxis);
	model = glm::scale(model, m_scale);
	return model;
}
