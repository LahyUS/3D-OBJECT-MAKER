#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "sources/Shader.h"
#include "sources/Camera.h"
#include "sources/Model.h"
#include "sources/Light.h"
#include "sources/Engine.h"
#include <iostream>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


float deltaTime = 0.0f;
float lastFrame = 0.0f;
float Engine::mouseScaleScroll = 0.0f;
float Engine::change_value = 0.0f;


bool Engine::show_demo_window = true;
bool Engine::show_another_window = false;
ImVec4 Engine::clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


int main()
{
	Engine myEngine("DEMO_3D",
		1920, 1080,
		3, 3,
		false);

	int frames = 0;
	float timer = 0.0f;

	//MAIN LOOP
	while (!myEngine.getWindowShouldClose())
	{
		// per-frame time logic
		// --------------------
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		timer += deltaTime;
		frames++;


		//UPDATE INPUT ---
		myEngine.update();
		myEngine.render();

		if (timer >= 1.0f)
		{
			timer = 0.0f;
			std::cout << "FPS: " << frames << std::endl;
			frames = 0;
		}
	}

	return 0;
}

