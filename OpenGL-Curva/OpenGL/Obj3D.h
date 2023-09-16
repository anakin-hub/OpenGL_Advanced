#pragma once
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"

class Obj3D
{
public:
	Obj3D() { collider = false; eliminable = false; isMoving = false; animated = false; }
	~Obj3D() {}

	void MTLsetID();
	void SendMaterial(Shader& shader, int id);

	Mesh* mesh;
	glm::mat4 transform;
	int status;
	bool collider, eliminable;
	float angle;
	glm::vec3* position;
	glm::vec3* rotation;
	glm::vec3* direction;
	glm::vec3* scale;

	//MTLs
	std::vector<Material*> MTLs;

	//bullet
	bool animated, isMoving;
	float speed, velocity, timeout, initialTime;
};

