#pragma once
#include "sceneobject.hpp"
#include <glm/glm.hpp>


class Light
	: public SceneObject
{
public:
	Light();
	void config() override;
	glm::vec3 getColor() const;

protected:
	glm::vec3 m_color = glm::vec3(1.f);
};

