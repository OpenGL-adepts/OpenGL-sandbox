#pragma once
#include "effectmanager.hpp"
#include "../skybox.hpp"
#include <memory>


class EnvMap
	: public Effect
{
public:
	explicit EnvMap(const std::shared_ptr<Skybox>& _skybox);
	std::string getName() const override;
	std::string getTutorialPath() const override;

protected:
	void doConfig() override;
	void doRender(const Scene& _scene, const Camera& _camera, const glm::mat4& _perspective) const override;

protected:
	std::shared_ptr<Skybox> m_skybox;
	int m_mode = 0;
	float m_refraction = 1.5f;
};
