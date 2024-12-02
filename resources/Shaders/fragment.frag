#version 460 core
#extension GL_ARB_bindless_texture : require

out vec4 FragColor;

in vec2 outTexCord;

layout(bindless_sampler) uniform sampler2D container;
uniform sampler2D face;
uniform float time;

void main()
{
	FragColor = mix(texture(container, outTexCord), texture(face, outTexCord), 0);
}