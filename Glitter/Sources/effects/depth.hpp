#pragma once

#include <imgui.h>

#include "effectmanager.hpp"


class Depth
	: public Effect
{
public:
	Depth();
	std::string getName() const override;
	std::string getTutorialPath() const override;

protected:
	void doConfig() override;
	void doRender(const Scene& _scene, const Camera& _camera, const glm::mat4& _perspective) const override;

	int m_exponent = 4;
};
