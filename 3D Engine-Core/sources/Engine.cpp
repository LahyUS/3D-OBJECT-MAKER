#pragma once
#include "Engine.h"
#include "Texture.h"
#include "externals/imgui/imgui_impl_glfw.h"

// Private functions
void Engine::initGLFW()
{
	//INIT GLFW
	if (glfwInit() == GLFW_FALSE)
	{
		std::cout << "ERROR::GLFW_INIT_FAILED" << "\n";
		glfwTerminate();
	}
}

// init window
void Engine::initWindow(const char* title, bool resizable)
{
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->GL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->GL_VERSION_MINOR);
	glfwWindowHint(GLFW_RESIZABLE, resizable);

	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); MAC OS

	this->window = glfwCreateWindow(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, title, NULL, NULL);
	std::cout << "3D Project with OpenGL - Console Debug" << std::endl << std::endl;

	if (this->window == nullptr)
	{
		std::cout << "ERROR::GLFW_WINDOW_INIT_FAILED" << "\n";
		glfwTerminate();
	}

	glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);
	glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);
	//IMPORTANT WHITH PERSPECTIVE MATRIX!!!

	//glViewport(0, 0, framebufferWidth, framebufferHeight);

	glfwMakeContextCurrent(this->window); //IMPORTANT!!

	// tell GLFW to capture our mouse
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

// init glew
int Engine::initGLAD()
{
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
}

