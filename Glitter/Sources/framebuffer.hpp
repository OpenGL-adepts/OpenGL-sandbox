#pragma once
#include <glad/glad.h>
#include <vector>

class Framebuffer {

private:
	GLuint m_FBO;
	GLuint m_colorTexture;
	GLuint m_depthTexture;
	GLsizei m_width;
	GLsizei m_height;
	std::vector<GLenum> m_drawBuffer;

	// Generates an empty color texture with 4 channels (RGBA, 8-bit) using bilinear filtering
	void generateColorTexture() {
		glGenTextures(1, &m_colorTexture);
		glBindTexture(GL_TEXTURE_2D, m_depthTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	}

public:
	Framebuffer(int _width, int _height) {
		m_width = _width;
		m_height = _height;
	}

	~Framebuffer() {
		glDeleteFramebuffers(1, &m_FBO);
		glDeleteTextures(1, &m_colorTexture);
		glDeleteTextures(1, &m_depthTexture);
	}

	// 
	void GenerateFBO() {
		// Generate new buffer
		glGenFramebuffers(1, &m_FBO);

		// Bind it to the pipeline
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

		// Generate empty texture
		generateColorTexture();
		// (optional) m_depthTexture can be generated here

		// Bind texture to the pipeline
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_colorTexture, 0);
		// (optional) m_depthTexture can be bound here

		m_drawBuffer.push_back(GL_COLOR_ATTACHMENT0);
		glDrawBuffers(m_drawBuffer.size(), &m_drawBuffer[0]);

		// FBO completeness check
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			fprintf(stderr, "Error: Framebuffer is not complete!");
		}

		// Unbind FBO
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	}

	void unbind() {
		// Attach to the default/main buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}


};