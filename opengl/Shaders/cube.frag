#version 330 core
struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shine; 
};

struct Light {
	vec3 pos;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;
uniform Light light;

out vec4 color;

in vec3 Normal;
in vec3 FragPos;

void main() {
	// Ambient
	vec3 ambient = light.ambient * material.ambient;
	
	// Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.pos - FragPos);
	vec3 diffuse = light.diffuse * material.diffuse * max(dot(lightDir, norm), 0.0);

	// Specular
	vec3 viewDir = normalize(- FragPos);
	vec3 reflectDir = reflect(-lightDir, norm); // reflect by a's head at b's tail
	vec3 specular = light.specular * material.specular * pow(max(dot(viewDir, reflectDir), 0.0), 32);

	vec3 result = ambient + diffuse + specular;
	color = vec4(result, 1.0f);
}
