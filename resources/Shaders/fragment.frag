#version 460 core

out vec4 FragColor;

in vec2 outTexCord;

uniform sampler2D container;
uniform sampler2D face;
uniform float time;

void main()
{
	FragColor = mix(texture(container, outTexCord), texture(face, outTexCord), 0);
}