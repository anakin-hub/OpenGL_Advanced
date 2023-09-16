#version 410

const int MAX_LIGHTS = 4;

struct Light {
    vec3 position;
	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 N;
in vec3 FragPos, pos_eye, normal_eye;;


uniform Light lights[MAX_LIGHTS];
uniform vec3 ka, kd, ks;
uniform float ns;

out vec4 frag_color;

void main () {
    // Ambient lighting
    vec3 ambient = vec3(0.0);
    //for (int i = 0; i < MAX_LIGHTS; i++) {
        ambient += lights[0].ambient * ka;
    //}
	
	// Diffuse lighting
    vec3 diffuse = vec3(0.0);
    for (int i = 0; i < MAX_LIGHTS; i++) {
		float d = length(lights[i].position - FragPos);
		float fatt = 1/(d * d);
        vec3 lightDir = normalize(lights[i].position - FragPos);
        float diff = max(dot(N, lightDir), 0.0);
        diffuse += lights[i].diffuse * (fatt * diff * kd);
    }
	
	// Specular lighting
    vec3 specular = vec3(0.0);
    for (int i = 0; i < MAX_LIGHTS; i++) {
        vec3 lightDir = normalize(lights[i].position - FragPos);
        vec3 viewDir = normalize(-pos_eye);
        vec3 reflectDir = reflect(-lightDir, normal_eye);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), ns);
        specular += lights[i].specular * (spec * ks);
    }

    vec3 result = ambient + diffuse + specular;
    frag_color = vec4(result, 1.0);
};