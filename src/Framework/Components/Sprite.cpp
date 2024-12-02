#include <Framework/Components/Sprite.hpp>

namespace Essentia
{
    std::vector<Vertex> Sprite::getAspectRatioAdjustedVertices(float aspectRatio)
    {
        float width = 0.5f;
        float height = 0.5f / aspectRatio;

        return {
            {{-width, -height, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}}, // Bottom-left
            {{ width, -height, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}}, // Bottom-right
            {{ width,  height, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}}, // Top-right
            {{-width,  height, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}  // Top-left
        };
    }

    std::vector<GLuint> Sprite::getDefaultSquareIndices() { return { 0, 1, 2, 2, 3, 0 }; }

    void Sprite::initializeShader()
    {
        std::string vertexCode = shaderGenerator.generateShader2D(SH_TYPE::VERTEX);
        std::string fragmentCode = shaderGenerator.generateShader2D(SH_TYPE::FRAGMENT);
        if (shader)
        {
            shader->recompileProgram(vertexCode.c_str(), fragmentCode.c_str(), DATA_SOURCE::STR_DATA);
        }
        else shader = std::make_shared<Shader>(vertexCode.c_str(), fragmentCode.c_str(), DATA_SOURCE::STR_DATA);
        
    }

    Sprite::Sprite(const std::string& texturePath, bool flip)
    {
        shaderGenerator.addTextureUniform(getTextureName(texturePath));
        initializeShader();
        shader->use();
        texture = TextureManager::getTexture(texturePath, GL_TEXTURE_2D, TEX_TYPE::TEX_DIFF, Essentia::defaultFilters, flip);
        float aspectRatio = static_cast<float>(texture->getWidth()) / static_cast<float>(texture->getHeight());

        Mesh _mesh(
            shader,
            getAspectRatioAdjustedVertices(aspectRatio),
            getDefaultSquareIndices(),
            { {getTextureName(texturePath), texture} }
        );
        mesh = std::make_shared<Mesh>(_mesh);
        shader->disable();
    }

    Sprite::Sprite(std::shared_ptr<Texture> _texture)
    {
        shaderGenerator.addTextureUniform(getTextureName(TextureManager::getTexturePath(_texture)));
        initializeShader();
        texture = _texture;
        float aspectRatio = static_cast<float>(_texture->getWidth()) / static_cast<float>(_texture->getHeight());
        shader->use();
        Mesh _mesh(
            shader,
            getAspectRatioAdjustedVertices(aspectRatio),
            getDefaultSquareIndices(),
            { {getTextureName(TextureManager::getTexturePath(_texture)), _texture} }
        );
        mesh = std::make_shared<Mesh>(_mesh);
        shader->disable();
    }

    void Sprite::setTexture(const std::string& texturePath, bool flip)
    {
        shaderGenerator.removeTextureUniform(getTextureName(TextureManager::getTexturePath(texture)));
        shader->use();
        texture = TextureManager::getTexture(texturePath, GL_TEXTURE_2D, TEX_TYPE::TEX_DIFF, Essentia::defaultFilters, flip);
        shader->disable();
        setTextureData(texture);
    }

    void Sprite::setTexture(std::shared_ptr<Texture> _texture)
    {
        shaderGenerator.removeTextureUniform(getTextureName(TextureManager::getTexturePath(texture)));
        shader->use();
        texture = TextureManager::getTexture(TextureManager::getTexturePath(_texture), GL_TEXTURE_2D, TEX_TYPE::TEX_DIFF);
        shader->disable();
        setTextureData(texture);
    }

    void Sprite::setTextureData(std::shared_ptr<Texture> texture)
    {
        float aspectRatio = static_cast<float>(texture->getWidth()) / static_cast<float>(texture->getHeight());
        mesh->updateVertices(getAspectRatioAdjustedVertices(aspectRatio));
        mesh->textures = { {getTextureName(TextureManager::getTexturePath(texture)), texture} };
        shaderGenerator.addTextureUniform(getTextureName(TextureManager::getTexturePath(texture)));
        initializeShader();
        mesh->shader = shader;
    }

    void Sprite::addCustomShaderFunction(SH_TYPE type, const std::string& functionCode)
    {
        shaderGenerator.addCustomFunction(type, functionCode);
        initializeShader();
        mesh->shader = shader;
    }

    void Sprite::addCustomShaderMainCode(SH_TYPE type, const std::string& mainCode)
    {
        shaderGenerator.addMainCode(type, mainCode);
        initializeShader();
        mesh->shader = shader;
    }

    void Sprite::addCustomShaderFunctionFromFile(SH_TYPE type, const std::string& filePath)
    {
        shaderGenerator.addCustomFunctionFromFile(type, filePath);
        initializeShader();
        mesh->shader = shader;
    }

    void Sprite::addCustomShaderMainCodeFromFile(SH_TYPE type, const std::string& filePath)
    {
        shaderGenerator.addMainCodeFromFile(type, filePath);
        initializeShader();
        mesh->shader = shader;
    }

    void Sprite::setShader(std::shared_ptr<Shader> _shader)
    {
        shader = _shader;
        mesh->shader = shader;
    }

    std::string Sprite::getTexturePath() const { return texture ? TextureManager::getTexturePath(texture) : "No Texture"; }

    std::shared_ptr<Texture> Sprite::getTexture() const { return texture; }

    std::shared_ptr<Shader> Sprite::getShader() const { return shader; }

    std::string Sprite::getTextureName(const std::string& path)
    {
        size_t lastSlash = path.find_last_of("/\\");
        size_t lastDot = path.find_last_of('.');
        if (lastSlash == std::string::npos) lastSlash = -1;
        if (lastDot == std::string::npos || lastDot <= lastSlash) lastDot = path.size();
        return path.substr(lastSlash + 1, lastDot - lastSlash - 1);
    }
}
