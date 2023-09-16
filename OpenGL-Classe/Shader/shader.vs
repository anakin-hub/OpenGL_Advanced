#version 410

layout(location=0) in vec3 vp;
layout(location=1) in vec3 vc;

out vec3 color;

uniform mat4 view, proj, matrix;

void main () {
	color = vc;
	gl_Position = proj * view * matrix * vec4 (vp, 1.0);
};