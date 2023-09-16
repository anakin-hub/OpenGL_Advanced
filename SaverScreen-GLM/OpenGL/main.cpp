#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;

// Shaders
const char* vertex_shader =
"#version 410\n"
"layout(location=0) in vec3 vp;"
"layout(location=1) in vec3 vc;"
"out vec3 color;"
"uniform mat4 matrix;"
"void main () {"
" color = vc;"
" gl_Position = matrix * vec4 (vp, 1.0);"
"}";

const char* fragment_shader =
"#version 410\n"
"in vec3 color;"
"out vec4 frag_color;"
"void main () {"
" frag_color = vec4 (color, 1.0);"
"}";


int main() {
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}
	/* Caso necessário, definições específicas para SOs, p. e. Apple OSX *
	/* Definir como 3.2 para Apple OS X */
	/*glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/
	GLFWwindow* window = glfwCreateWindow( 640, 480, "Screensaver", NULL, NULL);

	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);
	// inicia manipulador da extensão GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	// identifica vs e o associa com vertex_shader
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	// identifica fs e o associa com fragment_shader
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);
	// identifica do programa, adiciona partes e faz "linkagem"
	GLuint shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);

	GLfloat vertices[] = {
		// Positions // Colors
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom Right
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // Bottom Left
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f // Top
	};
	// 1 VBO e 1 VAO
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
		vertices, GL_STATIC_DRAW);
	// Posições:
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Cores:
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
		6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//setup screensaver
	float PosX, PosY, SpeedX, SpeedY;
	PosX = 0.f;
	PosY = -0.5f;
	SpeedX = 0.0001;
	SpeedY = 0.0001;

	// Define shader_programme como o shader a ser utilizado
	glUseProgram(shader_programme);
	glm::mat4 matrix = glm::mat4(1.0f);
	//matrix = glm::translate(matrix, glm::vec3(0.0f, -0.5f, 0.0f));
	//matrix = glm::rotate(matrix, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	//matrix = glm::scale(matrix, glm::vec3(0.5, 0.5, 0.5));
	int matrixLocation = glGetUniformLocation(shader_programme, "matrix");
	glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, glm::value_ptr(matrix));

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Posição X e Y do centro do objeto
		PosX += SpeedX;
		PosY += SpeedY;

		//LIMITES DA BORDA
		if (PosX >= 0.75f || PosX <= -.75f)
			SpeedX *= -1.f;
		if (PosY >= 0.75f || PosY <= -.75f)
			SpeedY *= -1.f;
		
		matrix = glm::mat4(1.0f);
		matrix = glm::translate(matrix, glm::vec3(PosX, PosY, 0.0f));
		matrix = glm::scale(matrix, glm::vec3(0.5, 0.5, 0.5));
		//LEMBRETE: para mudar a escala tem que mudar os limites da borda 
		glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, glm::value_ptr(matrix));

		//EXPLICAÇÃO: ao pegar e definir a posição x e y do centro do objeto combinado com a glm, toda vez que for atualizar
		//o código reseta a posição do triangulo, e passa uma nova posição pra ele, ai essa nova posição é enviada para o shader
		//e então o triangulo é desenhado.
		//é necessario fazer um calculo de onde vai ser os limites do triangulo, manualmente, por isso caso mude a escala,
		//os limites DEVERÃO ser alterados
		//ao chegar no limite, é alterado a direção dos floats de velocidade que indicam o sentido em que esta indo o triangulo

		// Define vao como verte array atual
		glBindVertexArray(VAO);
		// desenha pontos a partir do p0 e 3 no total do VAO atual com o shader atualmente em uso
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}		
	
	// encerra contexto GL e outros recursos da GLFW
	glfwTerminate();
	return 0;
}