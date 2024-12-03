#version 330 core
#extension GL_ARB_bindless_texture : require

out vec4 FragColor;

in vec2 TexCoords;

struct Material {
    layout(bindless_sampler) sampler2D diffuse;
    layout(bindless_sampler) sampler2D specular;
    layout(bindless_sampler) sampler2D normal;
    layout(bindless_sampler) sampler2D height;
    float shininess;
};

uniform Material material;

void main()
{    
    FragColor = texture(material.diffuse, TexCoords);
}