#version 460 core

out vec4 FragColor;
uniform vec3 a;

#extension GL_ARB_shading_language_include : enable
#if GL_ARB_shading_language_include

#include "utils.frag"

vec4 o =  test(a);
#else

vec4 o = vec4(a, 1.0);
#endif
void main()
{
	FragColor = o;
}