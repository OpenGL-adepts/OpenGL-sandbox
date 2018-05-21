#include "glitter.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>
#include <cstdio>
#include <cstdlib>
#include "shader.hpp"
#include "mesh.hpp"
#include "camera.hpp"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);


Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = 0;
float lastY = 0;
bool firstMouse = true;
bool dragCamera = false;
float deltaTime = 0, lastFrame = 0;

int main()
{
	// Load GLFW and Create a Window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

	// Check for Valid Context
	if (mWindow == nullptr) {
		fprintf(stderr, "Failed to Create OpenGL Context");
		return EXIT_FAILURE;
	}

	// Create Context and Load OpenGL Functions
	glfwMakeContextCurrent(mWindow);
	glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_callback);

	gladLoadGL();
	fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

	// Setup ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfwGL3_Init(mWindow, true);
	glfwSwapInterval(1);

	glfwSetCursorPosCallback(mWindow, mouse_callback);
	glfwSetMouseButtonCallback(mWindow, mouse_button_callback);
    glfwSetScrollCallback(mWindow, scroll_callback);

	glViewport(0, 0, mWidth, mHeight);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	Shader shader;
	shader.attach(PROJECT_SOURCE_DIR "/resources/shaders/camera.vert");
	shader.attach(PROJECT_SOURCE_DIR "/resources/shaders/camera.frag");
	shader.link();

	Mesh model(PROJECT_SOURCE_DIR "/resources/models/teapot/teapot.obj");

	lastFrame = glfwGetTime();

	// Rendering Loop
	while (!glfwWindowShouldClose(mWindow))
	{
		float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

		processInput(mWindow);

		// Background Fill Color
		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.activate();
		shader.bind("projection", glm::perspective(camera.getFOV(), (float)mWidth / (float)mHeight, 0.1f, 100.0f));
		shader.bind("view", camera.getViewMatrix());
		shader.bind("model", glm::rotate(glm::mat4(1.f), (float)glfwGetTime() * 0.3f, glm::vec3(0, 1, 0)));
		model.draw(shader.get());

		ImGui_ImplGlfwGL3_NewFrame();
		ImGui::Text("Hello, word!");
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		// Flip Buffers and Draw
		glfwSwapBuffers(mWindow);
		glfwPollEvents();
	}
	
	ImGui_ImplGlfwGL3_Shutdown();
	glfwTerminate();
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

	if(dragCamera)
		camera.handleMouseMove(xoffset, yoffset);
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if(button == GLFW_MOUSE_BUTTON_RIGHT)
		dragCamera = action == GLFW_PRESS;
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.handleScroll(yoffset);
}


void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.handleKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.handleKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.handleKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.handleKeyboard(RIGHT, deltaTime);
}

