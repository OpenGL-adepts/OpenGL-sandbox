#include "envmap.hpp"
#include "../gui.hpp"
#include "../config.hpp"


EnvMap::EnvMap(const std::shared_ptr<Skybox>& _skybox)
	: m_skybox(_skybox)
{
	loadShader(RELATIVE_PATH_ROOT "/resources/shaders/envmap.vert", RELATIVE_PATH_ROOT "/resources/shaders/envmap.frag");
}


std::string EnvMap::getName() const //override
{
	return "Environment mapping";
}


std::string EnvMap::getTutorialPath() const //override
{
	return "";
}


void EnvMap::doConfig() //override
{
	Gui::combo("Type", m_mode, {"Reflection", "Refraction"});

	if(m_mode == 1)
	{
		ImGui::Text("Examples:\n"
			"Air     - 1.00\n"
			"Water   - 1.33\n"
			"Ice     - 1.309\n"
			"Glass   - 1.52\n"
			"Diamond - 2.42");
		ImGui::SliderFloat("Refractive index", &m_refraction, 1.f, 3.f);
	}
}


void EnvMap::doRender(const Scene& _scene, const Camera& _camera, const glm::mat4& _perspective) const //override
{
	m_shader.activate();
	glActiveTexture(GL_TEXTURE0);
	m_skybox->getCubeMap().bind();
	m_shader.bind("skybox", 0);
	m_shader.bind("uViewPos", _camera.getPosition());
	m_shader.bind("uRefIndex", m_refraction);
	m_shader.bind("uType", m_mode);
	_scene.draw(m_shader, _camera, _perspective, 1);
}
