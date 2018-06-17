#include "scene.hpp"


Scene::Scene(GLFWwindow* _window)
	: m_native(_window)
{
}


bool Scene::loadFromFile(const std::string& _path)
{
	std::ifstream file(_path);

	if(!file)
		return false;

	m_objects.clear();
	m_currentObject = 0;

	try
	{
		nlohmann::json json;
		file >> json;

		if(!file)
			return false;

		auto root = std::filesystem::path(_path).parent_path();

		for(auto& elem : json.at("objects"))
		{
			try
			{
				auto tmpObj = std::make_shared<SceneObject>();

				tmpObj->loadFromFile((root / elem.at("model").get<std::string>()).make_preferred().string());

				try { tmpObj->setDisplayName(elem.at("name").get<std::string>()); } catch(...) {}
				try { tmpObj->setEnabled(elem.at("enabled")); } catch (...) {}
				try { tmpObj->setEnableTextures(elem.at("enableTextures")); } catch (...) {}

				tmpObj->setPosition (loadVector(elem, "position"));
				tmpObj->setRotation (loadVector(elem, "rotation"));
				tmpObj->setScale	(loadVector(elem, "scale", glm::vec3(1.f)));
				tmpObj->setColor	(loadVector(elem, "color", glm::vec3(1.f)));

				m_objects.push_back(std::move(tmpObj));
			}
			catch(...)
			{}
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
		tmp["name"] = obj->getDisplayName();
		tmp["enabled"] = obj->isEnabled();
		tmp["enableTextures"] = obj->isTextureEnabled();

		auto vec = obj->getPosition();
		tmp["position"] = {vec.x, vec.y, vec.z};
		vec = obj->getRotation();
		tmp["rotation"] = {vec.x, vec.y, vec.z};
		vec = obj->getScale();
		tmp["scale"] = {vec.x, vec.y, vec.z};
		vec = obj->getColor();
		tmp["color"] = {vec.x, vec.y, vec.z};

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
		obj->draw(_shader);
	}
}


void Scene::configObjects()
{
	if(ImGui::Button("Save scene"))
		saveToFile(m_native.saveSceneDialog().string());

	ImGui::SameLine();

	if(ImGui::Button("Load scene"))
		loadFromFile(m_native.openSceneDialog().string());

	ImGui::SameLine();

	if(ImGui::Button("Add object"))
		addObject(m_native.openModelDialog().string());

	std::vector<std::string> options;
	
	for(auto& obj : m_objects)
	{
		auto name = obj->getDisplayName();

		if(!obj->isEnabled())
			name += " (disabled)";

		options.push_back(name);
	}

	Gui::combo("Selected object", m_currentObject, options);

	if(m_currentObject >= 0 && m_currentObject < m_objects.size())
	{
		auto obj = m_objects[m_currentObject];

		float objScale = obj->getScale().x;
		char nameBuf[128] = {};
		memcpy(nameBuf, obj->getDisplayName().c_str(), std::min<size_t>(obj->getDisplayName().size(), sizeof nameBuf - 1));

		if(ImGui::InputText("Name", nameBuf, sizeof nameBuf))
			obj->setDisplayName(nameBuf);

		bool enabled = obj->isEnabled();
		ImGui::Checkbox("Enabled", &enabled);
		obj->setEnabled(enabled);

		ImGui::SameLine();

		if(ImGui::Button("Delete object"))
			ImGui::OpenPopup("Confirm deletion");

		if(ImGui::BeginPopupModal("Confirm deletion", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Are you sure to delete object?");

			if(ImGui::Button("Yes"))
			{
				ImGui::CloseCurrentPopup();
				m_objects.erase(m_objects.begin() + m_currentObject);
				
				if(m_currentObject > 0 && m_currentObject == m_objects.size())
					--m_currentObject;
			}
			ImGui::SameLine();

			if(ImGui::Button("No", ImVec2(100, 0)))
				ImGui::CloseCurrentPopup();

			ImGui::EndPopup();
		}
		glm::vec3 vec = obj->getPosition();
		ImGui::SliderFloat("Position X", &vec.x, -20.f, 20.f);
		ImGui::SliderFloat("Position Y", &vec.y, -20.f, 20.f);
		ImGui::SliderFloat("Position Z", &vec.z, -20.f, 20.f);
		obj->setPosition(vec);

		vec = obj->getRotation();
		ImGui::SliderAngle("Rotation X", &vec.x);
		ImGui::SliderAngle("Rotation Y", &vec.y);
		ImGui::SliderAngle("Rotation Z", &vec.z);
		obj->setRotation(vec);

		ImGui::SliderFloat("Scale", &objScale, 0.01f, 20.f);

		bool etextures = obj->isTextureEnabled();
		ImGui::Checkbox("Textures", &etextures);
		obj->setEnableTextures(etextures);

		glm::vec3 color = obj->getColor();
		ImGui::ColorEdit3("Color", (float*)&color);
		obj->setColor(color);

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


//static
glm::vec3 Scene::loadVector(const nlohmann::json& _obj, const std::string& _vectName, const glm::vec3& _default)
{
	auto vect = _obj.find(_vectName);

	if(vect != _obj.end())
	if(vect->is_array())
	{
		glm::vec3 result(0.f);
		auto it = vect->begin();
		result.x = (it++)->get<float>();
		result.y = (it++)->get<float>();
		result.z = (it++)->get<float>();
		return result;
	}

	return _default;
}
