#include "engine.hpp"


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
	
	Scene scene(m_window);
	scene.addObject(PROJECT_SOURCE_DIR "/resources/models/teapot/teapot.obj")->setPosition(glm::vec3(1.f, 0.f, 0.f));
	scene.addObject(PROJECT_SOURCE_DIR "/resources/models/nanosuit/nanosuit.obj");
	scene.addObject(PROJECT_SOURCE_DIR "/resources/models/cyborg/cyborg.obj")->setPosition(glm::vec3(-1.f, 0.f, 0.f));

	EffectManager effects;
	effects.registerEffect(std::make_shared<Phong>());
	effects.registerEffect(std::make_shared<Gouraud>());
	effects.registerEffect(std::make_shared<Flat>());
	effects.registerEffect(std::make_shared<Normal>());
	effects.registerEffect(std::make_shared<Depth>());

	lastFrame = glfwGetTime();

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

		recalcPerspective();
		effects.render(scene, camera, m_projMatrix);

		ImGui_ImplGlfwGL3_NewFrame();
		//ImGui::ShowDemoWindow();

		ImGui::SetNextWindowPos(ImVec2(0, 0));
		if(ImGui::Begin("OpenGL sandbox"))
		{
			if(ImGui::CollapsingHeader("About"))
			{
				ImGui::Text("OpenGL version: %s", glGetString(GL_VERSION));

				if(ImGui::Button("About this software"))
					ImGui::OpenPopup("About this software");
			}

			if(ImGui::BeginPopupModal("About this software"))
			{
				ImGui::Text("OpenGL sandbox");
				ImGui::Separator();

				ImGui::Text("Distributed under MIT license, available at\nhttps://github.com/OpenGL-adepts/OpenGL-sandbox\n\nAuthors:");
				ImGui::BulletText("Karolina Olszewska");
				ImGui::BulletText("Kasia Kidula");
				ImGui::BulletText("Pawel Koziol");
				ImGui::BulletText("Michal Martyniak");
				ImGui::BulletText("Przemyslaw Roguski");
				ImGui::Text("Developed as academic project in Gdansk University of Technology, 2018");

				if(ImGui::Button("Ok"))
					ImGui::CloseCurrentPopup();

				ImGui::EndPopup();
			}

			if(ImGui::CollapsingHeader("Scene"))
				scene.configObjects();

			if(ImGui::CollapsingHeader("Effects"))
			{
				configPerspective();
				ImGui::Separator();
				effects.config();
			}
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		}
		
		ImGui::End();
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


void Engine::configPerspective()
{
	const char* const tab[] = {"Perspective", "Orthogonal" };
	const char* current = tab[m_projection];

	if(ImGui::BeginCombo("Projection", current))
	{
		for(int i = 0; i < sizeof tab / sizeof *tab; ++i)
		{
			if(ImGui::Selectable(tab[i], current == tab[m_projection]))
				m_projection = i;

			if(current == tab[m_projection])
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}
}


void Engine::recalcPerspective()
{
	switch(m_projection)
	{
	case 0: // Perspective
		m_projMatrix = glm::perspective(camera.getFOV(), (float)mWidth / (float)mHeight, 0.1f, 100.0f);
		break;

	case 1: // Ortho
		m_projMatrix = glm::ortho(0.f, (float)mWidth, 0.f, (float)mHeight, -10000.f, 10000.f);
		m_projMatrix = glm::translate(m_projMatrix, glm::vec3(mWidth / 2.f, mHeight / 2.f, 0.f));
		m_projMatrix = glm::scale(m_projMatrix, glm::vec3(250.f));
		break;
	}
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
