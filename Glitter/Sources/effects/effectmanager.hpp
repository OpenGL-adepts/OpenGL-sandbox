#pragma once
#include "../scene.hpp"
#include "../camera.hpp"
#include "../shader.hpp"
#include <string>
#include <memory>
#include <vector>


class Effect
{
public:
	virtual ~Effect()
	{}

	// Return displayed name of the effect. Must return static address!
	virtual const char* getName() const = 0;

	// Handle ImGui controls for changing effect configuration
	virtual void config() {}

	// Handle scene rendering
	virtual void render(const Scene& _scene, const Camera& _camera, const glm::mat4& _perspective) const = 0;

};


class EffectManager
{
public:
	EffectManager();

	void registerEffect(const std::shared_ptr<Effect>& _effect);
	
	// Handle ImGui controls for changing current effect and its configurations
	void config();

	// Handle scene rendering
	void render(const Scene& _scene, const Camera& _camera, const glm::mat4& _perspective) const;

protected:
	std::vector<std::shared_ptr<Effect>> m_effects;
	int m_currentEffect;
};
