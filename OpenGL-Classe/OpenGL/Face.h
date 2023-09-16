#pragma once
#include <vector>

class Face
{
public:
	Face() { NumVerts = 0; };
	~Face() {};

	std::vector<int> verts;
	std::vector<int> norms;
	std::vector<int> colors;
	std::vector<int> texts;
	int NumVerts;
};

