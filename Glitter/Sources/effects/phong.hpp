#pragma once
#include "effectmanager.hpp"


class Phong
	: public Effect
{
public:
	Phong();
	std::string getName() const override;

protected:
	void doConfig() override;
	void doRender(const Scene& _scene, const Camera& _camera, const glm::mat4& _perspective) const override;

	float m_ambientStrength = 0.1f;
	float m_diffuseStrength = 0.8f;
	float m_specularStrength = 0.5f;
	int m_specularExponent = 32;
	bool m_bEnableNormalMapping = false;
	bool m_bUseBlinnPhong = false;
};
