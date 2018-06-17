#include "scene.hpp"
#include <glm/gtc/matrix_transform.hpp>


template <class SO>
static std::shared_ptr<SO> _configBasicObjects(std::vector<std::shared_ptr<SO>>& _objects, int& _selectedObject, const std::string& _elemType)
{
	std::vector<std::string> options;
	
	for(auto& obj : _objects)
	{
		auto name = obj->getDisplayName();

		if(!obj->isEnabled())
			name += " (disabled)";

		options.push_back(name);
	}

	ImGui::PushID(_elemType.c_str());

	Gui::combo("Selected " + _elemType, _selectedObject, options);

	if(_selectedObject >= 0 && _selectedObject < _objects.size())
	{
		auto obj = _objects[_selectedObject];

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
				_objects.erase(_objects.begin() + _selectedObject);
				
				if(_selectedObject > 0 && _selectedObject == (int)_objects.size())
					--_selectedObject;
			}
			ImGui::SameLine();

			if(ImGui::Button("No", ImVec2(100, 0)))
				ImGui::CloseCurrentPopup();

			ImGui::EndPopup();
		}

		ImGui::PopID();
		return obj;
	}

	ImGui::PopID();
	return nullptr;
}


Scene::Scene(GLFWwindow* _window)
	: m_native(_window)
	, m_lamp(new Mesh(PROJECT_SOURCE_DIR "/resources/models/sheep.obj"))
{
	try
	{
		m_lampShader.attach(PROJECT_SOURCE_DIR "/resources/shaders/lamp.vert");
		m_lampShader.attach(PROJECT_SOURCE_DIR "/resources/shaders/lamp.frag");
		m_lampShader.link();
	}
	catch(...) {}
}


bool Scene::loadFromFile(const std::string& _path)
{
	std::ifstream file(_path);

	if(!file)
		return false;

	m_actors.clear();
	m_lights.clear();
	m_currentActor = 0;
	m_currentLight = 0;

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
				auto tmpObj = std::make_shared<Actor>();
				tmpObj->loadFromFile((root / elem.at("model").get<std::string>()).make_preferred().string());
				tmpObj->fromJSON(elem);
				m_actors.push_back(std::move(tmpObj));
			}
			catch(...)
			{}
		}

		for(auto& elem : json.at("lights"))
		{
			try
			{
				auto tmpObj = std::make_shared<Light>();
				tmpObj->fromJSON(elem);
				m_lights.push_back(std::move(tmpObj));
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

	for(auto& obj : m_actors)
		objArr.push_back(obj->toJSON(_path));

	auto& lgtArr = json["lights"];

	for(auto& lgt : m_lights)
		lgtArr.push_back(lgt->toJSON(_path));

	std::ofstream file(_path);
	file << json;
	return !!file;
}


std::shared_ptr<Actor> Scene::addActor(const std::string& _modelPath)
{
	auto obj = std::make_shared<Actor>();

	if(obj->loadFromFile(_modelPath))
	{
		m_actors.push_back(obj);
		return obj;
	}

	return nullptr;
}


std::shared_ptr<Light> Scene::addLight()
{
	auto obj = std::make_shared<Light>();
	m_lights.push_back(obj);
	return obj;
}


void Scene::draw(const Shader& _shader, const Camera& _camera, const glm::mat4& _perspective) const
{
	_shader.bind("uProjection", _perspective);
	_shader.bind("uView", _camera.getViewMatrix());

	for (auto& obj : m_actors)
	{
		_shader.bind("uModel", obj->getModelMatrix());
		obj->draw(_shader);
	}

	drawLights(_camera, _perspective);
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
	if(addActor(m_native.openModelDialog().string()))
		m_currentActor = m_actors.size() - 1;

	if(auto obj = _configBasicObjects(m_actors, m_currentActor, "object"))
	{
		obj->config();
		
		//TODO: Move inside obj->config() (pass m_native)
		if (ImGui::Button("Load custom texture"))
			obj->setCustomTextureFromFile(m_native.openSceneDialog().string());
	}
}


void Scene::configLights()
{
	if(ImGui::Button("Add light"))
	{
		m_lights.push_back(std::make_shared<Light>());
		m_currentLight = m_lights.size() - 1;
	}

	if(auto light = _configBasicObjects(m_lights, m_currentLight, "light"))
		light->config();
}


void Scene::bindLights(const Shader& _shader) const
{
	char tmp[64];

	for(size_t i = 0; i < m_lights.size() && i < MaxLights; ++i)
	{
		sprintf(tmp, "uLight[%u].active",		i); _shader.bind(tmp, (int)m_lights[i]->isEnabled());
		sprintf(tmp, "uLight[%u].position",		i); _shader.bind(tmp, m_lights[i]->getPosition());
		sprintf(tmp, "uLight[%u].color",		i); _shader.bind(tmp, m_lights[i]->getColor());
		sprintf(tmp, "uLight[%u].attConstant",	i); _shader.bind(tmp, m_lights[i]->getAttenuationConstant());
		sprintf(tmp, "uLight[%u].attLinear",	i); _shader.bind(tmp, m_lights[i]->getAttenuationLinear());
		sprintf(tmp, "uLight[%u].attQuadratic",	i); _shader.bind(tmp, m_lights[i]->getAttenuationQuadratic());
	}

	for(size_t i = m_lights.size(); i < MaxLights; ++i)
	{
		sprintf(tmp, "uLight[%u].active",   i);
		_shader.bind(tmp, 0);
	}
}


size_t Scene::size() const
{
	return m_actors.size();
}


SceneObject& Scene::operator[](size_t _i)
{
	return *m_actors[_i];
}


const SceneObject& Scene::operator[](size_t _i) const
{
	return *m_actors[_i];
}


void Scene::drawLights(const Camera& _camera, const glm::mat4& _perspective) const
{
	m_lampShader.activate();
	m_lampShader.bind("uProjection", _perspective);
	m_lampShader.bind("uView", _camera.getViewMatrix());

	for(auto& light : m_lights)
	if(light->isEnabled())
	{
		m_lampShader.bind("uModel", glm::scale(glm::translate(glm::mat4(1.f), light->getPosition()), glm::vec3(0.25f)));
		m_lampShader.bind("uLightColor", light->getColor());
		m_lamp->draw(m_lampShader);
	}
}
