#version 410

layout (location = 0) in vec3 position;

uniform mat4 view, proj, matrix;

void main () {
	gl_Position = proj * view * matrix * vec4 (position, 1.0);
};