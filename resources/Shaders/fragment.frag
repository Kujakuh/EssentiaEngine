#version 460 core

out vec4 FragColor;
uniform vec3 a;

void main()
{
	FragColor = vec4(a, 1.0);
}