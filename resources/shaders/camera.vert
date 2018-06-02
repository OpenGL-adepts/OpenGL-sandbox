#version 330 core
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform vec3 uLightPos;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
//layout (location = 2) in vec2 aTexCoords;

//out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;


void main()
{
    //TexCoords = aTexCoords;
	//Normal = normalize(aNormal);
	Normal = normalize(mat3(transpose(inverse(uModel))) * aNormal);
	FragPos = vec3(uModel * vec4(aPos, 1.0));
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
}