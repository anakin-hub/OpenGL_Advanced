#pragma once
#include "Group.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Mesh
{
public:
	//Groups
	std::vector<Group*> Groups;
	//Vertexs
	std::vector<glm::vec3*> vertices;
	std::vector<glm::vec3*> normals;
	std::vector<glm::vec2*> mappings;

	//what is this?
	glm::vec3 min, max;
	std::string mtllib;//biblioteca do material?
};

