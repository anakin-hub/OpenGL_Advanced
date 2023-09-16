#include "Manager.h"
#include "Shader.h"

struct LightPoint
{
	glm::vec3 Ip;
	glm::vec3 Pl;
};

//Functionss
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void readScene(std::string filename);

// Camera
glm::mat4 view;
glm::vec3 cameraPos, cameraUp, cameraDirection;
float deltaTime, lastFrame, yaw = -90.0f, pitch = 0.0f;

// Mouse
float lastX, lastY;
bool firstMouse = true;

// AnimatedOBJ
int id_anim = -1;

// Window dimensions
float WIDTH = 800, HEIGHT = 600;

// Shaders
Shader shader;
//const char* vertex_shader =
//"#version 410\n"
//"layout(location=0) in vec3 vp;"
//"layout(location=1) in vec3 vc;"
//"out vec3 color;"
//"uniform mat4 view, proj, matrix;"//matrix = model -> matriz de modelo, transformações do objeto
//"void main () {"
//" color = vc;"
//" gl_Position = proj * view * matrix * vec4 (vp, 1.0);"
//"}";
//
//const char* fragment_shader =
//"#version 410\n"
//"in vec3 color;"
//"out vec4 frag_color;"
//"void main () {"
//" frag_color = vec4 (color, 1.0);"
//"}";

Manager manager;

