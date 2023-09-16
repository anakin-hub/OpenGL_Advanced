#version 410

in vec3 N;
in vec3 FragPos, pos_eye, normal_eye;;
in vec2 TextCoord;

uniform vec3 Pl;
uniform vec3 Ia, Ip;
uniform vec3 ka, kd, ks;
uniform float ns;
uniform sampler2D dif;
uniform sampler2D spec;

out vec4 frag_color;

void main () {
	vec3 ambient = Ia * ka;
	
	vec3 norm = normalize(N);
	float d = length(Pl - FragPos);
	float fatt = 1/(d * d);
	vec3 L = normalize(Pl - FragPos);
	float Ad = dot(norm, L);
	float diff = max(Ad, 0.0);
	vec3 diffuse = fatt * Ip * kd * diff;
	
	vec3 V = normalize(pos_eye - FragPos);
	vec3 R = reflect(N, -L);
	float As = max(dot(V,R), 0.0);
	float s = pow(As, ns);
	vec3 specular = ks * s;
	
	vec3 I = ambient + diffuse + specular;
	frag_color = vec4 (I, 1.0);
};