#pragma once

#include <imgui.h>

#include "effectmanager.hpp"


class Gouraud
	: public Effect
{
public:
	Gouraud();
	std::string getName() const override;

protected:
	void doConfig() override;
	void doRender(const Scene& _scene, const Camera& _camera, const glm::mat4& _perspective) const override;

	float m_ambientStrength = 0.1f;
	float m_diffuseStrength = 0.8f;
	float m_specularStrength = 0.5f;
	int m_specularExponent = 32;

};
