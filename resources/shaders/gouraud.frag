#version 330 core

uniform vec3 uLightPos;
uniform vec3 uViewPos;
uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;

in vec3 FragPos;
in vec2 TexCoords;
in vec3 LightAmbient;
in vec3 LightDiffuse;
in float LightSpecular;

out vec4 FragColor;


void main()
{
	vec4 objectColor = texture(texture_diffuse, TexCoords);
	vec3 light = LightAmbient + LightDiffuse + LightSpecular * vec3(texture(texture_specular, TexCoords));
	FragColor = objectColor * vec4(light, 1.0);
}
