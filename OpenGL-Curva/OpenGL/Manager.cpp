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

	curve_saved = false;
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

	window = glfwCreateWindow(w, h, "Curve Writer", NULL, NULL);

	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		return false;
	}

	curve_saved = false;
	return true;
}

void Manager::basicVBO()
{
	GLfloat vertices[] = {
	-0.5f, -0.5f, 0.0f,  // Vertex 1: position
	1.0f, 0.0f, 0.0f,    // Vertex 1: color (red)
	0.5f, -0.5f, 0.0f,   // Vertex 2: position
	0.0f, 1.0f, 0.0f,    // Vertex 2: color (green)
	0.0f, 0.5f, 0.0f,    // Vertex 3: position
	0.0f, 0.0f, 1.0f     // Vertex 3: color (blue)
	};

	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	// Bind the VAO
	glBindVertexArray(vao);

	// Bind the VBO and set the vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Specify the vertex attribute pointers
	glEnableVertexAttribArray(0); // Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(1); // Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	// Unbind the VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Manager::basicDraw(Shader& shader)
{
	// Use the shader program
	shader.Use();

	glm::mat4 transform = glm::mat4(1.0f);
	shader.setMat4("matrix", transform);

	// Bind the VAO
	glBindVertexArray(vao);

	// Draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// Unbind the VAO
	glBindVertexArray(0);
}

void Manager::configVBO()//colocar no objeto direto?
{	
	for (int i = 0; i < 4; i++)
	{
		//VAO - creation
		glGenVertexArrays(1, &lines[i].VAO);
		glBindVertexArray(lines[i].VAO);

		//VBO - vertices with no data
		glGenBuffers(1, &lines[i].VBO);
		glBindBuffer(GL_ARRAY_BUFFER, lines[i].VBO);
		glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

		//SHADER POINTER CONFIG
		GLint vertexAttributeLocation = 0;
		GLint vertexAttributeSize = 3;

		//SHADER POINTER LINK
		glEnableVertexAttribArray(vertexAttributeLocation);
		glVertexAttribPointer(vertexAttributeLocation, vertexAttributeSize, GL_FLOAT, GL_FALSE, 0, nullptr);
	}

	lines[0].color = glm::vec3(0.0f, 1.0f, 1.0f);
	lines[1].color = glm::vec3(1.0f, 1.0f, 1.0f);
	lines[2].color = glm::vec3(0.0f, 1.0f, 0.0f);
	lines[3].color = glm::vec3(1.0f, 0.0f, 0.0f);
	m = 0.05f;
	//std::cout << "VBO!\n";
}

void Manager::drawScene(Shader& shader)
{
	for (int i = 0; i < 4; i++)
	{
		glBindVertexArray(lines[i].VAO);

		shader.setVec3("color", lines[i].color);

		if (i == 0 && !curve_saved)
		{
			glPointSize(5.0f);
			glDrawArrays(GL_POINTS, 0, lines[i].points.size());
		}

		if (lines[i].points.size() > 3 && i > 0)
		{
			glDrawArrays(GL_LINE_STRIP, 0, lines[i].points.size());
		}
	}
}

void Manager::sendVertice(float x, float y)
{
	glm::vec3 v(x, y, 0.0f);

	std::cout << "vertice: " << v.x << "\t" << v.y << "\t" << v.z << std::endl;

	lines[0].points.push_back(v);

	std::vector<GLfloat> vs;

	for (int i = 0; i < lines[0].points.size(); i++)
	{
		vs.push_back(lines[0].points[i].x);
		vs.push_back(lines[0].points[i].y);
		vs.push_back(lines[0].points[i].z);
	}

	glBindBuffer(GL_ARRAY_BUFFER, lines[0].VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vs.size(), vs.data(), GL_DYNAMIC_DRAW);

	if(lines[0].points.size() > 3)
		B_spline();
}

void Manager::B_spline()
{
	if (!lines[1].points.empty())
	{
		lines[1].points.clear();
	}

	std::cout << "starting B-SPLine\n";

	float inc = 1.0f / 100;
	int N = lines[0].points.size();
	for (int i = 0; i < N; i++) {
		for (float t = 0; t <= 1; t += inc) {
			float x, y, z;

			x = (((-1 * pow(t, 3) + 3 * pow(t, 2) - 3 * t + 1) * lines[0].points[i].x +
				(3 * pow(t, 3) - 6 * pow(t, 2) + 0 * t + 4) * lines[0].points[(i + 1) % N].x +
				(-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1) * lines[0].points[(i + 2) % N].x +
				(1 * pow(t, 3) + 0 * pow(t, 2) + 0 * t + 0) * lines[0].points[(i + 3) % N].x) / 6);

			y = (((-1 * pow(t, 3) + 3 * pow(t, 2) - 3 * t + 1) * lines[0].points[i].y +
				(3 * pow(t, 3) - 6 * pow(t, 2) + 0 * t + 4) * lines[0].points[(i + 1) % N].y +
				(-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1) * lines[0].points[(i + 2) % N].y +
				(1 * pow(t, 3) + 0 * pow(t, 2) + 0 * t + 0) * lines[0].points[(i + 3) % N].y) / 6);
			
			z = (((-1 * pow(t, 3) + 3 * pow(t, 2) - 3 * t + 1) * lines[0].points[i].z +
				(3 * pow(t, 3) - 6 * pow(t, 2) + 0 * t + 4) * lines[0].points[(i + 1) % N].z +
				(-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1) * lines[0].points[(i + 2) % N].z +
				(1 * pow(t, 3) + 0 * pow(t, 2) + 0 * t + 0) * lines[0].points[(i + 3) % N].z) / 6);

			glm::vec3 p(x, y, z);

			lines[1].points.push_back(p);
		}
	}

	std::vector<GLfloat> vs;

	for (int i = 0; i < lines[1].points.size(); i++)
	{
		vs.push_back(lines[1].points[i].x);
		vs.push_back(lines[1].points[i].y);
		vs.push_back(lines[1].points[i].z);
	}

	glBindBuffer(GL_ARRAY_BUFFER, lines[1].VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vs.size(), vs.data(), GL_STATIC_DRAW);

	std::cout << "done!\n";

	In_Curve();
	Out_Curve();
}

void Manager::Out_Curve()//3
{
	if (!lines[3].points.empty())
	{
		lines[3].points.clear();
	}

	for (int i = 0; i < lines[1].points.size() - 1; i++)
	{
		glm::vec3 A, B, C;

		A = B = C = glm::vec3(0.0f);

		A = lines[1].points[i];
		B = lines[1].points[i + 1];

		float w, h;

		w = B.x - A.x;
		h = B.y - A.y;

		float angle;

		angle = glm::atan(h / w);

		if (w < 0)
		{
			angle += glm::pi<float>() / 2;
		}
		else
		{
			angle -= glm::pi<float>() / 2;
		}

		C.x = glm::cos(angle) * m + A.x;
		C.y = glm::sin(angle) * m + A.y;

		lines[3].points.push_back(C);
	}

	std::vector<GLfloat> vs;

	for (int i = 0; i < lines[3].points.size(); i++)
	{
		vs.push_back(lines[3].points[i].x);
		vs.push_back(lines[3].points[i].y);
		vs.push_back(lines[3].points[i].z);
	}

	glBindBuffer(GL_ARRAY_BUFFER, lines[3].VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vs.size(), vs.data(), GL_STATIC_DRAW);
}

void Manager::In_Curve()//2
{
	if (!lines[2].points.empty())
	{
		lines[2].points.clear();
	}

	for (int i = 0; i < lines[1].points.size()-1; i++)
	{
		glm::vec3 A, B, C;

		A = B = C = glm::vec3(0.0f);

		A = lines[1].points[i];
		B = lines[1].points[i + 1];

		float w, h;

		w = B.x - A.x;
		h = B.y - A.y;

		float angle;

		angle = glm::atan(h / w);

		if (w < 0)
		{
			angle -= glm::pi<float>() / 2;
		}
		else
		{
			angle += glm::pi<float>() / 2;
		}

		C.x = glm::cos(angle) * m + A.x;
		C.y = glm::sin(angle) * m + A.y;

		lines[2].points.push_back(C);
	}

	std::vector<GLfloat> vs;

	for (int i = 0; i < lines[2].points.size(); i++)
	{
		vs.push_back(lines[2].points[i].x);
		vs.push_back(lines[2].points[i].y);
		vs.push_back(lines[2].points[i].z);
	}

	glBindBuffer(GL_ARRAY_BUFFER, lines[2].VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vs.size(), vs.data(), GL_STATIC_DRAW);
}

void Manager::saveCurve()
{
	Mesh m;
	glm::vec3* v;
	//VERTICES
	for (int i = 2; i < 4; i++)
	{
		for (int l = 0; l < lines[i].points.size(); l++)//starting from internal
		{
			v = new glm::vec3(lines[i].points[l].x, lines[i].points[l].z, lines[i].points[l].y);
			m.vertices.push_back(v);
		}
	}
	//NORMAL
	v = new glm::vec3(0.0f, 1.0f, 0.0f);
	m.normals.push_back(v);
	//TEXTURES
	glm::vec2* vt;
	vt = new glm::vec2(0.f, 0.f);
	m.mappings.push_back(vt);
	vt = new glm::vec2(1.f, 0.f);
	m.mappings.push_back(vt);
	vt = new glm::vec2(1.f, 1.f);
	m.mappings.push_back(vt);
	vt = new glm::vec2(0.f, 1.f);
	m.mappings.push_back(vt);

	Group g;
	int N = lines[2].points.size();
	for (int i = 0; i < N-1; i++)
	{
		Face* f; // v/t/n
		f = new Face;
		//F1
		f->verts.push_back(i);
		f->texts.push_back(1);
		f->norms.push_back(1);

		f->verts.push_back(i+1);
		f->texts.push_back(2);
		f->norms.push_back(1);

		f->verts.push_back(i+N);
		f->texts.push_back(4);
		f->norms.push_back(1);

		f->NumVerts = 3;

		g.faces.push_back(f);
		
		f = new Face;
		//F2
		f->verts.push_back(i+N);
		f->texts.push_back(4);
		f->norms.push_back(1);

		f->verts.push_back(i + 1);
		f->texts.push_back(2);
		f->norms.push_back(1);

		f->verts.push_back(i + 1 + N);
		f->texts.push_back(3);
		f->norms.push_back(1);

		f->NumVerts = 3;

		g.faces.push_back(f);
	}

	std::string folderPath = "../";
	std::string fileName = "pista.obj";
	std::string filePath = folderPath + fileName;

	std::ofstream outputFile(filePath);

	if (outputFile.is_open()) 
	{
		for (int i = 0; i < m.vertices.size(); i++)
		{
			glm::vec3 v = *m.vertices[i];

			outputFile << "v " << v.x << " " << v.y << " " << v.z << std::endl;
		}

		for (int i = 0; i < m.mappings.size(); i++)
		{
			glm::vec2 v = *m.mappings[i];

			outputFile << "vt " << v.x << " " << v.y << std::endl;
		}

		for (int i = 0; i < m.normals.size(); i++)
		{
			glm::vec3 v = *m.normals[i];

			outputFile << "vn " << v.x << " " << v.y << " " << v.z << std::endl;
		}

		outputFile << "mtllib pista.mtl\n";
		outputFile << "g pista\n";
		outputFile << "usemtl pista\n";

		for (int i = 0; i < g.faces.size(); i++)
		{
			outputFile << "f";

			for (int f = 0; f < g.faces[i]->NumVerts; f++)
			{
				Face* face = new Face;
				face = g.faces[i];

				int v, t, n;
				v = face->verts[f];
				t = face->texts[f];
				n = face->norms[f];

				outputFile << " " << v << "/" << t << "/" << n;
			}

			outputFile << std::endl;
		}
				
		outputFile.close();
		std::cout << "OBJ created successfully." << std::endl;
	}
	else 
	{
		std::cout << "Failed to create the file." << std::endl;
	}

	outputFile.open("../pista.mtl");
	if (outputFile.is_open())
	{
		outputFile << "newmtl pista\n";
		float r, g, b;
		r = g = b = 1.0;
		outputFile << "Ka " << r << " " << g << " " << b << std::endl;
		r = g = b = 0.0;
		outputFile << "Kd " << r << " " << g << " " << b << std::endl;
		outputFile << "Ks " << r << " " << g << " " << b << std::endl;
		float ns = 20.0;
		outputFile << "Ns " << ns << std::endl;

		outputFile.close();
		std::cout << "MTL created successfully." << std::endl;
	}
	else
	{
		std::cout << "Failed to create the file." << std::endl;
	}
}

void Manager::saveB_spline()
{
	Mesh m;
	glm::vec3* v;
	//VERTICES
	for (int l = 0; l < lines[1].points.size(); l++)//starting from internal
	{
		v = new glm::vec3(lines[1].points[l].x, lines[1].points[l].z, lines[1].points[l].y);
		m.vertices.push_back(v);
	}
	
	std::string folderPath = "../";
	std::string fileName = "curve.anim";
	std::string filePath = folderPath + fileName;

	std::ofstream outputFile(filePath);

	if (outputFile.is_open())
	{
		for (int i = 0; i < m.vertices.size(); i++)
		{
			glm::vec3 v = *m.vertices[i];

			outputFile << "v " << v.x << " " << v.y << " " << v.z << std::endl;
		}

		for (int i = 0; i < m.normals.size(); i++)
		{
			glm::vec3 v = *m.normals[i];

			outputFile << "vn " << v.x << " " << v.y << " " << v.z << std::endl;
		}

		outputFile.close();
		std::cout << "Curve animation created successfully." << std::endl;
	}
	else
	{
		std::cout << "Failed to create the file." << std::endl;
	}
}
