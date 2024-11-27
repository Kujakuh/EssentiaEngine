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
#include <Shared/user_values.hpp>

namespace Essentia
{
    class Sprite : public IComponent
    {
        private:
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

        public:
            std::shared_ptr<Mesh> mesh;
            std::shared_ptr<Texture> texture;

            Sprite(const std::string& texturePath, Shader& shader, bool flip = true)
            {
                texture = TextureManager::getTexture(texturePath, GL_TEXTURE_2D, TEX_TYPE::TEX_DIFF, Essentia::defaultFilters, flip);

                int width = texture->getWidth();
                int height = texture->getHeight();
                float aspectRatio = static_cast<float>(width) / static_cast<float>(height);

                auto adjustedVertices = getAspectRatioAdjustedVertices(aspectRatio);

                mesh = std::make_shared<Mesh>(
                    shader,
                    adjustedVertices,
                    getDefaultSquareIndices(),
                    std::unordered_map<std::string, std::shared_ptr<Texture>>
                    { 
                        {getTextureName(texturePath), texture}
                    }
                );
            }

            Sprite(std::shared_ptr<Texture> texture, Shader& shader)
            {
                float aspectRatio = static_cast<float>(texture->getWidth()) / static_cast<float>(texture->getHeight());

                auto adjustedVertices = getAspectRatioAdjustedVertices(aspectRatio);

                mesh = std::make_shared<Mesh>(
                    shader,
                    adjustedVertices,
                    getDefaultSquareIndices(),
                    std::unordered_map<std::string, std::shared_ptr<Texture>>
                    { 
                        { getTextureName(TextureManager::getTexturePath(texture)), texture}
                    }
                );
            }

            void setTexture(const std::string& texturePath, Shader& shader, bool flip = true)
            {
                ska::flat_hash_map<FILTERS, GLenum> filters;
                filters[FILTERS::MIN_F] = GL_LINEAR;
                filters[FILTERS::MAG_F] = GL_LINEAR;
                filters[FILTERS::WRAP_S] = GL_REPEAT;
                filters[FILTERS::WRAP_T] = GL_REPEAT;

                texture = TextureManager::getTexture(texturePath, GL_TEXTURE_2D, TEX_TYPE::TEX_DIFF, Essentia::defaultFilters, flip);
                setTexture(texture);
            }

            void setTexture(std::shared_ptr<Texture> texture) 
            {
                int width = texture->getWidth();
                int height = texture->getHeight();
                float aspectRatio = static_cast<float>(width) / static_cast<float>(height);

                auto adjustedVertices = getAspectRatioAdjustedVertices(aspectRatio);
                mesh->updateVertices(adjustedVertices);
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

#endif // !SPRITE_H