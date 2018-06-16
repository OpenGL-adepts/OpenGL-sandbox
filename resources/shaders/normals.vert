#version 330 core
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec3 Normal;
out vec2 TexCoords;
out mat3 TBN;


void main()
{
	TexCoords = aTexCoords;
	mat3 normalMat = mat3(transpose(inverse(uModel)));
	Normal = normalize(normalMat * aNormal);
	
	// Normal mapping calculations
	vec3 Tangent = normalize(normalMat * aTangent);
	Tangent = normalize(Tangent - dot(Tangent, Normal) * Normal);
	vec3 Bitangent = cross(Normal, Tangent);
	TBN = mat3(Tangent, Bitangent, Normal);
	
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
	
}
