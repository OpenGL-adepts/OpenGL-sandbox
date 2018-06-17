#include "envmap.hpp"


EnvMap::EnvMap(const std::shared_ptr<Skybox>& _skybox)
	: m_skybox(_skybox)
{
	loadShader(PROJECT_SOURCE_DIR "/resources/shaders/envmap.vert", PROJECT_SOURCE_DIR "/resources/shaders/envmap.frag");
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
}


void EnvMap::doRender(const Scene& _scene, const Camera& _camera, const glm::mat4& _perspective) const //override
{
	m_shader.activate();
	glActiveTexture(GL_TEXTURE0);
	m_skybox->getCubeMap().bind();
	m_shader.bind("skybox", 0);
	m_shader.bind("uViewPos", _camera.getPosition());
	_scene.draw(m_shader, _camera, _perspective, 1);
}
