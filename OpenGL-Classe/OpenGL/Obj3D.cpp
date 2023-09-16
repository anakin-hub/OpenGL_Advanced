#include "Obj3D.h"

void Obj3D::MTLsetID()
{
	for (int g = 0; g < mesh->Groups.size(); g++)
	{
		Group* gp = new Group;
		gp = mesh->Groups[g];
		for (int m = 0; m < MTLs.size(); m++)
		{
			//std::cout << gp->material << "\t" << MTLs[m]->id << std::endl;
			if (gp->material == MTLs[m]->id)
			{
				//std::cout << "ID: " << m << std::endl;
				mesh->Groups[g]->materialID = m;
			}
		}
	}
}

void Obj3D::SendMaterial(Shader& shader, int id)
{
	shader.setVec3("ka", MTLs[id]->ka);
	shader.setVec3("kd", MTLs[id]->kd);
	shader.setVec3("ks", MTLs[id]->ks);
	shader.setfloat("ns", MTLs[id]->Ns);
	//std::cout << "sending\n";
}
