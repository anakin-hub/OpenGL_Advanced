#pragma once
#include <string>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Material
{
public:
	std::string id;
	glm::vec3 ka, kd, ks;
	float Ns;
	std::string map_kd;
	std::string map_ks;
	unsigned int TID_mapKd;
	unsigned int TID_mapKs;
};

