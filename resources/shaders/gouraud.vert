#version 330 core

struct Material
{
	vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct PointLight
{
	int active;
	vec3 position;
	vec3 color;
	
	float attConstant;
	float attLinear;
	float attQuadratic;
};

uniform float uAmbientStrength;
uniform float uDiffuseStrength;
uniform float uSpecularStrength;
uniform int uSpecularExponent;
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform vec3 uViewPos;
uniform Material uMaterial;

#define MAX_LIGHTS 16
uniform PointLight uLight[MAX_LIGHTS];

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Light;


vec3 pointLight(PointLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - FragPos);
		
	// Ambient
	vec3 ambient = uMaterial.ambient * uAmbientStrength;
	
	// Diffuse
	vec3 diffuse = uMaterial.diffuse * max(0.0, dot(normal, lightDir)) * uDiffuseStrength;
	
	// Specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float specAngle = max(0.0, dot(viewDir, reflectDir));
	float specularStrength = pow(specAngle, uMaterial.shininess) * uSpecularStrength;
	vec3 specular = uMaterial.specular * specularStrength;
	
	// Attenuation
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.attConstant + light.attLinear * distance + light.attQuadratic * distance * distance);
	
	return light.color * (ambient + diffuse + specular) * attenuation;
}


void main()
{
    TexCoords = aTexCoords;
	FragPos = vec3(uModel * vec4(aPos, 1.0));
	vec3 norm = normalize(mat3(transpose(inverse(uModel))) * aNormal);
	vec3 viewDir = normalize(uViewPos - FragPos);
	vec3 lightTotal = vec3(0.f);
	
	for(int i = 0; i < MAX_LIGHTS; ++i)
	if(uLight[i].active != 0)
		lightTotal += pointLight(uLight[i], norm, viewDir);
	
	Light = lightTotal;	
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
}