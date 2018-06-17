#include "actor.hpp"
#include <imgui.h>


void Actor::draw(const Shader& _shader, int _reservedTUs) //override
{
	if(m_bEnabled)
	{
		m_material.bind(_shader);
		m_model.draw(_shader, m_bEnableTextures, _reservedTUs);
	}
}


bool Actor::loadFromFile(const std::string& _path)
{
	m_modelPath = _path;

	auto i = m_modelPath.find_last_of("/\\");
	m_displayName = i != std::string::npos ? m_modelPath.substr(i + 1) : m_modelPath;

	return m_model.loadFromFile(m_modelPath);
}


void Actor::fromJSON(const nlohmann::json& _json) //override
{
	SceneObject::fromJSON(_json);

	try { setEnableTextures(_json.at("enableTextures")); } catch (...) {}

	m_material = Material();
	try { m_material.fromJSON(_json.at("material")); } catch(...) {}
}


bool Actor::setCustomTextureFromFile(const std::string& _path)
{
	auto path = std::filesystem::path(_path).make_preferred().string();
	return m_model.setCustomTextureFromFile(path);
}


void Actor::setEnableTextures(bool _bTextures)
{
	m_bEnableTextures = _bTextures;
}


const std::string& Actor::getModelPath() const
{
	return m_modelPath;
}


bool Actor::isTextureEnabled() const
{
	return m_bEnableTextures;
}


int Actor::getTrianglesNumber()
{
	return m_model.getTrianglesNumber(0);
}


void Actor::doConfig() //override
{
	ImGui::Checkbox("Textures", &m_bEnableTextures);
	m_material.config();
}


glm::mat4 Actor::doGetModelMatrix() const //override
{
	return m_model.centerAndNormalize();
}


void Actor::saveToJSON(nlohmann::json& _json, const std::string& _savePath) const //override
{
	_json["model"] = std::filesystem::relative(getModelPath(), std::filesystem::path(_savePath).parent_path()).string();
	_json["enableTextures"] = m_bEnableTextures;
	_json["material"] = m_material.toJSON();
}
