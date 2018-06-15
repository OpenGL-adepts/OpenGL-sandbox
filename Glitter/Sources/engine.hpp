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
#include "effects/effectmanager.hpp"
#include "effects/phong.hpp"
#include "effects/gouraud.hpp"
#include "effects/flat.hpp"
#include "effects/depth.hpp"
#include "effects/normal.hpp"


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
	void configPerspecive();
	void processInput();

protected:
	GLFWwindow* m_window;
	Camera camera;
	float lastX = 0;
	float lastY = 0;
	bool firstMouse = true;
	bool dragCamera = false;
	float deltaTime = 0;
	float lastFrame = 0;
	glm::mat4 m_projMatrix;
	int m_projection = 0;
};