int main() {
	readScene("config.txt");

	lastFrame = deltaTime = 0.0;
	lastX = WIDTH / 2;
	lastY = HEIGHT / 2;

	if (!manager.initialize(WIDTH, HEIGHT))
	{		
		std::cout << "INITIALIZATION ERROR!\n";
		return 1;
	}

	glfwSetInputMode(manager.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(manager.window, key_callback);
	glfwSetCursorPosCallback(manager.window, mouse_callback);

	//// identifica vs e o associa com vertex_shader
	//GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	//glShaderSource(vs, 1, &vertex_shader, NULL);
	//glCompileShader(vs);
	//// identifica fs e o associa com fragment_shader
	//GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	//glShaderSource(fs, 1, &fragment_shader, NULL);
	//glCompileShader(fs);
	//// identifica do programa, adiciona partes e faz "linkagem"
	//GLuint shader_programme = glCreateProgram();
	//glAttachShader(shader_programme, fs);
	//glAttachShader(shader_programme, vs);
	//glLinkProgram(shader_programme);

	//shader.loadFile("lightShader_backup");
	shader.loadFile("lightShader");

	//std::cout << "shader created\n";

	manager.configVBO();

	//glUseProgram(shader_programme);
	shader.Use();

	////LUZ AMBIENTE
	//shader.setVec3("Ia", 0.2f, 0.2f, 0.2f);
	////LUZ DIFUSA
	//shader.setVec3("Ip", 0.5f, 0.5f, 0.5f);
	////PONTO DE LUZ
	//shader.setVec3("Pl", 0.0f, 0.5f, 0.0f);

	//LUZ AMBIENTE
	shader.setVec3("lights[0].ambient", 0.1f, 0.1f, 0.1f);
	//LUZ DIFUSA
	shader.setVec3("lights[0].diffuse", 0.3f, 0.3f, 0.3f);
	//LUZ ESPECULAR
	shader.setVec3("lights[0].specular", 0.5f, 0.5f, 0.5f);
	//PONTO DE LUZ
	shader.setVec3("lights[0].position", 0.0f, 1.0f, -1.505f);

	//LUZ AMBIENTE
	shader.setVec3("lights[1].ambient", 0.1f, 0.1f, 0.1f);
	//LUZ DIFUSA
	shader.setVec3("lights[1].diffuse", 0.3f, 0.3f, 0.3f);
	//LUZ ESPECULAR
	shader.setVec3("lights[1].specular", 0.5f, 0.5f, 0.5f);
	//PONTO DE LUZ
	shader.setVec3("lights[1].position", 0.0f, 0.5f, 0.0f);


	glm::mat4 proj = glm::perspective(glm::radians(45.0f), WIDTH / HEIGHT, 0.1f, 100.0f);

	view = glm::lookAt(cameraPos, cameraPos + cameraDirection, cameraUp);

	shader.setMat4("view", view);
	//GLint viewLoc = glGetUniformLocation(shader_programme, "view");
	//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	shader.setMat4("proj", proj);
	//GLint projLoc = glGetUniformLocation(shader_programme, "proj");
	//glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

	std::cout << "ALL READY\n";

	while (!glfwWindowShouldClose(manager.window)) 
	{
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float currentFrame = glfwGetTime(); 
		deltaTime = currentFrame - lastFrame; 
		lastFrame = currentFrame;

		view = glm::lookAt(cameraPos, cameraPos + cameraDirection, cameraUp);

		shader.setMat4("view", view);
		//viewLoc = glGetUniformLocation(shader_programme, "view");
		//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		manager.drawScene(shader, deltaTime);//PRECISO VERIFICAR SE FUNCIONA

		glfwSwapBuffers(manager.window);
		glfwPollEvents();
	}

	// encerra contexto GL e outros recursos da GLFW
	glfwTerminate();
	return 0;
}

void readScene(std::string filename)
{
	std::ifstream arq(filename);

	std::string line;

	while (getline(arq, line))
	{
		std::stringstream sline;

		sline << line;

		std::string temp;

		sline >> temp;
		if (temp == "obj")
		{
			int id = -1, col, eli;
			std::string file_obj;
			float x, y, z, s, r;

			sline >> id;
			id--;
			sline >> file_obj;
			manager.read(file_obj);
			sline >> x >> y >> z;
			manager.objs[id]->position = new glm::vec3(x, y, z);
			sline >> s;
			manager.objs[id]->scale = new glm::vec3(s);			
			sline >> r >> x >> y >> z;
			manager.objs[id]->angle = r;
			manager.objs[id]->rotation = new glm::vec3(x, y, z);
			int c, e, a, car;
			sline >> c >> e >> a >> car;
			if (c == 0)
			{
				manager.objs[id]->collider = false;
			}
			else
			{
				manager.objs[id]->collider = true;
			}

			if (e == 0)
			{
				manager.objs[id]->eliminable = false;
			}
			else
			{
				manager.objs[id]->eliminable = true;
			}

			if (a == 0)
			{
				manager.objs[id]->animated = false;
			}
			else
			{
				manager.objs[id]->animated = true;
				float speed, tout;
				sline >> speed >> tout;
				manager.objs[id]->speed = speed;
				manager.objs[id]->timeout = tout;
				id_anim = id;
			}

			if (car == 0)
			{
				manager.objs[id]->car = false;
			}
			else
			{
				manager.objs[id]->car = true;
				manager.objs[id]->xlr8 = false;
			}
		}
		else if (temp == "cam")
		{
			float x, y, z;
			sline >> x >> y >> z;
			cameraPos = glm::vec3(x, y, z);
			sline >> x >> y >> z;
			cameraDirection = glm::vec3(x, y, z);
			sline >> x >> y >> z;
			cameraUp = glm::vec3(x, y, z);
		}
		else if (temp == "vp")
		{
			float w, h;
			sline >> w >> h;
			WIDTH = w;
			HEIGHT = h;
		}
		else if (temp == "anim")
		{
			int id = -1;
			std::string file_anim;
			float x, y, z, s, r;

			sline >> id;
			id--;
			sline >> file_anim;
			manager.readAnim(file_anim);
			sline >> x >> y >> z;
			//manager.objs[id]->position = new glm::vec3(x, y, z);
			sline >> s;
			//manager.objs[id]->scale = new glm::vec3(s);
			sline >> r >> x >> y >> z;
			//manager.objs[id]->angle = r;
			//manager.objs[id]->rotation = new glm::vec3(x, y, z);
		}
	}
	//std::cout << "Scene ok\n";
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	float cameraSpeed = 10.f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraDirection;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraDirection;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraDirection, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraDirection, cameraUp)) * cameraSpeed;

	//if (cameraPos.y != 0.0)
	//	cameraPos.y = 0.0;

	//if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) //DESABILITADO EM PROL DE DESCOBRIR COMO CRIAR O TIRO NOVAMENTE AGR MANDANDO AS NOVAS INFOS DE ILUMINAÇÃO
	//{
	//	if(id_anim >= 0)
	//		manager.spawnOBJ(manager.objs[id_anim], cameraPos, cameraDirection);
	//}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraDirection = glm::normalize(direction);
}