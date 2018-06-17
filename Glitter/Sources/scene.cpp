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
				tmpObj->fromJSON(elem);
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
		objArr.push_back(obj->toJSON(_path));

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


void Scene::draw(const Shader& _shader, const Camera& _camera, const glm::mat4& _perspective) const
{
	_shader.bind("uProjection", _perspective);
	_shader.bind("uView", _camera.getViewMatrix());

	for (auto& obj : m_objects)
	{
		_shader.bind("uModel", obj->getModelMatrix());
		obj->draw(_shader);
	}

	m_lights.draw(_camera, _perspective);
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

		obj->config();
	}
}


void Scene::configLights()
{
	m_lights.config();
}


void Scene::bindLights(const Shader& _shader) const
{
	m_lights.bind(_shader);
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
