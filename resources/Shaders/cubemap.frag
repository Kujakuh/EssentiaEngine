#version 330 core
#extension GL_ARB_bindless_texture : require

out vec4 FragColor;

in vec3 TexCoords;

layout(bindless_sampler) uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, TexCoords);
}