#include "Shader.h"

void Shader::checkCompileErrors(unsigned int shader, std::string type) 
{
	int success;
	char infoLog[1024];
	if(type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n-------------------------------------------------------" << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if(!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog <<"\n-------------------------------------------------------" << std::endl;
		}
	}
};

Shader::Shader(const char * vertexPath, const char * fragmentPath)
{
	// 1. Retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensures ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::badbit);

	try
	{
		// Open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		std::stringstream vShaderStream, fShaderStream;
		// Read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		// close file handlers
		vShaderFile.close();
		fShaderFile.close();

		// Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	} 
	catch(std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;		
	}

	const char *vShaderCode = vertexCode.c_str();
	const char *fShaderCode = fragmentCode.c_str();
	// 2. Compile shaders
	unsigned int vertex, fragment;

	// Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	// Print compile errors if any
	checkCompileErrors(vertex, "VERTEX");
	
	// Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	// Print compile errors if any
	checkCompileErrors(fragment, "FRAGMENT");
	
	// Shader Program
	this->Program = glCreateProgram();
	glAttachShader(this->Program, vertex);
	glAttachShader(this->Program, fragment);
	glLinkProgram(this->Program);

	// Print linking errors if any
	checkCompileErrors(this->Program, "PROGRAM");

	// Delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use()
{
	glUseProgram(this->Program);
}

void Shader::unUse()
{
	glUseProgram(0);
}

void Shader::setUniform1i(const std::string &nameUniform, int value) const
{
	glUniform1i(glGetUniformLocation(this->Program, nameUniform.c_str()), value);
}

void Shader::setUniform1f(const std::string &nameUniform, float value) const
{
	glUniform1f(glGetUniformLocation(this->Program, nameUniform.c_str()), value);
}

void Shader::setUniformVec2(const std::string &nameUniform, const glm::vec2 &value) const
{
	glUniform2f(glGetUniformLocation(this->Program, nameUniform.c_str()), value.x, value.y);
}

void Shader::setUniformVec3(const std::string &nameUniform, const glm::vec3 &value) const
{
	glUniform3f(glGetUniformLocation(this->Program, nameUniform.c_str()), value.x, value.y, value.z);
}

void Shader::setUniformMat4(const std::string &nameUnifrom, const glm::mat4 &value, bool transpose) const
{
	glUniformMatrix4fv(glGetUniformLocation(this->Program, nameUnifrom.c_str()), 1, transpose, glm::value_ptr(value));
}


