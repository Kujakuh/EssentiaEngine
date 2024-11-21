#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <string>
#include <memory>
#include <unordered_map>
#include <stdexcept>
#include <set>

#include <flat_hash_map>
#include <glad/glad.h>
#include <stb_image/stb_image.h>

#include <Core/Graphics/Texture.hpp>
#include <Core/enums.hpp>

namespace Essentia
{
    class TextureManager
    {
        private:
            static std::unordered_map<std::string, std::shared_ptr<Texture>> textureCache;
            static std::set<int> availableUnits;
            static int maxTextureUnits;

            TextureManager() {}
            static void detectMaxTextureUnits();

        public:
            static std::shared_ptr<Texture> getTexture(const std::string& texturePath, GLenum textureType,
                const ska::flat_hash_map<FILTERS, GLenum>& filters, TEX_TYPE type, bool flip = true);

            static std::shared_ptr<Texture> getCubemapTexture(const std::vector<std::string>& faces, GLenum textureType,
                const ska::flat_hash_map<FILTERS, GLenum>& filters, TEX_TYPE type);

            static void clearCache();
            static void releaseUnit(int unit);

        private:
            static int allocateUnit();
            static std::string generateCubemapKey(const std::vector<std::string>& faces);
    };

}

#endif //!TEXTURE_MANAGER_H