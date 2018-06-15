#pragma once
#include "effectmanager.hpp"
#include <imgui.h>

class Normal
	: public Effect
{
public:
	Normal();
	std::string getName() const override;

protected:
	void doConfig() override;
	void doRender(const Scene& _scene, const Camera& _camera, const glm::mat4& _perspective) const override;

	bool m_bEnableNormalMapping = false;

};
