#pragma once
#include "glitter.hpp"
#include "camera.hpp"
#include <GLFW/glfw3.h>


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
};
