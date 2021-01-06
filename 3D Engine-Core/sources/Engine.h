#pragma once
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Shader.h"
#include "Model.h"
#include "Light.h"

#include <iostream>

class Engine
{
private:
	//Variables
		//Window
	GLFWwindow* window;
	const int WINDOW_WIDTH;
	const int WINDOW_HEIGHT;
	int framebufferWidth;
	int framebufferHeight;

	//OpenGL Context
	const int GL_VERSION_MAJOR;
	const int GL_VERSION_MINOR;

	//Delta time
	float dt;
	float curTime;
	float lastTime;

	//Mouse Input
	float lastMouseX;
	float lastMouseY;
	float mouseOffsetX;
	float mouseOffsetY;
	bool firstMouse;

public:
	static float mouseScaleScroll;

	//Camera
	Camera camera;

	//Matrices
	glm::mat4 ViewMatrix;
	glm::vec3 camPosition;
	glm::vec3 worldUp;
	glm::vec3 camFront;

	glm::mat4 ProjectionMatrix;
	float fov;
	float nearPlane;
	float farPlane;

	//Shaders
	std::vector<Shader*> shaders;

	//Models
	std::vector<Model*> models;

	//Lights
	std::vector<PointLight*> Light;


	// Private functions
	void initGLFW()
	{
		//INIT GLFW
		if (glfwInit() == GLFW_FALSE)
		{
			std::cout << "ERROR::GLFW_INIT_FAILED" << "\n";
			glfwTerminate();
		}
	}

	// init window
	void initWindow(const char* title, bool resizable)
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

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	}

	// init glew
	int initGLAD()
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
	void initOpenGLOptions()
	{
		// enable z coordiante
		glEnable(GL_DEPTH_TEST);

		// hide things behind
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		//Input
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	// init matrices
	void initMatrices()
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
	void initShaders()
	{
		// build and compile shaders
		// -------------------------
		Shader* ourShader1 = new Shader("resources/shaders/vertLighting.vs.glsl", "resources/shaders/pointLighting.fs.glsl");
		Shader* ourShader2 = new Shader("resources/shaders/lamp.vs.glsl", "resources/shaders/lamp.fs.glsl");
		//Shader lampShader("resources/shaders/lamp.vs.glsl", "resources/shaders/lamp.fs.glsl");

		this->shaders.push_back(ourShader1);
		this->shaders.push_back(ourShader2);
		//this->shaders.push_back(lampShader);
	}

	void initModels()
	{
		// Model ourModel("resources/objects/FarmhouseMaya/farmhouse_obj.obj");
		//Model ourModel("resources/objects/backpack/nanosuit.obj");
		//this->models.push_back(new Model("resources/objects/nanosuit/nanosuit.obj"));
		this->models.push_back(new Model("resources/objects/bed_room/Bedroom 11.obj"));
	}

	void initPointLights()
	{
		this->Light.push_back(new PointLight(glm::vec3(4.0f, 4.0f, 4.0f), glm::vec3(0.0f), glm::vec3(0.0f)));
	}

	void initLights()
	{
		this->initPointLights();
	}

	void initUniforms()
	{
		this->shaders[0]->setUniformVec3("viewPos", camera.Position);
		//INIT UNIFORMS
		this->shaders[0]->setUniformMat4("view", this->ViewMatrix, false);
		this->shaders[0]->setUniformMat4("projection", this->ProjectionMatrix, false);

		for each (PointLight* pl in this->Light)
		{
			pl->sendToShader(*this->shaders[0]);
		}
	}

	void updateUniforms()		
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

		// view/projection transformations
		this->ProjectionMatrix = glm::perspective(glm::radians(camera.Zoom), 
												static_cast<float>(this->framebufferWidth) / this->framebufferHeight,
												this->nearPlane,
												this->farPlane);

		this->shaders[0]->setUniformMat4("projection", this->ProjectionMatrix, false);
		this->shaders[0]->setUniformMat4("view", this->ViewMatrix, false);
		// render the loaded model
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -5.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		this->shaders[0]->setUniformMat4("model", model, false);

		//Update framebuffer size and projection matrix
		glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);
	}

	//Static variables

public:
	//Constructors / Destructors
	Engine(
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
	}

	~Engine()
	{
		glfwDestroyWindow(this->window);
		glfwTerminate();
	
		for (auto*& i : this->models)
			delete i;

		for (size_t i = 0; i < this->Light.size(); i++)
			delete this->Light[i];
	}

	//Accessors
	int getWindowShouldClose()
	{
		return glfwWindowShouldClose(this->window);
	}

	//Modifiers
	void setWindowShouldClose()
	{
		glfwSetWindowShouldClose(this->window, GLFW_TRUE);
	}

	// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
	void updateKeyboardInput()
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
	void updateMouseInput()
	{
		double mouseX, mouseY;
		glfwGetCursorPos(this->window,&mouseX, &mouseY);

		if (this->firstMouse)
		{
			this->lastMouseX = (float)mouseX;
			this->lastMouseY = (float)mouseY;
			this->firstMouse = false;
		}

		// Calc offset
		this->mouseOffsetX = (float)mouseX - this->lastMouseX;
		this->mouseOffsetY = this->lastMouseY - mouseY;

		// Set last X and Y
		this->lastMouseX = (float)mouseX;
		this->lastMouseY = (float)mouseY;

		// Move light
		if (glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
		{
			this->Light[0]->setPosition(this->camera.Position);
		}

		this->camera.ProcessMouseMovement(this->mouseOffsetX, this->mouseOffsetY);
		mouseOffsetX = mouseOffsetY = 0;

		this->camera.ProcessMouseScroll((float)this->mouseScaleScroll);
		mouseScaleScroll = 0;

		if (glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
		{
			this->Light[0]->setPosition(this->camera.Position);
		}
	}

	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		mouseScaleScroll = yoffset;
	}

	void updateInput()
	{
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// ------------------------------------------------------------------------------
		glfwPollEvents();

		this->updateKeyboardInput();
		this->updateMouseInput();
		//this->camera.updateInput(dt, -1, this->mouseOffsetX, this->mouseOffsetY);
	}

	//Functions
	void updateDt()
	{
		this->curTime = static_cast<float>(glfwGetTime());
		this->dt = this->curTime - this->lastTime;
		this->lastTime = this->curTime;
	}

	void update()
	{
		//UPDATE INPUT ---
		this->updateDt();
		this->updateInput();

		//this->models[0]->rotate(glm::vec3(0.f, 1.f, 0.f));
		//this->models[1]->rotate(glm::vec3(0.f, 1.f, 0.f));
		//this->models[2]->rotate(glm::vec3(0.f, 1.f, 0.f));
	}

	void render()
	{
		// UPDATE --- 
		//updateInput(window);

		// DRAW ---
		// Clear
		glClearColor(0.1f, 0.1f, 0.1f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Update the uniforms
		this->updateUniforms();

		// Render models
		for (auto& i : this->models)
			i->Draw(*this->shaders[0]);

		// End Draw
		glfwSwapBuffers(window);
		glFlush();

		glBindVertexArray(0);
		glUseProgram(0);
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//Static functions
	static void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH)
	{
		// make sure the viewport matches the new window dimensions; note that width and 
		// height will be significantly larger than specified on retina displays.
		glViewport(0, 0, fbW, fbH);
	}
};