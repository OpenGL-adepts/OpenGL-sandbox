#version 330 core
uniform float uAmbientStrength;
uniform vec3 uLightPos;

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;


void main()
{
	// 'Limbo' effect
	//FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
	
	vec3 objectColor = vec3(0.0, 1.0, 0.0);
	vec3 ambient = vec3(uAmbientStrength);
	float diffuseStrength = max(0.0, dot(Normal, normalize(uLightPos - FragPos)));
	vec3 diffuse = vec3(diffuseStrength);
	
	FragColor = vec4(objectColor * (ambient + diffuse), 1.0);
}
