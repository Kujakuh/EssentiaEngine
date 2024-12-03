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

        glm::vec3 color = glm::vec3(1.0f);
        float shininess = 32.0f;

        void setAllTextures(std::shared_ptr<Texture> diffuse,
            std::shared_ptr<Texture> specular,
            std::shared_ptr<Texture> normal,
            std::shared_ptr<Texture> height) {
            this->diffuse = diffuse;
            this->specular = specular;
            this->normal = normal;
            this->height = height;
        }

        bool hasTexture() const {
            return diffuse || specular || normal || height;
        }
    };

}

#endif // !MATERIAL_H