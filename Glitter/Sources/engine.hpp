#pragma once

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>
#include "glitter.hpp"
#include "camera.hpp"
#include "shader.hpp"
#include "mesh.hpp"
#include "scene.hpp"
#include "skybox.hpp"


const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

class Engine
{
	Engine();

public:
	static Engine& instance();
	~Engine();
	int run();

	void onFramebufferSizeCallback(GLFWwindow* window, int width, int height);
	void onMouseCallback(GLFWwindow* window, double xpos, double ypos);
	void onMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	void onScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

protected:
	void recalcPerspective();
	void processInput();

protected:
	GLFWwindow* m_window;
	Camera camera;
	int m_wndWidth;
	int m_wndHeight;
	float lastX = 0;
	float lastY = 0;
	bool firstMouse = true;
	bool dragCamera = false;
	float deltaTime = 0;
	float lastFrame = 0;
	glm::mat4 m_projMatrix;
	int m_projection = 0;
	int m_currentBackground = 0;
};
