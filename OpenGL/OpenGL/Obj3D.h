#pragma once
#include "Mesh.h"

class Obj3D
{
	Mesh* mesh;
	glm::mat4 transform;
	int status;
	bool eliminavel;
	glm::vec3 direction;
};

