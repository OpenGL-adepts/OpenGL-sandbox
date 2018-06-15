#include "effectmanager.hpp"
#include "../gui.hpp"
#include <imgui.h>


void Effect::render(const Scene& _scene, const Camera& _camera, const glm::mat4& _perspective) const
{
	if(!m_bInvalid)
		doRender(_scene, _camera, _perspective);
}


bool Effect::isInvalid() const
{
	return m_bInvalid;
}


void Effect::loadShader(const std::string& _vertex, const std::string& _fragment)
{
	m_bInvalid = false;

	try
	{
		m_shader.attach(_vertex);
		m_shader.attach(_fragment);
		m_shader.link();
	}
	catch(...)
	{
		m_bInvalid = true;
	}
}


EffectManager::EffectManager()
	: m_currentEffect(0)
{
}


void EffectManager::registerEffect(const std::shared_ptr<Effect>& _effect)
{
	m_effects.push_back(_effect);
}


void EffectManager::config()
{
	std::vector<std::string> modes;

	for(auto& eff : m_effects)
		modes.push_back(eff->getName());

	Gui::combo("Mode", m_currentEffect, modes);
	m_effects[m_currentEffect]->config();
}


void EffectManager::render(const Scene& _scene, const Camera& _camera, const glm::mat4& _perspective) const
{
	if(m_currentEffect < m_effects.size())
		m_effects[m_currentEffect]->render(_scene, _camera, _perspective);
}
