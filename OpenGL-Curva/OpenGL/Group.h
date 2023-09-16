#pragma once
#include <string>
#include "Face.h"

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

class Group
{
public:
	Group() { material = "template"; }//pre set material

	std::string name;
	std::string material;
	int materialID;

	std::vector<Face*> faces;
	GLuint VAO;
	GLuint VBOvs, VBOvts, VBOvns, VBOvcs;
	int TotalNumVerts;
};

