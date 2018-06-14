#include "scene.hpp"
#include <json.hpp>
#include <string>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>


bool Scene::loadFromFile(const std::string& _path)
{
	std::ifstream file(_path);
	m_objects.clear();

	int debugOffset = 0;

	try
	{
		nlohmann::json json;
		file >> json;

		if(!file)
			return false;

		auto root = std::filesystem::path(_path).parent_path();

		for(auto& elem : json.at("objects"))
		{
			auto tmpObj = std::make_shared<SceneObject>();

			//TODO: gdzies to walnac indziej:
			tmpObj->setPosition(glm::vec3(debugOffset++ * 1.f, 0, 0));
			/////

			tmpObj->loadFromFile((root / elem.at("model").get<std::string>()).make_preferred().string());
			m_objects.push_back(std::move(tmpObj));
		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}


bool Scene::saveToFile(const std::string& _path) const
{
	nlohmann::json json;
	auto& objArr = json["objects"];

	for(auto& obj : m_objects)
	{
		nlohmann::json tmp;
		tmp["model"] = std::filesystem::relative(obj->getModelPath(), std::filesystem::path(_path).parent_path()).string();
		objArr.push_back(tmp);
	}

	std::ofstream file(_path);
	file << json;
	return !!file;
}


std::shared_ptr<SceneObject> Scene::addObject(const std::string& _modelPath)
{
	auto obj = std::make_shared<SceneObject>();

	if(obj->loadFromFile(_modelPath))
	{
		m_objects.push_back(obj);
		return obj;
	}

	return nullptr;
}


void Scene::draw(const Shader& _shader) const
{
	for (auto& obj : m_objects)
	{
		_shader.bind("uModel", obj->getModelMatrix());
		obj->draw(_shader.get());
	}
}


void Scene::configObjects()
{
	std::vector<std::string> options;
	
	for(auto& obj : m_objects)
		options.push_back(obj->getDisplayName());

	std::vector<const char*> optionsCstr;
	optionsCstr.resize(options.size());
	std::transform(options.begin(), options.end(), optionsCstr.begin(), [](const std::string& _str) { return _str.c_str(); });
	ImGui::Combo("Selected object", &m_currentObject, optionsCstr.data(), optionsCstr.size());

	if(m_currentObject >= 0 && m_currentObject < m_objects.size())
	{
		auto& obj = m_objects[m_currentObject];

		glm::vec3 objRotation = obj->getRotation();
		float objScale = obj->getScale().x;
		char nameBuf[128] = {};
		memcpy(nameBuf, obj->getDisplayName().c_str(), std::min(obj->getDisplayName().size(), 127u));

		if(ImGui::InputText("Name", nameBuf, sizeof nameBuf))
			obj->setDisplayName(nameBuf);

		ImGui::SliderAngle("Rotation X", &objRotation.x);
		ImGui::SliderAngle("Rotation Y", &objRotation.y);
		ImGui::SliderAngle("Rotation Z", &objRotation.z);
		ImGui::SliderFloat("Scale", &objScale, 0.01f, 20.f);

		obj->setRotation(objRotation);
		obj->setScale(glm::vec3(objScale));
	}
}


size_t Scene::size() const
{
	return m_objects.size();
}


SceneObject& Scene::operator[](size_t _i)
{
	return *m_objects[_i];
}


const SceneObject& Scene::operator[](size_t _i) const
{
	return *m_objects[_i];
}
