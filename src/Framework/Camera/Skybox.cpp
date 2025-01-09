#include <Framework/Camera/Skybox.hpp>

namespace Essentia
{
    const std::string vertexShaderCode = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;

        out vec3 TexCoords;

        uniform mat4 projection;
        uniform mat4 view;

        void main()
        {
            TexCoords = aPos;
            vec4 pos = projection * view * vec4(aPos, 1.0);
            gl_Position = pos.xyww;
        }
    )";

    const std::string fragmentShaderCodeBindless = R"(
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
    )";

    const std::string fragmentShaderCode = R"(
        #version 330 core

        out vec4 FragColor;

        in vec3 TexCoords;

        struct Material {
            samplerCube diffuse;
            samplerCube specular;
            samplerCube normal;
            samplerCube height;
            float shininess;
        };

        uniform Material material;

        void main()
        {    
            FragColor = texture(material.diffuse, TexCoords);
        }
    )";

    Skybox::Skybox(const std::string& path) :
        texture(TextureManager::getTexture(path, GL_TEXTURE_CUBE_MAP, TEX_TYPE::TEX_CUBEMAP)),
        shader(vertexShaderCode.c_str(), fragmentShaderCodeBindless.c_str(), DATA_SOURCE::STR_DATA)
    {
        if (!(GLAD_GL_ARB_bindless_texture && Essentia::bindlessTexturesMode))
            shader.recompileProgram(vertexShaderCode.c_str(), fragmentShaderCode.c_str(), DATA_SOURCE::STR_DATA);

        setupMesh();
    }

    Skybox::Skybox(std::vector<std::string> faces) :
        texture(TextureManager::getCubemapTexture(faces, GL_TEXTURE_CUBE_MAP, TEX_TYPE::TEX_CUBEMAP)),
        shader(vertexShaderCode.c_str(), fragmentShaderCodeBindless.c_str(), DATA_SOURCE::STR_DATA)
    {
        if (!(GLAD_GL_ARB_bindless_texture && Essentia::bindlessTexturesMode))
            shader.recompileProgram(vertexShaderCode.c_str(), fragmentShaderCode.c_str(), DATA_SOURCE::STR_DATA);

        setupMesh();
    }

    void Skybox::setupMesh() 
    {
        Material material;
        material.diffuse = texture;
        mesh = Mesh(
            std::make_shared<Shader>(shader),
            Essentia::cubeVertices,
            Essentia::cubeIndices,
            { material }
        );
    }

    void Skybox::render(glm::mat4 projection, glm::mat4 view) 
    {
        glDepthFunc(GL_LEQUAL);
        shader.use();

        shader.setUniform("projection", projection);
        shader.setUniform("view", view);

        mesh.render();

        shader.disable();
        glDepthFunc(GL_LESS);
    }
}