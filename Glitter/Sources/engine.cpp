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
	delete cubeMap;
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

	glEnable(GL_DEPTH_TEST);
	Shader shader(PROJECT_SOURCE_DIR "/resources/skyboxes/cubemap/cubemap.vs", PROJECT_SOURCE_DIR "/resources/skyboxes/cubemap/cubemap.fs");
	Shader skyboxShader(PROJECT_SOURCE_DIR "/resources/skyboxes/cubemap/skybox.vs", PROJECT_SOURCE_DIR "/resources/skyboxes/cubemap/skybox.fs");

	cubeMap = new CubeMap();

	// cube VAO
	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	// skybox VAO
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	std::vector<std::string> faces
	{
		PROJECT_SOURCE_DIR "/resources/skyboxes/skybox/right.jpg",
		PROJECT_SOURCE_DIR "/resources/skyboxes/skybox/left.jpg",
		PROJECT_SOURCE_DIR "/resources/skyboxes/skybox/top.jpg",
		PROJECT_SOURCE_DIR "/resources/skyboxes/skybox/bottom.jpg",
		PROJECT_SOURCE_DIR "/resources/skyboxes/skybox/front.jpg",
		PROJECT_SOURCE_DIR "/resources/skyboxes/skybox/back.jpg",
	};

	unsigned int cubemapTexture = cubeMap->loadCubemap(faces);

	shader.use();
	shader.setInt("skybox", 0);

	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

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

		shader.use();
		glm::mat4 model;
		glm::mat4 view = camera.getViewMatrixA();
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		shader.setMat4("model", model);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		shader.setVec3("cameraPos", camera.getPosition());

		glBindVertexArray(cubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.use();
		view = glm::mat4(glm::mat3(camera.getViewMatrixA())); // remove translation from the view matrix
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default


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

				ImGui::Text("Protip: Ctrl+Click controls to manually enter numbers.");
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

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &skyboxVAO);
	
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
