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
	// 'Limbo' effect
	//FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
	
	vec3 norm = normalize(Normal);
	vec4 objectColor = texture(texture_diffuse, TexCoords);
	
	// Ambient
	vec3 ambient = vec3(uAmbientStrength);
	
	// Diffuse
	vec3 lightDir = normalize(uLightPos - FragPos);
	float diffuseStrength = max(0.0, dot(norm, lightDir)) * uDiffuseStrength;
	vec3 diffuse = vec3(diffuseStrength);
	
	// Specular
	vec3 viewDir = normalize(uViewPos - FragPos);
	vec3 reflectDir = reflect(lightDir, norm);
	float specularStrength = pow(max(0.0, dot(viewDir, reflectDir)), uSpecularExponent) * uSpecularStrength;
	vec3 specular = vec3(specularStrength);
	
	FragColor = objectColor * vec4(ambient + diffuse + specular, 1.0);
}
