#include "engine.hpp"
#include "shader.hpp"
#include "mesh.hpp"
#include "scene.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	Engine::instance().onFramebufferSizeCallback(window, width, height);
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	Engine::instance().onMouseCallback(window, xpos, ypos);
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Engine::instance().onMouseButtonCallback(window, button, action, mods);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Engine::instance().onScrollCallback(window, xoffset, yoffset);
}



Engine::Engine()
	: m_window(nullptr)
	, camera(glm::vec3(0.0f, 0.0f, 3.0f))
{
	glfwInit();
}


//static 
Engine& Engine::instance()
{
	static Engine engine;
	return engine;
}


Engine::~Engine()
{
	glfwTerminate();
}


int Engine::run()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	m_window = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

	// Check for Valid Context
	if (!m_window)
	{
		fprintf(stderr, "Failed to Create OpenGL Context");
		return EXIT_FAILURE;
	}

	// Create Context and Load OpenGL Functions
	glfwMakeContextCurrent(m_window);
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

	gladLoadGL();
	fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

	// Setup ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfwGL3_Init(m_window, true);
	glfwSwapInterval(1);

	glfwSetCursorPosCallback(m_window, mouse_callback);
	glfwSetMouseButtonCallback(m_window, mouse_button_callback);
    glfwSetScrollCallback(m_window, scroll_callback);

	glViewport(0, 0, mWidth, mHeight);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	Shader shader;
	shader.attach(PROJECT_SOURCE_DIR "/resources/shaders/camera.vert");
	shader.attach(PROJECT_SOURCE_DIR "/resources/shaders/camera.frag");
	shader.link();


	Scene scene;
	scene.addObject(PROJECT_SOURCE_DIR "/resources/models/teapot/teapot.obj");
	scene.addObject(PROJECT_SOURCE_DIR "/resources/models/nanosuit/nanosuit.obj")->setPosition(glm::vec3(1.f, 0.f, 0.f));
	scene.addObject(PROJECT_SOURCE_DIR "/resources/models/sheep.obj")->setPosition(glm::vec3(-1.f, 0.f, 0.f));

	//Mesh model(PROJECT_SOURCE_DIR "/resources/models/teapot/teapot.obj");

	lastFrame = glfwGetTime();

	float ambientStrength = 0.1f;
	float diffuseStrength = 0.8f;
	float specularStrength = 0.5f;
	int specularExponent = 32;

	// Rendering Loop
	while (!glfwWindowShouldClose(m_window))
	{
		float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

		processInput();

		// Background Fill Color
		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.activate();
		shader.bind("uProjection", glm::perspective(camera.getFOV(), (float)mWidth / (float)mHeight, 0.1f, 100.0f));
		shader.bind("uView", camera.getViewMatrix());
//		shader.bind("uModel", glm::rotate(model.centerAtAndNormalize(glm::vec3(sin(glfwGetTime()), 0.f, 0.f)), (float)glfwGetTime() * 0.3f, glm::vec3(0, 1, 0)));
		shader.bind("uAmbientStrength", ambientStrength);
		shader.bind("uDiffuseStrength", diffuseStrength);
		shader.bind("uSpecularStrength", specularStrength);
		shader.bind("uSpecularExponent", specularExponent);
		shader.bind("uLightPos", glm::vec3(5.f, 0, 0));
		shader.bind("uViewPos", camera.getPosition());
		scene.draw(shader);
		//model.draw(shader.get());

		ImGui_ImplGlfwGL3_NewFrame();
		ImGui::Text("Phong lighting");
		ImGui::SliderFloat("Ambient", &ambientStrength, 0.f, 1.f);
		ImGui::SliderFloat("Diffuse", &diffuseStrength, 0.f, 1.f);
		ImGui::SliderFloat("Specular", &specularStrength, 0.f, 1.f);
		ImGui::SliderInt("Specular exponent", &specularExponent, 2, 256);
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		// Flip Buffers and Draw
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
	
	ImGui_ImplGlfwGL3_Shutdown();
	return 0;
}


void Engine::onFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


void Engine::onMouseCallback(GLFWwindow* window, double xpos, double ypos)
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


void Engine::onMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if(button == GLFW_MOUSE_BUTTON_RIGHT)
		dragCamera = action == GLFW_PRESS;
}


void Engine::onScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.handleScroll(yoffset);
}


void Engine::processInput()
{
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(m_window, true);

	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
        camera.handleKeyboard(FORWARD, deltaTime);

    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
        camera.handleKeyboard(BACKWARD, deltaTime);

    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
        camera.handleKeyboard(LEFT, deltaTime);

    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
        camera.handleKeyboard(RIGHT, deltaTime);
}
