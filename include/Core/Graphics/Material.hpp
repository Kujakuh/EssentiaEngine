#ifndef MATERIAL_H
#define MATERIAL_H

#include <memory>
#include <glm/glm.hpp>
#include <Core/Graphics/Texture.hpp>

namespace Essentia {

    struct Material
    {
        // Texturas para Blinn-Phong
        std::shared_ptr<Texture> diffuse = nullptr;
        std::shared_ptr<Texture> specular = nullptr;
        std::shared_ptr<Texture> normal = nullptr;
        std::shared_ptr<Texture> height = nullptr;
        std::shared_ptr<Texture> alpha = nullptr;

        // Texturas adicionales para PBR
        std::shared_ptr<Texture> albedo = nullptr;    // Albedo map
        std::shared_ptr<Texture> roughness = nullptr; // Roughness map
        std::shared_ptr<Texture> metallic = nullptr;  // Metallic map
        std::shared_ptr<Texture> ao = nullptr;        // Ambient Occlusion map
        std::shared_ptr<Texture> emissive = nullptr;  // Emissive map (opcional)

        // Propiedades de material
        glm::vec3 color = glm::vec3(1.0f);
        float shininess = 32.0f;
        float metallicValue = 0.0f;         // Valor por defecto si no hay textura metálica
        float roughnessValue = 1.0f;        // Valor por defecto si no hay textura de rugosidad

        // Métodos para configurar texturas
        void setBlinnPhongTextures(std::shared_ptr<Texture> diffuse,
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

        void setPBRTextures(std::shared_ptr<Texture> albedo,
            std::shared_ptr<Texture> roughness,
            std::shared_ptr<Texture> metallic,
            std::shared_ptr<Texture> ao,
            std::shared_ptr<Texture> emissive)
        {
            this->albedo = albedo;
            this->roughness = roughness;
            this->metallic = metallic;
            this->ao = ao;
            this->emissive = emissive;
        }

        bool hasTexture() const {
            return diffuse || specular || normal || height || alpha ||
                albedo || roughness || metallic || ao || emissive;
        }

        bool hasAlpha() const {
            if (diffuse) return diffuse->hasAlpha() || alpha;
            else return false;
        }

        // Métodos para verificar propiedades PBR
        bool isMetallic() const { return metallicValue > 0.0f || metallic != nullptr; }
        bool isRough() const { return roughnessValue < 1.0f || roughness != nullptr; }
    };
}

#endif // !MATERIAL_H
