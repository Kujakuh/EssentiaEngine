#version 460 core

out vec4 FragColor;
in vec2 outTexCord;

uniform sampler2D Tex;

void main()
{
	FragColor = texture(Tex, outTexCord);
}