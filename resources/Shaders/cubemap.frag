#version 330 core
#extension GL_ARB_bindless_texture : require

out vec4 FragColor;

in vec3 TexCoords;

struct Material {
    layout(bindless_sampler) samplerCube diffuse;
    layout(bindless_sampler) samplerCube specular;
    layout(bindless_sampler) samplerCube normal;
    layout(bindless_sampler) samplerCube height;
    float shininess;
};

uniform Material material;

void main()
{    
    FragColor = texture(material.diffuse, TexCoords);
}