// init openGl option
void Engine::initOpenGLOptions()
{
	// enable z coordiante
	glEnable(GL_DEPTH_TEST);

	// hide things behind

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//Input
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

// init matrices
void Engine::initMatrices()
{
	this->ViewMatrix = glm::mat4(1.f);
	this->ViewMatrix = glm::lookAt(this->camPosition, this->camPosition + this->camFront, this->worldUp);

	this->ProjectionMatrix = glm::mat4(1.f);
	this->ProjectionMatrix = glm::perspective(
		glm::radians(this->fov),
		static_cast<float>(this->framebufferWidth) / this->framebufferHeight,
		this->nearPlane,
		this->farPlane
	);
}

// init shader
void Engine::initShaders()
{
	// build and compile shaders
	// -------------------------
	Shader* ourShader0 = new Shader("resources/shaders/vertLighting.vs.glsl", "resources/shaders/pointLighting.fs.glsl");
	this->shaders.push_back(ourShader0);

	Shader* ourShader1 = new Shader("resources/shaders/skybox_vs.glsl", "resources/shaders/skybox_fs.glsl");
	this->shaders.push_back(ourShader1);
	//this->shaders.push_back(lampShader);
}

void Engine::initModels()
{
	// Model ourModel("resources/objects/FarmhouseMaya/farmhouse_obj.obj");
	//Model ourModel("resources/objects/backpack/nanosuit.obj");
	//this->models.push_back(new Model("resources/objects/nanosuit/nanosuit.obj"));
	this->models.push_back(new Model("resources/objects/bed_room/Bedroom 11.obj"));
}

void Engine::initPointLights()
{
	this->Light.push_back(new PointLight(glm::vec3(1.0f, 1.0f, 1.0f), 4.0f, glm::vec3(0.0f), glm::vec3(0.0f)));
}

void Engine::initLights()
{
	this->initPointLights();
}

void Engine::initUniforms()
{
	this->shaders[0]->setUniformVec3("viewPos", camera.Position);
	//INIT UNIFORMS
	this->shaders[0]->setUniformMat4("view", this->ViewMatrix, false);
	this->shaders[0]->setUniformMat4("projection", this->ProjectionMatrix, false);

	for each (PointLight * pl in this->Light)
	{
		pl->sendToShader(*this->shaders[0]);
	}
}

//Static variables

Engine::~Engine()
{
	glfwDestroyWindow(this->window);
	glfwTerminate();

	for (auto*& i : this->models)
		delete i;

	for (size_t i = 0; i < this->Light.size(); i++)
		delete this->Light[i];

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

//Accessors
int Engine::getWindowShouldClose()
{
	return glfwWindowShouldClose(this->window);
}

//Modifiers
void Engine::setWindowShouldClose()
{
	glfwSetWindowShouldClose(this->window, GLFW_TRUE);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Engine::updateKeyboardInput()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
		this->dt *= 3.0f;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		this->camera.ProcessKeyboard(FORWARD, this->dt);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		this->camera.ProcessKeyboard(BACKWARD, this->dt);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		this->camera.ProcessKeyboard(LEFT, this->dt);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		this->camera.ProcessKeyboard(RIGHT, this->dt);

	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		this->camPosition.y -= 0.05f;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		this->camPosition.y += 0.05f;
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void Engine::updateMouseInput()
{
	//switch (this->window_cursor_state)
	//{
	//case 0:	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//case 1:	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	//}


	double mouseX, mouseY;
	glfwGetCursorPos(this->window, &mouseX, &mouseY);

	if (this->firstMouse)
	{
		this->lastMouseX = (float)mouseX;
		this->lastMouseY = (float)mouseY;
		this->firstMouse = false;
	}

	// Calc offset
	this->mouseOffsetX = -(float)mouseX + this->lastMouseX;
	this->mouseOffsetY = -this->lastMouseY + mouseY;

	// Set last X and Y
	this->lastMouseX = (float)mouseX;
	this->lastMouseY = (float)mouseY;

	// Move light
	if (glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
	{
		this->window_cursor_state = 0;
		this->Light[0]->setPosition(this->camera.Position);
		this->camera.ProcessMouseMovement(this->mouseOffsetX, this->mouseOffsetY);
		mouseOffsetX = mouseOffsetY = 0;
	}

	// Move light
	//else if (glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	//{
	//	this->window_cursor_state = 1;
	//}

	//else if(glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	//{
	//	this->Light[0]->setPosition(this->camera.Position);
	//}

	this->camera.ProcessMouseScroll((float)this->mouseScaleScroll);
	mouseScaleScroll = 0;
}

// Mouse scroll call back function
void Engine::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Engine::mouseScaleScroll = yoffset;
}

// Up date user's input
void Engine::updateInput()
{
	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	// ------------------------------------------------------------------------------
	glfwPollEvents();

	this->updateKeyboardInput();
	this->updateMouseInput();
	//this->camera.updateInput(dt, -1, this->mouseOffsetX, this->mouseOffsetY);
}

// Functions
// Update variable value
void Engine::updateDt()
{
	this->curTime = static_cast<float>(glfwGetTime());
	this->dt = this->curTime - this->lastTime;
	this->lastTime = this->curTime;
}

// Update user's interation on frame
void Engine::update()
{
	//UPDATE INPUT ---
	this->updateDt();
	this->updateInput();

	//this->models[0]->rotate(glm::vec3(0.f, 1.f, 0.f));
	//this->models[1]->rotate(glm::vec3(0.f, 1.f, 0.f));
	//this->models[2]->rotate(glm::vec3(0.f, 1.f, 0.f));
}

void Engine::initIMGUI()
{
	// Setup Dear ImGui context
	//IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	//ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	//ImGui_ImplOpenGL3_Init(glsl_version);
}

void Engine::ImGuiRender()
{
	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		//static float f = 0.0f;
		//static int counter = 0;
		//ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
		//ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		//ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		//ImGui::Checkbox("Another Window", &show_another_window);

		float trans_x = this->models[0]->position.x;
		ImGui::SliderFloat("float", &trans_x, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		this->Engine::change_value = trans_x;
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		float rotation_val = this->models[0]->position.x;
		ImGui::SliderFloat("Rotation", &rotation_val, 0.0f, 0.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		this->Engine::change_value = rotation_val;
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		//if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		//	counter++;
		//ImGui::SameLine();
		//ImGui::Text("counter = %d", counter);
		//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		//ImGui::End();
	}

	// Rendering ImGui
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Engine::updateUniforms()
{
	// Enable shader
	this->shaders[0]->use();
	//this->Light[0]->setPosition(camera.Position); // Uncomment to move the light along with camera
	//this->Light[0]->setViewPos(camera.Position);

	for each (PointLight * pl in this->Light)
	{
		pl->sendToShader(*this->shaders[0]);
	}

	// Update view matrix (camera)
	this->ViewMatrix = this->camera.GetViewMatrix();

	// View/Projection transformations
	this->ProjectionMatrix = glm::perspective(glm::radians(camera.Zoom),
		static_cast<float>(this->framebufferWidth) / this->framebufferHeight,
		this->nearPlane,
		this->farPlane);

	this->shaders[0]->setUniformMat4("projection", this->ProjectionMatrix, false);
	this->shaders[0]->setUniformMat4("view", this->ViewMatrix, false);

	// Transform the loaded model
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f)); // translate it down so it's at the center of the scene
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down
	// model = glm::rotate(model, glm::radians(Engine::change_value), glm::vec3(0.0f, 1.0f, 0.0f));

	// Update change_value for rotation
	Engine::change_value += 0.05f;

	// Send updated uniform to shader program
	this->shaders[0]->setUniformMat4("model", model, false);

	//Update framebuffer size and projection matrix
	glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);
}
// Skybox

void Engine::initSkyBox()
{
	// skybox VAO
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, 432, &Engine::skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// load textures
// -------------
	vector<std::string> faces;
	faces.push_back("resources/skybox/right.jpg");
	faces.push_back("resources/skybox/left.jpg");
	faces.push_back("resources/skybox/top.jpg");
	faces.push_back("resources/skybox/bottom.jpg");
	faces.push_back("resources/skybox/back.jpg");
	faces.push_back("resources/skybox/front.jpg");

	cubemapTexture = Model::LoadCubemap(faces);

	this->shaders[1]->use();
	this->shaders[1]->setInt("skybox", 0);
}

// Render function
void Engine::render()
{
	// UPDATE --- 
	//updateInput(window);

	// DRAW ---
	// Clear
	glClearColor(0.1f, 0.1f, 0.1f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//Update the uniforms
	this->updateUniforms();

	this->shaders[0]->use();
	// Render models
	for (auto& i : this->models)
		i->Draw(*this->shaders[0]);



	this->shaders[1]->use();

	// Render Sky box
	// Draw skybox as last
	glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
	this->shaders[1]->use();
	glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
	// this->shaders[1]->setUniformMat4("model", view, false);
	this->shaders[1]->setUniformMat4("view", view, false);
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
	this->shaders[1]->setUniformMat4("projection", projection, false);

	// skybox cube
	glBindVertexArray(this->skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default

	ImGuiRender();

// End Draw
	glfwSwapBuffers(window);
	//glFlush();
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glfwPollEvents();
}

// Static functions
void Engine::framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, fbW, fbH);
}

//Constructors / Destructors
Engine::Engine(
	const char* title,
	const int WINDOW_WIDTH, const int WINDOW_HEIGHT,
	const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR,
	bool resizable
)
	:
	WINDOW_WIDTH(WINDOW_WIDTH),
	WINDOW_HEIGHT(WINDOW_HEIGHT),
	GL_VERSION_MAJOR(GL_VERSION_MAJOR),
	GL_VERSION_MINOR(GL_VERSION_MINOR),
	camera(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f))
{
	// Init variables
	this->window = nullptr;
	this->framebufferWidth = this->WINDOW_WIDTH;
	this->framebufferHeight = this->WINDOW_HEIGHT;

	// Set up camera parameter
	this->camPosition = glm::vec3(0.f, 0.f, 1.f);
	this->worldUp = glm::vec3(0.f, 1.f, 0.f);
	this->camFront = glm::vec3(0.f, 0.f, -1.f);

	// Set up view-port
	this->fov = 90.f;
	this->nearPlane = 0.1f;
	this->farPlane = 1000.f;

	// Initialize moving specifications
	this->dt = 0.f;
	this->curTime = 0.f;
	this->lastTime = 0.f;

	// Initialize mouse
	this->lastMouseX = 0.0;
	this->lastMouseY = 0.0;
	this->mouseOffsetX = 0.0;
	this->mouseOffsetY = 0.0;
	this->firstMouse = true;

	// Initilaize our engine system
	this->initGLFW();
	this->initWindow(title, resizable);
	this->initGLAD();
	this->initOpenGLOptions();

	// Initialize components of my engine
	this->initMatrices();
	this->initShaders();
	this->initModels();
	this->initLights();
	this->initUniforms();
	this->initSkyBox();
	this->initIMGUI();
}