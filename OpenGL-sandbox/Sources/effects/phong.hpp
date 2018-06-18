#pragma once
#include "effectmanager.hpp"
#include <imgui.h>


class Phong
	: public Effect
{
public:
	Phong();
	std::string getName() const override;
	std::string getTutorialPath() const override;

protected:
	void doConfig() override;
	void doRender(const Scene& _scene, const Camera& _camera, const glm::mat4& _perspective) const override;

	float m_ambientStrength = 0.1f;
	float m_diffuseStrength = 0.8f;
	float m_specularStrength = 0.5f;
	bool m_bEnableNormalMapping = false;
	bool m_bUseBlinnPhong = false;
};
