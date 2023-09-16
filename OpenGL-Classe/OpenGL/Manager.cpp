#include "Manager.h"

//Initialize the GLFW and GLEW
bool Manager::initialize()
{
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return false;
	}
	/* Caso necessário, definições específicas para SOs, p. e. Apple OSX *
	/* Definir como 3.2 para Apple OS X */
	/*glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/
	window = glfwCreateWindow(800, 600, "Obj Reader", NULL, NULL);

	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	// inicia manipulador da extensão GLEW
	glewExperimental = GL_TRUE;
	glewInit();
	return true;
}

bool Manager::initialize(float w, float h)
{
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return false;
	}
	/* Caso necessário, definições específicas para SOs, p. e. Apple OSX *
	/* Definir como 3.2 para Apple OS X */
	/*glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/
	window = glfwCreateWindow(w, h, "Obj Reader", NULL, NULL);

	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	// inicia manipulador da extensão GLEW
	glewExperimental = GL_TRUE;
	glewInit();
	return true;
}

void Manager::read(std::string filename)
{
	Mesh* mesh = new Mesh;
	Group* g = new Group;
	Face* f = new Face;

	float minX, minY, minZ, maxX, maxY, maxZ;

	int face = 0;

	bool firstGroup = false, firstVerify = false;

	std::string folder = "../../Objs/";
	std::string path = folder + filename;

	std::ifstream arq(path);

	std::string line;

	while (getline(arq, line))
	{
		std::stringstream sline;
		
		sline << line;
		
		std::string temp;
		
		sline >> temp;

		//std::cout << temp << std::endl;
		if (temp == "v")
		{
			float x, y, z;
			sline >> x >> y >> z;
			
			//std::cout << x << "\t";

			glm::vec3 *v = new glm::vec3(x, y, z);

			mesh->vertices.push_back(v);

			if (!firstVerify)
			{
				minX = maxX = x;
				minY = maxY = y;
				minZ = maxZ = z;
				firstVerify = true;
			}
			else
			{
				if (x < minX)
					minX = x;
				if (x > maxX)
					maxX = x;
				if (y < minY)
					minY = y;
				if (y > maxY)
					maxY = y;
				if (z < minZ)
					minZ = z;
				if (z > maxZ)
					maxZ = z;
			}

			//std::cout << "VERTICE X: " << x << " Y: " << y << " Z: " << z << std::endl;
		}
		else if (temp == "vt")
		{
			float x, y; // U,V
			sline >> x >> y;
			glm::vec2 *vt = new glm::vec2(x, y);

			mesh->mappings.push_back(vt);

			//std::cout << "TEXTURE U: " << x << " V: " << y << std::endl;
		}
		else if (temp == "vn")
		{
			float x, y, z;
			sline >> x >> y >> z;
			glm::vec3 *vn = new glm::vec3(x, y, z);

			mesh->normals.push_back(vn);

			//std::cout << "VERTICE X: " << x << " Y: " << y << " Z: " << z << std::endl;
		}
		else if (temp == "vc")
		{
			float x, y, z;
			sline >> x >> y >> z;
			glm::vec3* vc = new glm::vec3(x, y, z);

			mesh->colors.push_back(vc);

			//std::cout << "COLORS X: " << x << " Y: " << y << " Z: " << z << std::endl;
		}
		else if (temp == "g")
		{
			std::string name;
			sline >> name;

			if (!firstGroup)
			{
				g->name = name;
				firstGroup = true;
				//std::cout << "TRUE" << std::endl;
			}
			else
			{
				mesh->Groups.push_back(g);
				g = new Group;
				g->name = name;
				//std::cout << "FALSE" << std::endl;
			}
			firstVerify = false;
		}
		else if (temp == "mtllib")
		{
			std::string mtl;
			//std::cout << "MTL: " << mtl << std::endl;
			sline >> mtl;
			mesh->mtllib = mtl;
		}
		else if (temp == "usemtl")
		{
			std::string idmtl;
			sline >> idmtl;
			g->material = idmtl;
		}
		else if (temp == "f")
		{
			while (sline)
			{
				std::string token;
				sline >> token;
				std::stringstream stoken;
				stoken << token;
				std::string aux;
				int n = 0;		
				while (std::getline(stoken, aux, '/'))
				{
					int v;
					std::stringstream saux;
					saux << aux;
					saux >> v;

					if (n == 0 && v != 0)
					{
						//std::cout << "v" << v;
						f->verts.push_back(v - 1);
						f->NumVerts++;
					}
					else if (n == 1 && v != 0)
					{
						//std::cout << "vt" << v;
						f->texts.push_back(v - 1);
					}
					else if (n == 2 && v != 0)
					{
						//std::cout << "vn" << v;
						f->norms.push_back(v - 1);
					}
					else if (n == 3 && v != 0)
					{
						//std::cout << "vc" << v;
						f->colors.push_back(v - 1);
					}

					
					n++;
					v = 0;
				}
				//std::cout << "\t";
			}
			//std::cout << std::endl;
			g->faces.push_back(f);
			f = new Face;
		}
	}
	arq.close();
	mesh->Groups.push_back(g);

	Obj3D* o = new Obj3D;
	Material* mtl = new Material;
	bool first = true;
	
	//PODEMOS COLOCAR TEMPLATE.MTL NO INICIALIZADOR DA MESH E USEMTL "template"...

	std::cout << mesh->mtllib << std::endl;

	if (mesh->mtllib != "NONE")
		path = folder + mesh->mtllib;
	else
		path = folder + "template.mtl";

	std::ifstream mtlArq(path);

	while (getline(mtlArq, line)) 
	{
		std::stringstream sline;

		sline << line;

		std::string temp;

		sline >> temp;

		if (temp == "newmtl")
		{
			std::string id;
			sline >> id;
			if (first)
			{
				first = false;
			}
			else
			{
				o->MTLs.push_back(mtl);
				mtl = new Material;
			}
			mtl->id = id;
		}
		else if (temp == "Ka")//ambient
		{
			float r, g, b;
			sline >> r >> g >> b;
			glm::vec3 k(r,g,b);
			mtl->ka = k;
		}
		else if (temp == "Kd")//difuse
		{
			float r, g, b;
			sline >> r >> g >> b;
			glm::vec3 k(r, g, b);
			mtl->kd = k;
		}
		else if (temp == "Ks")//specular
		{
			float r, g, b;
			sline >> r >> g >> b;
			glm::vec3 k(r, g, b);
			mtl->ks = k;
		}
		else if (temp == "Ns")
		{
			float Ns;
			sline >> Ns;
			mtl->Ns = Ns;
		}
		else if (temp == "map_Kd")
		{
			std::string map;
			sline >> map;
			mtl->map_kd = map;
			//std::string file = folder + map;
			//mtl->TID_mapKd = loadTexture(file.c_str());
		}
		else if (temp == "map_Ks")
		{
			std::string map;
			sline >> map;
			mtl->map_ks = map;
			//std::string file = folder + map;
			//mtl->TID_mapKs = loadTexture(file.c_str());
		}
	}
	mtlArq.close();
	o->MTLs.push_back(mtl);

	mesh->min = glm::vec3(minX, minY, minZ);
	mesh->max = glm::vec3(maxX, maxY, maxZ);

	VerifyColors(mesh);
	o->mesh = mesh;

	o->MTLsetID();
	//std::cout << "MTL!\n";

	objs.push_back(o);
	//std::cout << "Leitura OBJ OK!" << std::endl;
}

void Manager::readAnim(std::string filename)
{
	std::string folder = "../../Objs/";
	std::string path = folder + filename;

	std::ifstream arq(path);

	std::string line;

	while (getline(arq, line))
	{
		std::stringstream sline;

		sline << line;

		std::string temp;

		sline >> temp;

		if (temp == "v")
		{
			float x, y, z;
			sline >> x >> y >> z;

			glm::vec3* v = new glm::vec3(x, y, z);
			pista.position.push_back(*v);
		}
	}
	arq.close();
}

//unsigned int Manager::loadTexture(const char* texturePath)
//{
//	unsigned int textureID;
//	glGenTextures(1, &textureID);
//	glBindTexture(GL_TEXTURE_2D, textureID);
//
//	// Set texture wrapping and filtering options
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//	int width, height, numChannels;
//	unsigned char* imageData = stbi_load(texturePath, &width, &height, &numChannels, 0);
//	if (imageData)
//	{
//		GLenum format;
//		if (numChannels == 1)
//			format = GL_RED;
//		else if (numChannels == 3)
//			format = GL_RGB;
//		else if (numChannels == 4)
//			format = GL_RGBA;
//
//		// Reverse the image data vertically for BMP format
//		stbi_set_flip_vertically_on_load(true);
//
//		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
//		glGenerateMipmap(GL_TEXTURE_2D);
//	}
//	else
//	{
//		// Error handling if the texture failed to load
//		std::cout << "Failed to load texture: " << texturePath << std::endl;
//	}
//
//	// Free the image data memory
//	stbi_image_free(imageData);
//
//	return textureID;
//}

void Manager::configVBO()//colocar no objeto direto?
{
	for (int o = 0; o < objs.size(); o++)
	{
		Mesh* mesh = new Mesh;
		mesh = objs[o]->mesh;
		glm::vec3 v, vc, vn;
		glm::vec2 vt;

		for (int i = 0; i < mesh->Groups.size(); i++)
		{
			Group g = *mesh->Groups[i];
			std::vector<float> vs;
			std::vector<float> vts;
			std::vector<float> vns;
			std::vector<float> vcs;
			for (int l = 0; l < g.faces.size(); l++)
			{
				Face f = *g.faces[l];

				//std::cout << "FACE" << l << "\t";
				//std::cout << "VERTICES: " << f.NumVerts << "\n";
				for (int n = 0; n < f.NumVerts; n++)
				{
					if (f.verts.size() > n)
					{
						v = *mesh->vertices[f.verts[n]];
						vs.push_back(v.x);
						vs.push_back(v.y);
						vs.push_back(v.z);
						//std::cout << "VERTICE " << f.verts[n] << "\t";
					}

					if (f.colors.size() > n)
					{
						//std::cout << "mesh colors size: " << mesh->colors.size() << std::endl;
						//std::cout << "face colors size: " << f.colors.size() << std::endl;
						vc = *mesh->colors[f.colors[n]];
						vcs.push_back(vc.x);
						vcs.push_back(vc.y);
						vcs.push_back(vc.z);
						//std::cout << "COLOR " << f.colors[n] << "\n";
					}

					if (f.norms.size() > n)
					{
						vn = *mesh->normals[f.norms[n]];
						vns.push_back(vn.x);
						vns.push_back(vn.y);
						vns.push_back(vn.z);
						//std::cout << "NORMAL " << f.norms[n] << "\n";
					}

					if (f.texts.size() > n)
					{
						vt = *mesh->mappings[f.texts[n]];
						vts.push_back(vt.x);
						vts.push_back(vt.y);
						//std::cout << "TEXT " << f.texts[n] << "\n";
					}

					g.TotalNumVerts++;
				}
			}
			GLuint VAO, VBOv, VBOc, VBOn, VBOt;

			//VBO - vertices
			glGenBuffers(1, &VBOv);
			glBindBuffer(GL_ARRAY_BUFFER, VBOv);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vs.size(), vs.data(), GL_STATIC_DRAW);

			//VBO - colors
			glGenBuffers(1, &VBOc);
			glBindBuffer(GL_ARRAY_BUFFER, VBOc);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vcs.size(), vcs.data(), GL_STATIC_DRAW);

			//VBO - normals
			glGenBuffers(1, &VBOn);
			glBindBuffer(GL_ARRAY_BUFFER, VBOn);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vns.size(), vns.data(), GL_STATIC_DRAW);

			//VBO - textures
			glGenBuffers(1, &VBOt);
			glBindBuffer(GL_ARRAY_BUFFER, VBOt);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vts.size(), vts.data(), GL_STATIC_DRAW);

			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);

			//VERTICES
			glBindBuffer(GL_ARRAY_BUFFER, VBOv);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

			//COLORS
			glBindBuffer(GL_ARRAY_BUFFER, VBOc);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

			//NORMALS
			glBindBuffer(GL_ARRAY_BUFFER, VBOn);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

			//TEXTURES
			glBindBuffer(GL_ARRAY_BUFFER, VBOt);
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, NULL);

			g.VAO = VAO;
			g.VBOvs = VBOv;
			g.VBOvcs = VBOc;
			g.VBOvns = VBOn;
			g.VBOvts = VBOt;

			*mesh->Groups[i] = g;
			objs[o]->mesh = mesh;
		}

		glm::vec3 Tmin(-1.0f), Tmax(1.0f);

		//std::cout << "vec tmin tmax ok\n";

		if(mesh->max.x > Tmax.x || mesh->max.y > Tmax.y || mesh->max.z > Tmax.z || mesh->min.x < Tmin.x || mesh->min.y < Tmin.y || mesh->min.z < Tmin.z)
		{
			//std::cout << "scale start\n";

			objs[o]->scale = new glm::vec3(scaleObj(objs[o]->mesh->min, objs[o]->mesh->max));
			
			//std::cout << objs[o]->scale->x << objs[o]->scale->y << objs[o]->scale->z << std::endl;
			//std::cout << "scale end\n";
		}
	}
	//std::cout << "VBO!\n";
}

void Manager::drawScene(Shader &shader, float time)
{	
	//std::cout << "START LOOP!\n";
		for (int o = 0; o < objs.size(); o++)
		{
			if (objs[o]->car)
			{
				if (!objs[o]->xlr8)
				{
					objs[o]->offset = *objs[o]->position;
					objs[o]->position = new glm::vec3(pista.position[0] + objs[o]->offset);
					objs[o]->pistaPos = 0;
					objs[o]->xlr8 = true;
				}
				else
				{
					objs[o]->pistaPos++;
					if (objs[o]->pistaPos >= pista.position.size())
					{
						objs[o]->pistaPos = 0;
					}
					objs[o]->position = new glm::vec3(pista.position[objs[o]->pistaPos] + objs[o]->offset);
				}
			}

			//std::cout << o << "\t";
			//std::cout << objs[o]->mesh->Groups[0]->name << "\t";
			//std::cout << objs[o]->scale->x << "\t";
			 
			Mesh m = *objs[o]->mesh;

			objs[o]->transform = glm::mat4(1.0f);

			objs[o]->transform = glm::translate(objs[o]->transform, *objs[o]->position);
			//std::cout << "TRANSLATE!\t";
			objs[o]->transform = glm::rotate(objs[o]->transform, glm::radians(objs[o]->angle), *objs[o]->rotation);
			//std::cout << "ROTATE!\t";
			objs[o]->transform = glm::scale(objs[o]->transform, *objs[o]->scale);						
			//std::cout << "SCALE!\t";
			shader.setMat4("matrix", objs[o]->transform);
			//GLint matrixLoc = glGetUniformLocation(shader_programme, "matrix");
			//glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, glm::value_ptr(objs[o]->transform));
			//std::cout << "MATRIX!\t";
			for (int i = 0; i < m.Groups.size(); i++)
			{
				Group g = *m.Groups[i];
				//MATERIAL
				objs[o]->SendMaterial(shader, g.materialID);

				glBindVertexArray(g.VAO);
				//glDrawArrays(GL_TRIANGLES, 0, g.TotalNumVerts);

				
				int f = 0;
				for (int l = 0; l < g.faces.size(); l++)
				{
					//DRAW
					glDrawArrays(GL_TRIANGLE_FAN, f, g.faces[l]->NumVerts);

					f += g.faces[l]->NumVerts;
				}
			}
			//std::cout << "DRAWED!\t";
			if (objs[o]->isMoving)
			{
				objs[o]->velocity = objs[o]->speed * time;
				*objs[o]->position += objs[o]->velocity * *objs[o]->direction;
				if (objs[o]->timeout < glfwGetTime())
				{
					objs.erase(objs.begin() + o);
					o--;
				}
				//std::cout << "MOVING!\t";
			}
		}
		//std::cout << "FINISH LOOP!\n";
}

void Manager::spawnOBJ(Obj3D* obj, glm::vec3 camPos, glm::vec3 camRot)
{
	Mesh mesh = *obj->mesh;
	glm::vec3 v, vc, vn;
	glm::vec2 vt;

	for (int i = 0; i < mesh.Groups.size(); i++)
	{
		Group g = *mesh.Groups[i];
		std::vector<float> vs;
		std::vector<float> vts;
		std::vector<float> vns;
		std::vector<float> vcs;
		for (int l = 0; l < g.faces.size(); l++)
		{
			Face f = *g.faces[l];

			//std::cout << "FACE" << l << "\t";
			//std::cout << "VERTICES: " << f.NumVerts << "\n";
			for (int n = 0; n < f.NumVerts; n++)
			{
				if (f.verts.size() > n)
				{
					v = *mesh.vertices[f.verts[n]];
					vs.push_back(v.x);
					vs.push_back(v.y);
					vs.push_back(v.z);
					//std::cout << "VERTICE " << f.verts[n] << "\t";
				}

				if (f.colors.size() > n)
				{
					vc = *mesh.colors[f.colors[n]];
					vcs.push_back(vc.x);
					vcs.push_back(vc.y);
					vcs.push_back(vc.z);
					//std::cout << "COLOR " << f.colors[n] << "\n";
				}

				if (f.norms.size() > n)
				{
					vn = *mesh.normals[f.norms[n]];
					vns.push_back(vn.x);
					vns.push_back(vn.y);
					vns.push_back(vn.z);
					//std::cout << "NORMAL " << f.norms[n] << "\n";
				}

				if (f.texts.size() > n)
				{
					vt = *mesh.mappings[f.texts[n]];
					vts.push_back(vt.x);
					vts.push_back(vt.y);
					//std::cout << "TEXT " << f.texts[n] << "\n";
				}

				g.TotalNumVerts++;
			}
		}
		GLuint VAO, VBOv, VBOc, VBOn, VBOt;

		//VBO - vertices
		glGenBuffers(1, &VBOv);
		glBindBuffer(GL_ARRAY_BUFFER, VBOv);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vs.size(), vs.data(), GL_STATIC_DRAW);

		//VBO - colors
		glGenBuffers(1, &VBOc);
		glBindBuffer(GL_ARRAY_BUFFER, VBOc);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vcs.size(), vcs.data(), GL_STATIC_DRAW);

		//VBO - normals
		glGenBuffers(1, &VBOn);
		glBindBuffer(GL_ARRAY_BUFFER, VBOn);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vns.size(), vns.data(), GL_STATIC_DRAW);

		//VBO - textures
		glGenBuffers(1, &VBOt);
		glBindBuffer(GL_ARRAY_BUFFER, VBOt);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vts.size(), vts.data(), GL_STATIC_DRAW);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		//VERTICES
		glBindBuffer(GL_ARRAY_BUFFER, VBOv);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		//COLORS
		glBindBuffer(GL_ARRAY_BUFFER, VBOc);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		//NORMALS
		glBindBuffer(GL_ARRAY_BUFFER, VBOn);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		//TEXTURES
		glBindBuffer(GL_ARRAY_BUFFER, VBOt);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		g.VAO = VAO;
		g.VBOvs = VBOv;
		g.VBOvcs = VBOc;
		g.VBOvns = VBOn;
		g.VBOvts = VBOt;

		std::cout << "id nova: " << g.materialID << std::endl;

		g.materialID = obj->mesh->Groups[i]->materialID;

		*mesh.Groups[i] = g;
	}

	Obj3D* o = new Obj3D;
	o->mesh = new Mesh;
	*o->mesh = mesh;

	o->position = new glm::vec3;
	*o->position = camPos + camRot;
	o->direction = new glm::vec3;
	*o->direction = camRot;
	o->scale = new glm::vec3();
	*o->scale = *obj->scale;
	o->angle = obj->angle;
	o->rotation = new glm::vec3;
	*o->rotation = *obj->rotation;

	o->initialTime = glfwGetTime();
	o->timeout = obj->timeout + o->initialTime;
	o->speed = obj->speed;
	o->animated = true;
	o->isMoving = true;
	//Obj3D* o = new Obj3D;
	//o = obj;
	objs.push_back(o);
}

glm::vec3 Manager::scaleObj(glm::vec3 min, glm::vec3 max)
{
	float x, y, z;
	//std::cout << max.x << std::endl;
	//std::cout << min.x << std::endl;

	x = max.x - min.x;
	y = max.y - min.y;
	z = max.z - min.z;
	//std::cout << x << std::endl;
	
	x = powf(x, 2);
	y = powf(y, 2);
	z = powf(z, 2);
	//std::cout << x << std::endl;

	float d;

	d = sqrtf(x + y + z);
	//std::cout << 1/d << std::endl;

	return glm::vec3(1/d);
}

void Manager::VerifyColors(Mesh* mesh)
{
	if (mesh->colors.size() <= 0)
	{
		//std::cout << "COLORS\t";
		glm::vec3* vc = new glm::vec3(1.0, 0.0, 0.0);
		mesh->colors.push_back(vc);

		//std::cout << mesh->colors.size() << std::endl;

		for (int i = 0; i < mesh->Groups.size(); i++)
		{
			Group g = *mesh->Groups[i];
			for (int j = 0; j < g.faces.size(); j++)
			{
				//std::cout << "VC " << j << ":\t";
				for (int n = 0; n < g.faces[j]->NumVerts; n++)
				{
					g.faces[j]->colors.push_back(0);
					//std::cout << 0 << "\t";
				}
				//std::cout << std::endl;
			}
			*mesh->Groups[i] = g;
		}
	}
}
