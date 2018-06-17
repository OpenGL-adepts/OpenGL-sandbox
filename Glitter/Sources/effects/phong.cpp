#include "phong.hpp"


Phong::Phong()
{
	loadShader(PROJECT_SOURCE_DIR "/resources/shaders/phong.vert", PROJECT_SOURCE_DIR "/resources/shaders/phong.frag");
}


std::string Phong::getName() const //override
{
	return "Phong";
}


void Phong::doConfig() //override
{
	ImGui::Checkbox("Blinn-Phong", &m_bUseBlinnPhong);
	ImGui::SliderFloat("Ambient", &m_ambientStrength, 0.f, 1.f);
	ImGui::SliderFloat("Diffuse", &m_diffuseStrength, 0.f, 1.f);
	ImGui::SliderFloat("Specular", &m_specularStrength, 0.f, 1.f);
	ImGui::Checkbox("Normal mapping", &m_bEnableNormalMapping);
	mat.config();
}


void Phong::doRender(const Scene& _scene, const Camera& _camera, const glm::mat4& _perspective) const //override
{
	m_shader.activate();
	m_shader.bind("uProjection", _perspective);
	m_shader.bind("uView", _camera.getViewMatrix());
	m_shader.bind("uAmbientStrength", m_ambientStrength);
	m_shader.bind("uDiffuseStrength", m_diffuseStrength);
	m_shader.bind("uSpecularStrength", m_specularStrength);
	m_shader.bind("uEnableNormalMapping", (int)m_bEnableNormalMapping);
	m_shader.bind("uUseBlinnPhong", (int)m_bUseBlinnPhong);
	m_shader.bind("uViewPos", _camera.getPosition());

	// Per light (point)
	m_shader.bind("uLightPos", glm::vec3(5.f, 0, 0));
	m_shader.bind("uLightColor", glm::vec3(1.f));

	// Per object
	mat.bind(m_shader);

	_scene.draw(m_shader);
}
