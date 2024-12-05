#ifndef MATERIAL_H
#define MATERIAL_H

#include <memory>
#include <glm/glm.hpp>
#include <Core/Graphics/Texture.hpp>

namespace Essentia {

    struct Material 
    {
        std::shared_ptr<Texture> diffuse = nullptr;
        std::shared_ptr<Texture> specular = nullptr;
        std::shared_ptr<Texture> normal = nullptr;
        std::shared_ptr<Texture> height = nullptr;
        std::shared_ptr<Texture> alpha = nullptr;

        glm::vec3 color = glm::vec3(1.0f);
        float shininess = 32.0f;

        void setAllTextures(std::shared_ptr<Texture> diffuse,
            std::shared_ptr<Texture> specular,
            std::shared_ptr<Texture> normal,
            std::shared_ptr<Texture> height,
            std::shared_ptr<Texture> alpha)
        {
            this->diffuse = diffuse;
            this->specular = specular;
            this->normal = normal;
            this->height = height;
            this->alpha = alpha;
        }

        bool hasTexture() const {
            return diffuse || specular || normal || height || alpha;
        }

        bool hasAlpha() const 
        { 
            if (diffuse) return diffuse->hasAlpha() || alpha;
            else return false;
        }
    };
}

#endif // !MATERIAL_H