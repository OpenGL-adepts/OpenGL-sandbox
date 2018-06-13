#version 330 core

uniform float uAmbientStrength;
uniform float uDiffuseStrength;
uniform float uSpecularStrength;
uniform int uSpecularExponent;
uniform vec3 uLightPos;
uniform vec3 uViewPos;
uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;

in vec3 FragPos;
in vec2 TexCoords;
in vec3 FragViewPos;

out vec4 FragColor;


void main()
{
	vec3 norm = normalize(cross(dFdx(FragViewPos), dFdy(FragViewPos)));
	
	vec3 lightDir = normalize(uLightPos - FragPos);
	vec3 viewDir = normalize(uViewPos - FragPos);
	
	// Ambient
	vec3 lightAmbient = vec3(uAmbientStrength);
	
	// Diffuse
	float diffuseStrength = max(0.0, dot(norm, lightDir)) * uDiffuseStrength;
	vec3 lightDiffuse = vec3(diffuseStrength);
	
	// Specular
	vec3 reflectDir = reflect(-lightDir, norm);
	float lightSpecular = pow(max(0.0, dot(viewDir, reflectDir)), uSpecularExponent) * uSpecularStrength;

	vec4 objectColor = texture(texture_diffuse, TexCoords);
	vec3 light = lightAmbient + lightDiffuse + lightSpecular * vec3(texture(texture_specular, TexCoords));
	FragColor = objectColor * vec4(light, 1.0);
}
