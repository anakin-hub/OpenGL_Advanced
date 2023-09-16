#pragma once
#include "Shader.h"
#include "Obj3D.h"
#include "Material.h"
#include <vector>
#include<cmath>
#include<iostream>
#include<fstream>
#include<sstream>

struct Line {
	GLuint VAO, VBO;
	std::vector<glm::vec3> points;
	glm::vec3 color;
};

class Manager
{
public:
	bool initialize();
	bool initialize(float w, float h);
	void basicVBO();
	void basicDraw(Shader& shader);
	void configVBO();
	void drawScene(Shader &shader);
	void sendVertice(float x, float y);
	void B_spline();
	void Out_Curve();
	void In_Curve();
	void saveCurve();
	void saveB_spline();
	
	GLFWwindow* window;
	GLuint vbo, vao;
	Line lines[4];//CONTROL = 0; B-SPLINE = 1; INTERNAL = 2; EXTERNAL = 3;

	float m;//magnitude (curve's offset to internal and external lines)
	bool curve_saved;
};

