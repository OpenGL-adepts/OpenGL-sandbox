#pragma once
#include "sceneobject.hpp"
#include <json.hpp>


class Actor
	: public SceneObject
{
public:
	void draw(const Shader& _shader, int _reservedTUs) override;
	bool loadFromFile(const std::string& _path);
	void fromJSON(const nlohmann::json& _json) override;
	bool setCustomTextureFromFile(const std::string& _path);
	void setEnableTextures(bool _bTextures);
	const std::string& getModelPath() const;
	bool isTextureEnabled() const;

protected:
	void doConfig() override;
	glm::mat4 doGetModelMatrix() const override;
	void saveToJSON(nlohmann::json& _json, const std::string& _savePath) const override;

protected: 
	Mesh m_model;
	Material m_material;
	bool m_bEnableTextures = true;
	std::string m_modelPath;

};
