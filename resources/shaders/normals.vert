#version 330 core
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 Normal;
out vec2 TexCoords;

void main()
{
	TexCoords = aTexCoords;
	mat3 normalMat = mat3(transpose(inverse(uModel)));
	Normal = normalize(normalMat * aNormal);
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
	
}
