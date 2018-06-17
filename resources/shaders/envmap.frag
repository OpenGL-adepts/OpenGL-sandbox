#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 uViewPos;
uniform samplerCube uSkybox;
uniform float uRefIndex;
uniform int uType;

void main()
{    
	vec3 I = normalize(Position - uViewPos);

	if(uType == 0)
	{
		// Reflection
		vec3 R = reflect(I, Normal);
		FragColor = vec4(texture(uSkybox, R).rgb, 1.0);
	}
	else
	{
		// Refraction
		vec3 R = refract(I, Normal, 1.0 / uRefIndex);
		FragColor = vec4(texture(uSkybox, R).rgb, 1.0);
	}
}
