#version 330 core

uniform int uDepthExponent;

out vec4 FragColor;


void main()
{
	float near = min(gl_DepthRange.near, gl_DepthRange.far);
	float far  = max(gl_DepthRange.near, gl_DepthRange.far);
	
	float depth = pow((gl_FragCoord.z - near) / (far - near), uDepthExponent);
	FragColor = vec4(vec3(depth), 1.0);
}
