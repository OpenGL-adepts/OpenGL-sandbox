#version 330 core
uniform float uAmbientStrength;
uniform float uDiffuseStrength;
uniform float uSpecularStrength;
uniform int uSpecularExponent;
uniform vec3 uLightPos;
uniform vec3 uViewPos;
uniform sampler2D texture_diffuse;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;


void main()
{
	FragColor = vec4(normalize(Normal), 1.f);
}
