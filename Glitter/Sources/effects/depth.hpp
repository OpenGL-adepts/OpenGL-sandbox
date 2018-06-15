#pragma once
#include "effectmanager.hpp"


class Depth
	: public Effect
{
public:
	Depth();
	std::string getName() const override;
	void config() override;

protected:
	void doRender(const Scene& _scene, const Camera& _camera, const glm::mat4& _perspective) const override;

	int m_exponent = 4;
};
