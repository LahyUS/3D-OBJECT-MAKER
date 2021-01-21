#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// INGUI
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
// Component
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

	int window_cursor_state;

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
	static bool show_demo_window;
	static bool show_another_window;
	static ImVec4 clear_color;

	static float mouseScaleScroll;
	static float change_value;

	//Sky-box
	unsigned int cubemapTexture;
	unsigned int skyboxVAO;
	unsigned int skyboxVBO;

	// Ground 
	unsigned int groundVAO, groundVBO, groundTexture;

	//Camera
	Camera camera;

	//Matrices
	glm::mat4 ViewMatrix;

	glm::mat4 ProjectionMatrix;
	float fov;
	float nearPlane;
	float farPlane;

	// Sky box
	// static float skyboxVertices[];

	//Shaders
	std::vector<Shader*> shaders;

	//Models
	std::vector<Model*> models;

	//Lights
	std::vector<PointLight*> Light;

	// Private functions
	void initGLFW();

	void initIMGUI();

	void ImGuiRender();

	// init window
	void initWindow(const char* title, bool resizable);

	// init glew
	int initGLAD();

	// init openGl option
	void initOpenGLOptions();

	// init matrices
	void initMatrices();

	// init shader
	void initShaders();

	// init loaded model
	void initModels();

	// init sky box
	void initSkyBox();

	void initPointLights();

	void initLights();

	void initUniforms();

	void updateUniforms();

	void initGround();


public:
	//Constructors / Destructors
	Engine(
		const char* title,
		const int WINDOW_WIDTH, const int WINDOW_HEIGHT,
		const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR,
		bool resizable
	);

	~Engine();

	//Accessors
	int getWindowShouldClose();

	//Modifiers
	void setWindowShouldClose();

	// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
	void updateKeyboardInput();

	// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
	void updateMouseInput();

	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	void updateInput();

	//Functions
	void updateDt();

	void update();

	void render();

	//Static functions
	static void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH);
};