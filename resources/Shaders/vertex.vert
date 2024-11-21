#version 460 core

layout (location = 0) in vec3 p;
layout (location = 1) in vec2 tx;

out vec2 outTexCord;

void main()
{
	outTexCord = tx;
	gl_Position = vec4(p, 1);

}