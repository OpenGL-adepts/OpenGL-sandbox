#version 330 core

struct Material
{
	vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

uniform float uAmbientStrength;
uniform float uDiffuseStrength;
uniform float uSpecularStrength;
uniform int uEnableNormalMapping;
uniform int uUseBlinnPhong;
uniform vec3 uViewPos;
uniform Material uMaterial;
uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform sampler2D texture_normal;

uniform vec3 uLightPos;
uniform vec3 uLightColor;

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
	vec4 objectColor = vec4(vec3(texture(texture_diffuse, TexCoords)) * uMaterial.color, 1.f);
	
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
	vec3 ambient = uMaterial.ambient * uAmbientStrength;
	
	// Diffuse
	vec3 diffuse = uMaterial.diffuse * max(0.0, dot(norm, lightDir)) * uDiffuseStrength;
	
	// Specular
	float specAngle;
	
	if(uUseBlinnPhong != 0)
	{
		vec3 halfDir = normalize(lightDir + viewDir);
		specAngle = pow(max(0.0, dot(halfDir, norm)), 4);
	}
	else
	{
		vec3 reflectDir = reflect(-lightDir, norm);
		specAngle = max(0.0, dot(viewDir, reflectDir));
	}
	
	float specularStrength = pow(specAngle, uMaterial.shininess) * uSpecularStrength;
	vec3 specular = uMaterial.specular * specularStrength * vec3(texture(texture_specular, TexCoords));
	
	FragColor = objectColor * vec4(uLightColor * (ambient + diffuse + specular), 1.0);
}
