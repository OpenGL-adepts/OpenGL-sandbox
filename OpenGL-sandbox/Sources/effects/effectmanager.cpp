#include "effectmanager.hpp"


void Effect::config()
{
	if(m_bInvalid)
	{
		ImGui::TextWrapped("Shader failed to compile with following error:\n%s", m_errorString.c_str());

		if(ImGui::Button("Compile"))
		{
			m_shader.clearProgram();
			loadShader(m_shaderVertPath, m_shaderFragPath);
		}
	}
	else
		doConfig();
}


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
	m_shaderVertPath = _vertex;
	m_shaderFragPath = _fragment;

	try
	{
		m_shader.attach(_vertex);
		m_shader.attach(_fragment);
		m_shader.link();
	}
	catch(std::exception& _e)
	{
		m_bInvalid = true;
		m_errorString = _e.what();
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
	{
		auto name = eff->getName();

		if(eff->isInvalid())
			name += " (error)";

		modes.push_back(name);
	}

	Gui::combo("Mode", m_currentEffect, modes);

	auto tutorialPath = m_effects[m_currentEffect]->getTutorialPath();

	if(!tutorialPath.empty())
	if (ImGui::Button("See tutorial"))
		ShellExecute(NULL, "open", tutorialPath.c_str(), NULL, NULL, SW_SHOWNORMAL);

	m_effects[m_currentEffect]->config();
}


void EffectManager::render(const Scene& _scene, const Camera& _camera, const glm::mat4& _perspective) const
{
	if(m_currentEffect < m_effects.size())
		m_effects[m_currentEffect]->render(_scene, _camera, _perspective);
}
