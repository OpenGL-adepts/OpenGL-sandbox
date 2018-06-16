#pragma once

#include <imgui.h>

#include "effectmanager.hpp"


class Normal
	: public Effect
{
public:
	Normal();
	std::string getName() const override;
	std::string getTutorialPage() const override;

protected:
	void doConfig() override;
	void doRender(const Scene& _scene, const Camera& _camera, const glm::mat4& _perspective) const override;

	bool m_bEnableNormalMapping = false;

};
