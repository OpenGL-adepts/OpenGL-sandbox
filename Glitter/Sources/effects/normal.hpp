#pragma once
#include "effectmanager.hpp"


class Normal
	: public Effect
{
public:
	Normal();
	std::string getName() const override;
	void config() override;

protected:
	void doRender(const Scene& _scene, const Camera& _camera, const glm::mat4& _perspective) const override;

	bool m_bEnableNormalMapping;

};
