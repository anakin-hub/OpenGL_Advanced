#pragma once
#include<string>
#include<iostream>
#include<fstream>
#include<sstream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	void loadFile(std::string filename);
	void Use();
	GLuint getShader();
	void setBool(const std::string& name, bool value);
	void setInt(const std::string& name, int value);
	void setfloat(const std::string& name, float value);
	void setVec3(const std::string& name, glm::vec3& vector);
	void setVec3(const std::string& name, float x, float y, float z);
	void setMat4(const std::string& name, glm::mat4 transform);

protected:
	const char* vertex_shader;
	const char* fragment_shader;
	GLuint vs, fs, ID;

	void checkCompileErrors(unsigned int shader, std::string type);
};

