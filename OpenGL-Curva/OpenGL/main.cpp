#include "Manager.h"
#include "Shader.h"

//Functionss
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

// Camera
glm::mat4 view;
glm::vec3 cameraPos, cameraUp, cameraDirection;

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
	lastX = WIDTH / 2;
	lastY = HEIGHT / 2;

	if (!manager.initialize(WIDTH, HEIGHT))
	{		
		std::cout << "INITIALIZATION ERROR!\n";
		return 1;
	}

	glfwSetInputMode(manager.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetKeyCallback(manager.window, key_callback);
	glfwSetMouseButtonCallback(manager.window, mouseButtonCallback);

	shader.loadFile("shader");
	//std::cout << "shader created\n";

	manager.configVBO();
	//manager.basicVBO();

	shader.Use();

	// Set the viewport size
	glViewport(0, 0, 800, 600);

	// Set the orthographic projection parameters
	float left = -1.0f;
	float right = 1.0f;
	float bottom = -1.0f;
	float top = 1.0f;
	float near = -1.0f;
	float far = 1.0f;

	// Calculate the orthographic projection matrix
	glm::mat4 proj = glm::ortho(left, right, bottom, top, near, far);

	//view = glm::lookAt(cameraPos, cameraPos + cameraDirection, cameraUp);
	view = glm::mat4(1.0f);

	shader.setMat4("view", view);
	//GLint viewLoc = glGetUniformLocation(shader_programme, "view");
	//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	shader.setMat4("proj", proj);
	//GLint projLoc = glGetUniformLocation(shader_programme, "proj");
	//glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

	glm::mat4 transform = glm::mat4(1.0f);
	shader.setMat4("matrix", transform);

	std::cout << "ALL READY\n";

	while (!glfwWindowShouldClose(manager.window)) 
	{
		//glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		manager.drawScene(shader);
		//manager.basicDraw(shader);

		glfwSwapBuffers(manager.window);
		glfwPollEvents();
	}

	// encerra contexto GL e outros recursos da GLFW
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
	{
		if (manager.lines[0].points.size() > 3)
		{
			manager.saveCurve();
			manager.saveB_spline();
			manager.curve_saved = true;
		}
	}
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		//std::cout << "P: " << xpos << "\t" << ypos << std::endl;

		float normalizedX = (xpos / 400.0) - 1.0;
		float normalizedY = 1.0 - (ypos / 300.0);


		manager.sendVertice(normalizedX, normalizedY);
	}
}
