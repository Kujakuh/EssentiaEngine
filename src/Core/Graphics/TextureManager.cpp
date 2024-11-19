#include <Core/Graphics/TextureManager.hpp>

namespace Essentia
{
    std::unordered_map<std::string, std::shared_ptr<Texture>> TextureManager::textureCache;

    std::shared_ptr<Texture> TextureManager::getTexture(const std::string& texturePath, GLenum textureType, int textureUnit,
        const ska::flat_hash_map<FILTERS, GLenum>& filters, TEX_TYPE type, bool flip)
    {
        auto it = textureCache.find(texturePath);
        if (it != textureCache.end())
        {
            // Si la textura ya está en el cache, la devolvemos
            return it->second;
        }
        else 
        {
            // Si no está en el cache, cargamos una nueva textura
            std::shared_ptr<Texture> newTexture = std::make_shared<Texture>(texturePath.c_str(), textureType, textureUnit, filters, type, flip);
            textureCache[texturePath] = newTexture;
            return newTexture;
        }
    }

    std::shared_ptr<Texture> TextureManager::getCubemapTexture(const std::vector<std::string>& faces, GLenum textureType, int textureUnit,
        const ska::flat_hash_map<FILTERS, GLenum>& filters, TEX_TYPE type)
    {
        std::string key = generateCubemapKey(faces); // Generamos una clave única para el cubemap

        auto it = textureCache.find(key);
        if (it != textureCache.end()) 
        {
            // Si ya está en el cache, la devolvemos
            return it->second;
        }
        else 
        {
            // Si no está en el cache, cargamos una nueva textura cubemap
            std::shared_ptr<Texture> newTexture = std::make_shared<Texture>(faces, textureType, textureUnit, filters, type);
            textureCache[key] = newTexture; // Guardamos la nueva textura en el cache
            return newTexture;
        }
    }

    void TextureManager::clearCache() {textureCache.clear();}

    std::string TextureManager::generateCubemapKey(const std::vector<std::string>& faces) 
    {
        std::string key;
        for (const auto& face : faces) key += face;
        return key;
    }
}
