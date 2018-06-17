#version 330 core

struct Material
{
	vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform vec3 uLightPos;
uniform vec3 uViewPos;
uniform Material uMaterial;
uniform sampler2D texture_diffuse;

in vec3 FragPos;
in vec2 TexCoords;
in vec3 Light;

out vec4 FragColor;


void main()
{
	vec3 objectColor = texture(texture_diffuse, TexCoords).rgb * uMaterial.color;
	FragColor = vec4(objectColor * Light, 1.0);
}
