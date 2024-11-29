#ifndef SPRITE_H
#define SPRITE_H

#include <glm/glm.hpp>

#include <string>
#include <memory>

#include <Core/ECS/IComponent.hpp>
#include <Core/Graphics/Mesh.hpp>
#include <Core/Graphics/Texture.hpp>
#include <Core/Graphics/Shader.hpp>
#include <Core/Graphics/TextureManager.hpp>
#include <Core/Graphics/ShaderGen.hpp>
#include <Shared/user_values.hpp>

namespace Essentia
{
    class Sprite : public IComponent
    {
        private:
            std::shared_ptr<Shader> shader;
            ShaderGenerator shaderGenerator;

            std::vector<Vertex> getAspectRatioAdjustedVertices(float aspectRatio)
            {
                float width = 0.5f;
                float height = 0.5f / aspectRatio;

                return {
                    // Positions             // Normals         // UVs
                    {{-width, -height, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}}, // Bottom-left
                    {{ width, -height, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}}, // Bottom-right
                    {{ width,  height, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}}, // Top-right
                    {{-width,  height, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}  // Top-left
                };
            }

            std::vector<GLuint> getDefaultSquareIndices() { return { 0, 1, 2, 2, 3, 0 }; }

            void initializeShader()
            {
                std::string vertexCode = shaderGenerator.generateShader2D(SH_TYPE::VERTEX);
                std::string fragmentCode = shaderGenerator.generateShader2D(SH_TYPE::FRAGMENT);
                //std::string geometryCode = shaderGenerator.generateShader(SH_TYPE::GEOMETRY);

                shader = std::make_shared<Shader>(vertexCode.c_str(), fragmentCode.c_str(), DATA_SOURCE::STR_DATA);
                //shader = std::make_shared<Shader>(vertexCode.c_str(), fragmentCode.c_str(), geometryCode.c_str(), DATA_SOURCE::STR_DATA);
            }

        public:
            std::shared_ptr<Mesh> mesh;
            std::shared_ptr<Texture> texture;

            Sprite(const std::string& texturePath, bool flip = true)
            {
                shaderGenerator.addTextureUniform(getTextureName(texturePath));
                initializeShader();

                texture = TextureManager::getTexture(texturePath, GL_TEXTURE_2D, TEX_TYPE::TEX_DIFF, Essentia::defaultFilters, flip);

                int width = texture->getWidth();
                int height = texture->getHeight();
                float aspectRatio = static_cast<float>(width) / static_cast<float>(height);

                auto adjustedVertices = getAspectRatioAdjustedVertices(aspectRatio);

                Mesh _mesh(
                    shader,
                    adjustedVertices,
                    getDefaultSquareIndices(),
                    { {getTextureName(texturePath), texture} }
                );
                mesh = std::make_shared<Mesh>(_mesh);
            }

            Sprite(std::shared_ptr<Texture> _texture)
            {
                shaderGenerator.addTextureUniform(getTextureName(TextureManager::getTexturePath(_texture)));
                initializeShader();

                texture = _texture;

                float aspectRatio = static_cast<float>(_texture->getWidth()) / static_cast<float>(_texture->getHeight());

                auto adjustedVertices = getAspectRatioAdjustedVertices(aspectRatio);

                Mesh _mesh(
                    shader,
                    adjustedVertices,
                    getDefaultSquareIndices(),
                    { {getTextureName(TextureManager::getTexturePath(_texture)), _texture} }
                );
                mesh = std::make_shared<Mesh>(_mesh);
            }

            void setTexture(const std::string& texturePath, bool flip = true)
            {
                shaderGenerator.removeTextureUniform(getTextureName(TextureManager::getTexturePath(texture)));
                texture = TextureManager::getTexture(texturePath, GL_TEXTURE_2D, TEX_TYPE::TEX_DIFF, Essentia::defaultFilters, flip);
                setTextureData(texture);
            }
            void setTexture(std::shared_ptr<Texture> _texture)
            {
                shaderGenerator.removeTextureUniform(getTextureName(TextureManager::getTexturePath(texture)));
                texture = TextureManager::getTexture(TextureManager::getTexturePath(_texture), GL_TEXTURE_2D, TEX_TYPE::TEX_DIFF);
                setTextureData(texture);
            }

            void setTextureData(std::shared_ptr<Texture> texture)
            {
                int width = texture->getWidth();
                int height = texture->getHeight();
                float aspectRatio = static_cast<float>(width) / static_cast<float>(height);

                auto adjustedVertices = getAspectRatioAdjustedVertices(aspectRatio);
                mesh->updateVertices(adjustedVertices);
                mesh->textures = { {getTextureName(TextureManager::getTexturePath(texture)), texture} };

                shaderGenerator.addTextureUniform(getTextureName(TextureManager::getTexturePath(texture)));
                initializeShader();

                mesh->shader = shader;
            }

            void addCustomShaderFunction(SH_TYPE type, const std::string& functionCode)
            {
                shaderGenerator.addCustomFunction(type, functionCode);
                initializeShader();
            }

            void addCustomShaderMainCode(SH_TYPE type, const std::string& mainCode)
            {
                shaderGenerator.addMainCode(type, mainCode);
                initializeShader();
            }

            std::string getTexturePath() const { return texture ? TextureManager::getTexturePath(texture) : "No Texture"; }
            std::shared_ptr<Texture> getTexture() const { return texture; }

        private:
            std::string getTextureName(const std::string& path)
            {
                size_t lastSlash = path.find_last_of("/\\");
                size_t lastDot = path.find_last_of('.');
                if (lastSlash == std::string::npos) lastSlash = -1;
                if (lastDot == std::string::npos || lastDot <= lastSlash) lastDot = path.size();
                return path.substr(lastSlash + 1, lastDot - lastSlash - 1);
            }
    };
}

#endif // SPRITE_H