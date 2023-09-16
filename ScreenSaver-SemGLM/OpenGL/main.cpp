#include <iostream>
#include <cmath>

using namespace std;

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// STRUCTS
struct vector
{
	float x;
	float y;
};

// FUNCTIONS
float DotProduct(float x1, float y1, float x2, float y2);
float DotProduct(vector N, vector Rin);
vector Reflexion(vector N, vector Rin);

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
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, // Bottom Right
		-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, // Bottom Left
		0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f // Top
	};

	// 1 VBO e 1 VAO
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Posições:
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Cores:
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Setup Model Matrix
	float matrix[] = {
		0.5f,	0.f,	0.f,	0.f, 
		0.f,	0.5f,	0.f,	0.f,
		0.f,	0.f,	0.5f,	0.f,
		0.f,	0.f,	0.f,	1.f
	};

	//setup screensaver
	bool impact = false;
	float PosX, PosY, Speed, angle;
	PosX = 0.f;
	PosY = -0.75f;
	Speed = 0.01;

	vector N, Rin;
	N.x = 0.f;
	N.y = 1.f;
	Rin.x = 0.45f;
	Rin.y = 0.45f;

	angle = atan2f(Rin.y, Rin.x);
	Rin.x = cosf(angle);
	Rin.y = sinf(angle);

	// Define shader_programme como o shader a ser utilizado
	glUseProgram(shader_programme);
	int matrixLocation = glGetUniformLocation(shader_programme, "matrix");
	glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, matrix);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		if (fabs(PosX) > 0.75f && !impact) {
			N.x = 1 * (PosX / -(fabs(PosX)));
			N.y = 0;
			impact = true;			
		}
		if (fabs(PosY) > 0.75f && !impact) {
			N.y = 1 * (PosY / -(fabs(PosY)));
			N.x = 0;
			impact = true;
		}

		if (impact)
		{
			Rin.x = -Rin.x;
			Rin.y = -Rin.y;
			Rin = Reflexion(N, Rin);

			/*cout << "X: " << Rin.x << "\t";
			cout << "Y: " << Rin.y << "\t";*/

			vector t;
			
			angle = atan2f(Rin.y, Rin.x);
			t.x = cosf(angle);
			t.y = sinf(angle);

			/*cout << "X: " << t.x << "\t";
			cout << "Y: " << t.y << "\n";*/

			Rin = t;
			impact = false;
		}


		matrix[12] = PosX + (Speed * Rin.x);
		PosX = matrix[12];
		matrix[13] = PosY + (Speed * Rin.y);
		PosY = matrix[13];


		//Envia a matriz para o shader
		glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, matrix);
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

float DotProduct(float x1, float y1, float x2, float y2)
{
	float dot = 0;
	dot = (x1 * x2) + (y1 * y2);
	return dot;
}

float DotProduct(vector N, vector Rin)
{
	float R;

	R = N.x * Rin.x + N.y * Rin.y;

	return R;
}

vector Reflexion(vector N, vector Rin)
{
	vector R;
	float dp = DotProduct(N, Rin);
	N.x = 2 * dp * N.x;
	N.y = 2 * dp * N.y;
	R.x = N.x - Rin.x;
	R.y = N.y - Rin.y;

	return R;
}
