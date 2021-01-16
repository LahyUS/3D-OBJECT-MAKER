#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#include <iostream>

class Light
{
protected:
	float intensity;
	glm::vec3 color;

public:
	Light(float intensity, glm::vec3 color)
	{
		this->intensity = intensity;
		this->color = color;
	}

	~Light()
	{

	}

	//Functions
	virtual void sendToShader(Shader& program) = 0;
};

class PointLight : public Light
{
protected:
	glm::vec3 position;
	float constant;
	float linear;
	float quadratic;
	glm::vec3 ambient;
	glm::vec3 specular;
	glm::vec3 diffuse;
	glm::vec3 viewPos;

public:
	PointLight(glm::vec3 color, float intensity, glm::vec3 viewpos, glm::vec3 position,
		float constant = 1.f, float linear = 0.045f, float quadratic = 0.0075f)
		: Light(intensity, color)
	{
		this->position = position;
		this->constant = constant;
		this->linear = linear;
		this->quadratic = quadratic;
		this->ambient = glm::vec3(0.2f, 0.2f, 0.2f);
		this->diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
		this->specular = glm::vec3(0.5f, 0.5f, 0.5f);
		this->viewPos = viewpos;
		this->color = color * this->intensity;
	}

	glm::vec3 getColor()
	{
		return this->color;
	}

	~PointLight()
	{

	}

	void setPosition(const glm::vec3 position)
	{
		this->position = position;
	}

	void setViewPos(const glm::vec3 viewpos)
	{
		this->viewPos = viewpos;
	}

	void sendToShader(Shader& program)
	{
		program.setUniformVec3("light.position", this->position);
		program.setUniformVec3("viewPos", this->viewPos);
		program.setUniformVec3("light.color", this->color);
		program.setUniformVec3("light.ambient", this->ambient);
		program.setUniformVec3("light.diffuse", this->diffuse);
		program.setUniformVec3("light.specular", this->specular);
		program.setUniform1f("light.constant", this->constant);
		program.setUniform1f("light.linear", this->linear);
		program.setUniform1f("light.quadratic", this->quadratic);
		program.setUniform1f("material.shininess", 30.4f);
	}
}; 