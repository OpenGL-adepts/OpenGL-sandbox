#pragma once
#include <json.hpp>
#include <string>
#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include "shader.hpp"
#include "sceneobject.hpp"
#include "native.hpp"
#include "camera.hpp"
#include "gui.hpp"
#include "actor.hpp"
#include "light.hpp"


class Scene
{
public:
	// Number of light uniforms in shader
	static constexpr int MaxLights = 16;

	Scene(GLFWwindow* _window);
	bool loadFromFile(const std::string& _path);
	bool saveToFile(const std::string& _path) const;

	std::shared_ptr<SceneObject> addObject(const std::string& _modelPath);

	void draw(const Shader& _shader, const Camera& _camera, const glm::mat4& _perspective) const;
	void configObjects();
	void configLights();
	void bindLights(const Shader& _shader) const;

	size_t size() const;
	SceneObject& operator[](size_t _i);
	const SceneObject& operator[](size_t _i) const;

protected:
	void drawLights(const Camera& _camera, const glm::mat4& _perspective) const;

protected:
	Native m_native;
	std::vector<std::shared_ptr<Actor>> m_actors;
	std::vector<std::shared_ptr<Light>> m_lights;
	Shader m_lampShader;
	std::unique_ptr<Mesh> m_lamp;
	int m_currentActor = 0;
	int m_currentLight = 0;

};
