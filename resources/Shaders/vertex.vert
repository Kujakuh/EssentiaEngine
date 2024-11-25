#version 460 core

layout (location = 0) in vec3 pos;
layout (location = 2) in vec2 tex;

out vec2 outTexCord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(pos, 1.0);
    outTexCord = vec2(tex.x, tex.y);
}
