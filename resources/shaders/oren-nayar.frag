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
uniform int uEnableNormalMapping;
uniform Material uMaterial;
uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;
uniform vec3 uViewPos;

#define MAX_LIGHTS 16
uniform PointLight uLight[MAX_LIGHTS];

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
in mat3 TBN;

out vec4 FragColor;


vec3 pointLight(PointLight light, vec3 normal)
{
	vec3 lightDir = normalize(light.position - FragPos);
		
	// Ambient
	vec3 ambient = uMaterial.ambient * uAmbientStrength;
	
	// Diffuse
	vec3 viewDir = normalize(uViewPos - FragPos);
	float sigma2 = uMaterial.roughness * uMaterial.roughness;
	float A = 1 - 0.5 * sigma2 / (sigma2 + 0.33);
	float B = 0.45 * sigma2 / (sigma2 + 0.09);
	float LdotN = dot(lightDir, normal);
	float VdotN = dot(viewDir, normal);
	float angleLN = acos(LdotN);
	float angleVN = acos(VdotN);
	float alpha = max(angleLN, angleVN);
	float beta  = min(angleLN, angleVN);
	float C = clamp(dot(normalize(viewDir - normal * VdotN), normalize(lightDir - normal * LdotN)), 0.0, 1.0);
	float orenNayarFactor = A + B * C * sin(alpha) * tan(beta);
	vec3 diffuse = uMaterial.diffuse * max(0.0, LdotN) * orenNayarFactor * uDiffuseStrength;
	
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
	
	for(int i = 0; i < MAX_LIGHTS; ++i)
	if(uLight[i].active != 0)
		lightTotal += pointLight(uLight[i], norm);
	
	FragColor = objectColor * vec4(lightTotal, 1.0);
}
