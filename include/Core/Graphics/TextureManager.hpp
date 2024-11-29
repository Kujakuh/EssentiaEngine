#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <string>
#include <memory>
#include <unordered_map>
#include <set>
#include <vector>

#include <flat_hash_map>
#include <glad/glad.h>
#include <stb_image/stb_image.h>

#include <Core/Graphics/Texture.hpp>
#include <Shared/enums.hpp>
#include <Shared/user_values.hpp>

namespace Essentia
{
    class TextureManager
    {
        private:
            static int maxTextureUnits;

            // Unidades de textura disponibles por shader
            static std::unordered_map<GLuint, std::set<int>> availableUnitsPerShader;

            // Caché de texturas por shader
            static std::unordered_map<GLuint, std::unordered_map<std::string, std::shared_ptr<Texture>>> textureCachePerShader;

            TextureManager() {}
            static void detectMaxTextureUnits();

        public:
            static std::shared_ptr<Texture> getTexture(const std::string& texturePath, GLenum textureType,
                TEX_TYPE type, const ska::flat_hash_map<FILTERS, GLenum>& filters = Essentia::defaultFilters, bool flip = true);

            static std::shared_ptr<Texture> getCubemapTexture(const std::vector<std::string>& faces, GLenum textureType,
                TEX_TYPE type, const ska::flat_hash_map<FILTERS, GLenum>& filters = Essentia::defaultFilters);

            static void clearCache();
            static void releaseUnit(int unit);

            static std::string getTexturePath(const std::shared_ptr<Texture>& texture);

            // Gestión por shader
            static void initializeForShader(GLuint shaderID);

        private:
            static int allocateUnit(GLuint shaderID);
            static std::string generateCubemapKey(const std::vector<std::string>& faces);
    };

}

#endif // !TEXTURE_MANAGER_H