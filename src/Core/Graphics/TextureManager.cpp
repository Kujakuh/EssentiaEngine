#include <Core/Graphics/TextureManager.hpp>

namespace Essentia
{
    std::unordered_map<std::string, std::shared_ptr<Texture>> TextureManager::textureCache;
    std::set<int> TextureManager::availableUnits;
    int TextureManager::maxTextureUnits = 0;

    void TextureManager::detectMaxTextureUnits()
    {
        if (maxTextureUnits == 0)
        {
            glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
            availableUnits.clear();
            for (int i = 0; i < maxTextureUnits; ++i)
            {
                availableUnits.insert(i);
            }
        }
    }

    int TextureManager::allocateUnit()
    {
        detectMaxTextureUnits();
        if (availableUnits.empty())
        {
            throw std::runtime_error("No available texture units!");
        }

        int unit = *availableUnits.begin();
        availableUnits.erase(unit);
        return unit;
    }

    void TextureManager::releaseUnit(int unit)
    {
        if (unit >= 0 && unit < maxTextureUnits)
        {
            availableUnits.insert(unit);
        }
    }

    std::shared_ptr<Texture> TextureManager::getTexture(const std::string& texturePath, GLenum textureType,
        const ska::flat_hash_map<FILTERS, GLenum>& filters, TEX_TYPE type, bool flip)
    {
        detectMaxTextureUnits();
        auto it = textureCache.find(texturePath);
        if (it != textureCache.end())
        {
            return it->second;
        }

        int textureUnit = allocateUnit();
        std::shared_ptr<Texture> newTexture = std::make_shared<Texture>(
            texturePath.c_str(), textureType, textureUnit, filters, type, flip);
        textureCache[texturePath] = newTexture;
        return newTexture;
    }

    std::shared_ptr<Texture> TextureManager::getCubemapTexture(const std::vector<std::string>& faces, GLenum textureType,
        const ska::flat_hash_map<FILTERS, GLenum>& filters, TEX_TYPE type)
    {
        detectMaxTextureUnits();
        std::string key = generateCubemapKey(faces);

        auto it = textureCache.find(key);
        if (it != textureCache.end())
        {
            return it->second;
        }

        int textureUnit = allocateUnit();
        std::shared_ptr<Texture> newTexture = std::make_shared<Texture>(
            faces, textureType, textureUnit, filters, type);
        textureCache[key] = newTexture;
        return newTexture;
    }

    void TextureManager::clearCache()
    {
        for (auto& [_, texture] : textureCache)
        {
            TextureManager::releaseUnit(texture->getTextureUnit());
        }
        textureCache.clear();
    }

    std::string TextureManager::generateCubemapKey(const std::vector<std::string>& faces)
    {
        std::string key;
        for (const auto& face : faces)
        {
            key += face;
        }
        return key;
    }
}

