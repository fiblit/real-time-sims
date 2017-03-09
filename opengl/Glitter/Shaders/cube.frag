#version 330 core

out vec4 color;

uniform vec3 objectColor;
uniform vec3 lightColor;

in vec3 LightPos;
in vec3 Normal;
in vec3 FragPos;

void main() {
	// Ambiet
	float ambientStrength = 0.1f;
	float ambient = ambientStrength;
	
	// Diffuse
	float diffuseStrength = 1.0f;
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(LightPos - FragPos);
	float diffuse = diffuseStrength * max(dot(lightDir, norm), 0.0);

	// Specular
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(- FragPos);
	vec3 reflectDir = reflect(-lightDir, norm); // reflect by a's head at b's tail
	float specular = specularStrength * pow(max(dot(viewDir, reflectDir), 0.0), 32);

	vec3 result = ((ambient + diffuse + specular) * lightColor) * objectColor;
	color = vec4(result, 1.0f);
}
