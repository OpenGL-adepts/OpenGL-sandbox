#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 uViewPos;
uniform samplerCube uSkybox;

void main()
{    
    vec3 I = normalize(Position - uViewPos);
    vec3 R = reflect(I, Normal);
    FragColor = vec4(texture(uSkybox, R).rgb, 1.0);
}
