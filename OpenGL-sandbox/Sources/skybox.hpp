#pragma once
#include "shader.hpp"
#include "cubemap.hpp"
#include "camera.hpp"
#include <glm/glm.hpp>


class Skybox
{
public:
	Skybox();
	~Skybox();
	void draw(const Camera& _camera, const glm::mat4& _projection);
	bool loadSkyboxById(int _id);
	const CubeMap& getCubeMap() const;

protected:
	Shader m_skyboxShader;
	CubeMap m_cubeMap;
	GLuint m_skyboxVAO;
	GLuint m_skyboxVBO;
};
