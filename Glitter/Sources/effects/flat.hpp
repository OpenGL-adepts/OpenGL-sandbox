#pragma once
#include "effectmanager.hpp"


class Flat
	: public Effect
{
public:
	Flat();
	const char* getName() const override;
	void config() override;
	void render(const Scene& _scene, const Camera& _camera, const glm::mat4& _perspective) const override;

protected:
	Shader m_shader;
	float m_ambientStrength = 0.1f;
	float m_diffuseStrength = 0.8f;
	float m_specularStrength = 0.5f;
	int m_specularExponent = 32;

};
