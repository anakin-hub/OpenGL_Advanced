#pragma once
#include "Group.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Mesh
{
public:
	Mesh() { mtllib = "NONE"; }

	//Groups
	std::vector<Group*> Groups;
	//Vertexs
	std::vector<glm::vec3*> vertices;
	std::vector<glm::vec3*> normals;
	std::vector<glm::vec2*> mappings;
	std::vector<glm::vec3*> colors;

	//BoundingBox
	glm::vec3 min, max;//add metodo para verificar se não ultrapassa a bounding box ao adicionar os vertexs

	//biblioteca do material
	std::string mtllib;//salva o nome da biblioteca de material para ser renderizada após a leitura do obj3D
};

