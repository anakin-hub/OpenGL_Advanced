#pragma once
#include "Obj3D.h"
#include "Pista.h"
#include<cmath>
#include<iostream>
#include<fstream>
#include<sstream>

// stb_image
#include "stb_image.h"

class Manager
{
public:
	std::vector<Obj3D*> objs;
	bool initialize();
	bool initialize(float w, float h);
	void read(std::string filename);
	void readAnim(std::string filename);
	//unsigned int loadTexture(const char* texturePath);
	void configVBO();
	void drawScene(Shader &shader, float time);
	void spawnOBJ(Obj3D* obj, glm::vec3 camPos, glm::vec3 camRot);
	glm::vec3 scaleObj(glm::vec3 min, glm::vec3 max);
	void VerifyColors(Mesh* mesh);


	Pista pista;
	
	GLFWwindow* window;
};

