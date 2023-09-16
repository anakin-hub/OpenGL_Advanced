#version 410

layout(location=0) in vec3 vp;
layout(location=1) in vec3 vc;
layout(location=2) in vec3 vn;
layout(location=3) in vec2 vt;

out vec3 N;
out vec3 FragPos, pos_eye, normal_eye;

uniform mat4 view, proj, matrix;

void main () {
	pos_eye = vec3(view * matrix * vec4(vp, 1.0));
	normal_eye = vec3(view * matrix * vec4(vn, 0.0));
	
	FragPos = vec3(matrix * vec4(vp, 1.0));
	N = mat3(transpose(inverse(matrix))) * vn;
	
	gl_Position = proj * vec4 (pos_eye, 1.0);
};