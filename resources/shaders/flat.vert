#version 330 core
uniform float uAmbientStrength;
uniform float uDiffuseStrength;
uniform float uSpecularStrength;
uniform int uSpecularExponent;
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform vec3 uLightPos;
uniform vec3 uViewPos;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 FragPos;
flat out vec3 LightAmbient;
flat out vec3 LightDiffuse;
flat out float LightSpecular;


void main()
{
    TexCoords = aTexCoords;
	FragPos = vec3(uModel * vec4(aPos, 1.0));
	
	vec3 norm = normalize(mat3(transpose(inverse(uModel))) * aNormal);
	
	vec3 lightDir = normalize(uLightPos - FragPos);
	vec3 viewDir = normalize(uViewPos - FragPos);
	
	// Ambient
	LightAmbient = vec3(uAmbientStrength);
	
	// Diffuse
	float diffuseStrength = max(0.0, dot(norm, lightDir)) * uDiffuseStrength;
	LightDiffuse = vec3(diffuseStrength);
	
	// Specular
	vec3 reflectDir = reflect(-lightDir, norm);
	LightSpecular = pow(max(0.0, dot(viewDir, reflectDir)), uSpecularExponent) * uSpecularStrength;
	
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
}