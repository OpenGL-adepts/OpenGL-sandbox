#pragma once
#include "effectmanager.hpp"


class Depth
	: public Effect
{
public:
	Depth();
	const char* getName() const override;
	void config() override;
	void render(const Scene& _scene, const Camera& _camera, const glm::mat4& _perspective) const override;

protected:
	Shader m_shader;
	int m_exponent = 4;
};
