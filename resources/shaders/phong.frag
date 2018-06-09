#version 330 core
uniform float uAmbientStrength;
uniform float uDiffuseStrength;
uniform float uSpecularStrength;
uniform int uSpecularExponent;
uniform int uEnableNormalMapping;
uniform vec3 uLightPos;
uniform vec3 uViewPos;
uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform sampler2D texture_normal;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

out vec4 FragColor;


void main()
{
	vec3 norm, lightDir, viewDir;
	vec4 objectColor = texture(texture_diffuse, TexCoords);
	
	if(uEnableNormalMapping != 0)
	{
		// Normal mapping
		norm = normalize(texture(texture_normal, TexCoords).rgb * 2.0 - 1.0);
		lightDir = normalize(TangentLightPos - TangentFragPos);
		viewDir = normalize(TangentViewPos - TangentFragPos);
	}
	else
	{
		// Standard normals
		norm = normalize(Normal);
		lightDir = normalize(uLightPos - FragPos);
		viewDir = normalize(uViewPos - FragPos);
	}
	
	// Ambient
	vec3 ambient = vec3(uAmbientStrength);
	
	// Diffuse
	float diffuseStrength = max(0.0, dot(norm, lightDir)) * uDiffuseStrength;
	vec3 diffuse = vec3(diffuseStrength);
	
	// Specular
	vec3 reflectDir = reflect(-lightDir, norm);
	float specularStrength = pow(max(0.0, dot(viewDir, reflectDir)), uSpecularExponent) * uSpecularStrength;
	vec3 specular = vec3(specularStrength * texture(texture_specular, TexCoords));
	
	FragColor = objectColor * vec4(ambient + diffuse + specular, 1.0);
}
