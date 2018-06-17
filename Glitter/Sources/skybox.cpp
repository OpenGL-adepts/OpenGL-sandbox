#include "skybox.hpp"


const float SkyboxVertices[] =
{
	// positions
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f,  1.0f
};


Skybox::Skybox()
{
	m_skyboxShader.attach(PROJECT_SOURCE_DIR "/resources/skyboxes/cubemap/skybox.vert");
	m_skyboxShader.attach(PROJECT_SOURCE_DIR "/resources/skyboxes/cubemap/skybox.frag");
	m_skyboxShader.link();
	m_skyboxShader.activate();
	m_skyboxShader.bind("skybox", 0);

	std::vector<std::string> faces
	{
		PROJECT_SOURCE_DIR "/resources/skyboxes/skybox/right.jpg",
		PROJECT_SOURCE_DIR "/resources/skyboxes/skybox/left.jpg",
		PROJECT_SOURCE_DIR "/resources/skyboxes/skybox/top.jpg",
		PROJECT_SOURCE_DIR "/resources/skyboxes/skybox/bottom.jpg",
		PROJECT_SOURCE_DIR "/resources/skyboxes/skybox/front.jpg",
		PROJECT_SOURCE_DIR "/resources/skyboxes/skybox/back.jpg",
	};

	m_cubemapTexture = m_cubeMap.loadCubemap(faces);
	m_cubeMap.textureId = m_cubemapTexture;

	// skybox VAO
	glGenVertexArrays(1, &m_skyboxVAO);
	glGenBuffers(1, &m_skyboxVBO);
	glBindVertexArray(m_skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SkyboxVertices), &SkyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}


Skybox::~Skybox()
{
	glDeleteVertexArrays(1, &m_skyboxVAO);
	glDeleteBuffers(1, &m_skyboxVBO);
}


void Skybox::draw(const Camera& _camera, const glm::mat4& _projection)
{
	// change depth function so depth test passes when values are equal to depth buffer's content
	glDepthFunc(GL_LEQUAL);

	m_skyboxShader.activate();
	m_skyboxShader.bind("view", glm::mat4(glm::mat3(_camera.getViewMatrix()))); // remove translation from the view matrix
	m_skyboxShader.bind("projection", _projection);

	glBindVertexArray(m_skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	// set depth function back to default
	glDepthFunc(GL_LESS);
}


GLuint Skybox::getBackgroundTextureFromChoosen(int _currentBackground)
{
	if (m_cubeMap.currentBackground != _currentBackground)
	{
		glDeleteTextures(1, &m_cubeMap.textureId);
		m_cubeMap.currentBackground = _currentBackground;
		m_cubeMap.textureId = m_cubeMap.getTextureByCurrentBackground();
	}

	return m_cubemapTexture = m_cubeMap.textureId;
}