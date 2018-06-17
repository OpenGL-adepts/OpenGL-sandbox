#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 Position;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    Normal = mat3(transpose(inverse(uModel))) * aNormal;
    Position = vec3(uModel * vec4(aPos, 1.0));
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
}
