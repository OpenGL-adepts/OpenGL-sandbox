#version 330 core

uniform sampler2D texture_normal;
uniform int uEnableNormalMapping;

in vec3 Normal;
in vec2 TexCoords;
in mat3 TBN;
out vec4 FragColor;


void main()
{
	if(uEnableNormalMapping != 0)
	{
		vec3 nm = normalize(TBN * (texture(texture_normal, TexCoords).rgb * 2.0 - 1.0));
		FragColor = vec4((nm + 1.0) / 2.0, 1.0);
	}
	else
		FragColor = vec4((normalize(Normal) + 1.0) / 2.0, 1.f);
}
