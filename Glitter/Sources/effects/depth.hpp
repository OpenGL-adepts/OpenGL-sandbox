#pragma once
#include "effectmanager.hpp"
#include <imgui.h>


class Depth
	: public Effect
{
public:
	Depth();
	std::string getName() const override;

protected:
	void doConfig() override;
	void doRender(const Scene& _scene, const Camera& _camera, const glm::mat4& _perspective) const override;

	int m_exponent = 4;
};
