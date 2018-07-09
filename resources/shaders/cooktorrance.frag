#version 330 core

struct Material
{
	vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
	float roughness;
};

struct PointLight
{
	int active;
	vec3 position;
	vec3 color;
	
	float attConstant;
	float attLinear;
	float attQuadratic;
};

uniform float uAmbientStrength;
uniform float uDiffuseStrength;
uniform float uSpecularStrength;
uniform int uEnableNormalMapping;
uniform vec3 uViewPos;
uniform Material uMaterial;
uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform sampler2D texture_normal;

#define MAX_LIGHTS 16
uniform PointLight uLight[MAX_LIGHTS];

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
in mat3 TBN;

out vec4 FragColor;


vec3 pointLight(PointLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - FragPos);
		
	// Ambient
	vec3 ambient = uMaterial.ambient * uAmbientStrength;
	
	// Specular
	
	float spec = 0.0;
	vec3 halfDir = normalize(viewDir + lightDir);
	
	float LdotN = max(0.0, dot(lightDir, normal));
	float HdotN = max(0.0, dot(halfDir, normal));
	float VdotN = max(0.0, dot(viewDir, normal));
	float VdotH = max(0.0, dot(viewDir, halfDir));
	
	if(LdotN > 0)
	{
		// Schlick fresnel term approx.
		float F0 = 0.8;
		float F = F0 + (1.0 - F0) * pow(1.0 - VdotH, 5);
		
		// Beckmann distribution factor
		float HdotN2 = HdotN * HdotN;
		float m2 = uMaterial.roughness * uMaterial.roughness;
		float D = exp((HdotN2 - 1) / (HdotN2 * m2)) / (4 * m2 * HdotN2 * HdotN2);
		//D = uMaterial.roughness * exp(-pow(acos(HdotN) / uMaterial.roughness, 2.0));
		
		// geometric attenuation term
		float G = clamp(min((2 * HdotN * VdotN) / VdotH, (2 * HdotN * LdotN) / VdotH), 0.0, 1.0);
		spec = F * D * G / (4 * LdotN * VdotN);
	}
	
	vec3 diffuse = uMaterial.diffuse * spec * uSpecularStrength;

	// Attenuation
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.attConstant + light.attLinear * distance + light.attQuadratic * distance * distance);
	
	return light.color * (ambient + diffuse) * attenuation;
}


void main()
{
	vec4 objectColor = vec4(vec3(texture(texture_diffuse, TexCoords)) * uMaterial.color, 1.f);
	vec3 lightTotal = vec3(0.f);
	vec3 norm;
	
	if(uEnableNormalMapping != 0) // Normal mapping
		norm = normalize(TBN * (texture(texture_normal, TexCoords).rgb * 2.0 - 1.0));
	else // Standard normals
		norm = normalize(Normal);
	
	vec3 viewDir = normalize(uViewPos - FragPos);
	
	for(int i = 0; i < MAX_LIGHTS; ++i)
	if(uLight[i].active != 0)
		lightTotal += pointLight(uLight[i], norm, viewDir);
	
	FragColor = objectColor * vec4(lightTotal, 1.0);
}
