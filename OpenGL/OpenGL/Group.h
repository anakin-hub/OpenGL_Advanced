#pragma once
#include <string>
#include "Face.h"

class Group
{
public:

	std::string name;
	std::string material;

	std::vector<Face*> faces;
};

