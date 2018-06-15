#pragma once
#include "../scene.hpp"
#include "../camera.hpp"
#include "../shader.hpp"
#include <string>
#include <memory>
#include <vector>
#include "../gui.hpp"
#include <imgui.h>


class Effect
{
public:
	virtual ~Effect()
	{}

	// Return displayed name of the effect.
	virtual std::string getName() const = 0;

	// Handle ImGui controls for changing effect configuration
	void config();

	// Handle scene rendering
	void render(const Scene& _scene, const Camera& _camera, const glm::mat4& _perspective) const;

	bool isInvalid() const;

protected:
	virtual void doConfig() {}
	virtual void doRender(const Scene& _scene, const Camera& _camera, const glm::mat4& _perspective) const = 0;
	void loadShader(const std::string& _vertex, const std::string& _fragment);

protected:
	bool m_bInvalid = true;
	std::string m_errorString;
	Shader m_shader;

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
