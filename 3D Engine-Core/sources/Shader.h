#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glad/glad.h>

class Shader
{
public:
	unsigned int ID;
	// The program ID
	unsigned int Program;
	// Constructor reads and builds the shader
	Shader(const char* vertexPath, const char* fragmentPath);
	// Use the program
	void use();
	// Un use the program
	void unUse();

	// ------------------------------------------------------------------------
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	
	//Set uniform1i
	void setUniform1i(const std::string &nameUniform, const int value) const;
	//Set uniform1f
	void setUniform1f(const std::string &nameUniform, const float value) const;
	//Set uniform vector 2
	void setUniformVec2(const std::string &nameUniform, const glm::vec2 &value) const;
	//Set uniform vector 3
	void setUniformVec3(const std::string &nameUniform, const glm::vec3 &value) const;
	//Set uniform matrix 4
	void setUniformMat4(const std::string &nameUnifrom, const glm::mat4 &value, bool transpose) const;

private:
	void checkCompileErrors(unsigned int shader, std::string type);
};

