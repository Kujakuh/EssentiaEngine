#ifndef SPRITE_H
#define SPRITE_H

#include <glm/glm.hpp>

#include <string>
#include <memory>
#include <vector>

#include <Core/ECS/IComponent.hpp>
#include <Core/Graphics/Mesh.hpp>
#include <Core/Graphics/Texture.hpp>
#include <Core/Graphics/Shader.hpp>
#include <Core/Graphics/TextureManager.hpp>
#include <Core/Graphics/ShaderGen.hpp>
#include <Shared/user_values.hpp>

namespace Essentia
{
    struct Sprite : public IComponent
    {
        private:
            std::shared_ptr<Shader> shader;
            ShaderGenerator shaderGenerator;

            std::vector<Vertex> getAspectRatioAdjustedVertices(float aspectRatio);
            std::vector<GLuint> getDefaultSquareIndices();
            void initializeShader();

        public:
            std::shared_ptr<Mesh> mesh;
            std::shared_ptr<Texture> texture;

            Sprite(const std::string& texturePath, bool flip = true);
            Sprite(std::shared_ptr<Texture> _texture);

            void setTexture(const std::string& texturePath, bool flip = true);
            void setTexture(std::shared_ptr<Texture> _texture);
            void setTextureData(std::shared_ptr<Texture> texture);
            void addCustomShaderFunction(SH_TYPE type, const std::string& functionCode);
            void addCustomShaderMainCode(SH_TYPE type, const std::string& mainCode);
            void addCustomShaderFunctionFromFile(SH_TYPE type, const std::string& filePath);
            void addCustomShaderMainCodeFromFile(SH_TYPE type, const std::string& filePath);
            void setShader(std::shared_ptr<Shader> _shader);
            std::string getTexturePath() const;
            std::shared_ptr<Texture> getTexture() const;
            std::shared_ptr<Shader> getShader() const;

        private:
            std::string getTextureName(const std::string& path);
    };
}

#endif // !SPRITE_H
