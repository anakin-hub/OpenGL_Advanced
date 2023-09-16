#pragma once
#include <vector>

class Face
{
public:
	Face() {};
	~Face() {};

	std::vector<int> verts;
	std::vector<int> norms;
	std::vector<int> texts;
};

