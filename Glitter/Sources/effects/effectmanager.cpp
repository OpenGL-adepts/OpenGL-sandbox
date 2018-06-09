#include "effectmanager.hpp"
#include <imgui.h>


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
	auto* currentName = m_effects[m_currentEffect]->getName();

	if(ImGui::BeginCombo("Mode", currentName))
	{
		for(int i = 0; i < m_effects.size(); ++i)
		{
			auto* eff = m_effects[i]->getName();
			bool selected = currentName = eff;

			if(ImGui::Selectable(eff, selected))
				m_currentEffect = i;

			if(selected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}

	m_effects[m_currentEffect]->config();
}


void EffectManager::render(const Scene& _scene, const Camera& _camera, const glm::mat4& _perspective) const
{
	if(m_currentEffect < m_effects.size())
		m_effects[m_currentEffect]->render(_scene, _camera, _perspective);
}
